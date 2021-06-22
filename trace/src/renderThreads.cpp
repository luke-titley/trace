//------------------------------------------------------------------------------
// Copywrite Luke Titley 2015
//------------------------------------------------------------------------------
#include "trace/renderThreads.h"
//------------------------------------------------------------------------------
#include "trace/geoAPI.h"
#include "trace/image.h"
#include "trace/triangleCache.h"
#include "trace/shadestack.h"
#include "trace/pixelIterator.h"
#include "trace/radiance.h"
#include "trace/random.h"
#include "trace/sampledspectrum.h"
#include "trace/supersampleiterator.h"
#include "trace/shade.h"

namespace tc
{

//------------------------------------------------------------------------------
Vector3<float> computePixelLocationInWorldSpace(
    const Vector3<size_t>& pixel, const Vector3<size_t> dimensions)
{
    const float ratio =
        static_cast<float>(dimensions.x) / static_cast<float>(dimensions.y);

    const Vector3<float> pixelAsFloat(
        static_cast<float>(dimensions.x - pixel.x),
        static_cast<float>(dimensions.y - pixel.y), 0.0f);

    const Vector3<float> worldSpace(
        ((pixelAsFloat.x / dimensions.x) - 0.5f) * ratio,
        (pixelAsFloat.y / dimensions.y) - 0.5f, 1.0f);
    return worldSpace;
}

//------------------------------------------------------------------------------
Vector3<float> sampledSpectrumToRGB(const SampledSpectrum& sampledSpectrum)
{
    // TODO LT: Currently we are assuming BW only.
    // This function needs to be implemented properly to convert from a
    // collection of wavelength intensities to three RGB colour values.
    Vector3<float> result(sampledSpectrum.getWavelength(0));
    result.a = 1.0f;
    return result;
}

//------------------------------------------------------------------------------
Bounds<size_t> computeTileBounds(const size_t idx, const Vector3<size_t> step,
                                 const size_t divisions,
                                 const Vector3<size_t> dimensions,
                                 const size_t maxBlocks)
{
    const size_t block = idx % maxBlocks;
    const Vector3<size_t> blockXY(block % divisions, block / divisions);
    const Vector3<size_t> lower = step * blockXY;
    Vector3<size_t> upper = lower + step;

    if (blockXY.x == (divisions - 1))
    {
        upper.x += dimensions.width % step.x;
    }
    if (blockXY.y == (divisions - 1))
    {
        upper.y += dimensions.height % step.y;
    }

    return Bounds<size_t>(lower, upper);
}

//------------------------------------------------------------------------------
// RenderThreads
//------------------------------------------------------------------------------
RenderThreads::RenderThreads(const Range& range, const GeoAPI& geoApi,
                             const ShadeAPI& shadeApi, Image& image,
                             RWLock& arrayLock, bool& hasNewContent,
                             const tc::LogContext& logContext,
                             const size_t maxRayDepth,
                             const size_t qualityLevel,
                             const size_t samplesPerPixel,
                             const size_t threadCount)
    : ThreadBundle(range, threadCount),
      m_blocks(0),
      m_geoApi(geoApi),
      m_shadeApi(shadeApi),
      m_image(image),
      m_arrayLock(arrayLock),
      m_hasNewContent(hasNewContent),
      m_logContext(logContext),
      m_maxRayDepth(maxRayDepth),
      m_qualityLevel(qualityLevel),
      m_samplesPerPixel(samplesPerPixel)
{
}

//------------------------------------------------------------------------------
size_t RenderThreads::getBlock()
{
    return __sync_fetch_and_add(&m_blocks, 1);
}

//------------------------------------------------------------------------------
void RenderThreads::run(const size_t threadIndex, const Range& range)
{
    const Vector3<size_t> dimensions =
        Vector3<size_t>(m_image.getWidth(), m_image.getHeight());

    const Vector3<float> pixelSize(1.0f / static_cast<float>(dimensions.x),
                                   1.0f / static_cast<float>(dimensions.y));

    SearchCache searchCache;
    ShadeStack shadeStack;

    shade::Integrator integrator(m_geoApi, m_shadeApi, searchCache,
                                 shadeStack, m_maxRayDepth,
                                 m_qualityLevel, 0.0001f);  // TODO LT: Expose this as a parameter

#if 1
    const size_t divisions =
        getThreadCount() * 4;  // The number of divisions in
                               // the x and y axies, for making
                               // blocks of pixels to render.
#else
    const size_t divisions =
        getThreadCount();  // The number of divisions in
                           // the x and y axies, for making
                           // blocks of pixels to render.
#endif
    const Vector3<size_t> step = dimensions / divisions;
    const size_t maxBlocks = (divisions * divisions);
    const size_t iterations = maxBlocks * m_samplesPerPixel;

    // Allocate a temporary buffer to store a tile in.
    // Store slightly more space to account for dimensions that don't cleanly
    // divide.
    const Vector3<size_t> tileDimensions = step + 1;
    tc::Array<float, 4> tile(tileDimensions.width, tileDimensions.height);

    const float black[] = {0.0f, 0.0f, 0.0f, 0.0f};
    tile.fill(black);

    // Iterate over all the pixels in the block we are rendering.
    for (size_t idx = getBlock(); idx < iterations; idx = getBlock())
    {
        const size_t superSample = idx / maxBlocks;
        const Bounds<size_t> tileBounds =
            computeTileBounds(idx, step, divisions, dimensions, maxBlocks);

        // Copy our data as it stands from the global array.
        {
            RWLock_Read readFromArray(m_arrayLock);

            for (size_t y = tileBounds.m_min.y; y != tileBounds.m_max.y; ++y)
            {
                for (size_t x = tileBounds.m_min.x; x != tileBounds.m_max.x;
                     ++x)
                {
                    const Vector3<size_t> pixel(x, y);
                    const Vector3<size_t> pixelInTile =
                        pixel - tileBounds.m_min;

                    Vector3<float> pixelColor;
                    m_image.getPixel(pixel.x, pixel.y, pixelColor);
                    for (size_t channel = 0; channel != 4; ++channel)
                    {
                        tile.setValue(pixelInTile.x, pixelInTile.y, channel,
                                      pixelColor[channel]);
                    }
                }
            }
        }

        for (size_t x = tileBounds.m_min.x; x != tileBounds.m_max.x; ++x)
        {
            for (size_t y = tileBounds.m_min.y; y != tileBounds.m_max.y; ++y)
            {
                if (shouldStop())
                {
                    return;
                }

                const Vector3<size_t> pixel(x, y);
                const Vector3<float> fragment =
                    computePixelLocationInWorldSpace(pixel, dimensions);

                SampledSpectrum sampledSpectrum;

                //  The coordinates relative to the tile.
                const Vector3<size_t> pixelInTile = pixel - tileBounds.m_min;

                // Generate a point on the image plane that is randomly offset
                // from the given pixel position, but within the distance
                // dictated by pixelSize.
                const float jitter_x =
                    generateRandomFloat(fragment.x,
                                        fragment.x + pixelSize.x);
                const float jitter_y =
                    generateRandomFloat(fragment.y,
                                        fragment.y + pixelSize.y);

                const Vector3<float> offset(jitter_x, jitter_y);

                // Build a ray that starts at the origin 0,0 and intersects the
                // point defined by jitter_x/y on the image plane. We will fire
                // this ray into scene and if it hits anything we'll shade the
                // collision point.
                const Vector3<float> ray_direction =
                    offset.overwrite(Vector3<float>::kZ, fragment.z)
                        .normalized();

                const Vector3<float> sensorCentre(0.0f, 0.0f, 0.0f);
                const Ray ray(ray_direction, sensorCentre);

                // If we've hit something then there is a value to shade.
                ShadeStackFrame frame(ray, m_geoApi.geo_trace(searchCache,ray));

                // Estimate the result.
                while(integrator.next(frame))
                {
                }

                integrator.computeSampledSpectrum(sampledSpectrum, frame);

                const Vector3<float> color =
                    sampledSpectrumToRGB(sampledSpectrum);

                // Get the previous pixel value so that we can merge our
                // value with the new value.
                const Vector3<float> originalValue(
                    tile.getValue(pixelInTile.x, pixelInTile.y, 0),
                    tile.getValue(pixelInTile.x, pixelInTile.y, 1),
                    tile.getValue(pixelInTile.x, pixelInTile.y, 2),
                    tile.getValue(pixelInTile.x, pixelInTile.y, 3));

                const float one_over_n =
                    1.0f / static_cast<float>(superSample + 1);

                const Vector3<float> finalValue =
                    originalValue +
                    ((color * one_over_n) - (originalValue * one_over_n));

                // Write the pixels out to the tile.
                tile.setValue(pixelInTile.x, pixelInTile.y, 0, finalValue.r);
                tile.setValue(pixelInTile.x, pixelInTile.y, 1, finalValue.g);
                tile.setValue(pixelInTile.x, pixelInTile.y, 2, finalValue.b);
                tile.setValue(pixelInTile.x, pixelInTile.y, 3, finalValue.a);
            }
        }

        // Report progress per tile iteration.
        progressIncrement(tileBounds.computeDimensions().area());

        // Copy the new data to m_image.
        {
            RWLock_Write writeToArray(m_arrayLock);

            // Indicate that 'we have new content!'
            m_hasNewContent = true;

            // We're done with the first iteration, copy this tile out to the
            // actual image.
            for (size_t y = tileBounds.m_min.y; y != tileBounds.m_max.y; ++y)
            {
                for (size_t x = tileBounds.m_min.x; x != tileBounds.m_max.x;
                     ++x)
                {
                    const Vector3<size_t> pixel(x, y);
                    const Vector3<size_t> pixelInTile =
                        pixel - tileBounds.m_min;

                    Vector3<float> pixelColor;
                    for (size_t channel = 0; channel != 4; ++channel)
                    {
                        pixelColor[channel] = tile.getValue(pixelInTile.x,
                                      pixelInTile.y, channel);
                    }
                    m_image.setPixel(pixel.x, pixel.y, pixelColor);
                }
            }
        }
    }
}

//------------------------------------------------------------------------------
RenderThreads::~RenderThreads()
{
    stop();
    join();
}

}  // namespace tc
