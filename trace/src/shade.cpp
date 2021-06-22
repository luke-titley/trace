//------------------------------------------------------------------------------
// Copywrite Luke Titley 2015
//------------------------------------------------------------------------------
#include "trace/shade.h"
//------------------------------------------------------------------------------
#include "trace/assert.h"
#include "trace/shadestack.h"
#include "trace/geoAPI.h"
#include "trace/geoid.h"
#include "trace/radiance.h"
#include "trace/matrix.h"
#include "trace/random.h"
#include "trace/sampledspectrum.h"
#include "trace/shadeAPI.h"
#include "trace/shader.h"
#include "trace/supersampleiterator.h"
#include "trace/surfaceframe.h"
#include "trace/solidangle.h"

namespace tc
{

namespace shade
{

//------------------------------------------------------------------------------
// generateRandomDirection
//------------------------------------------------------------------------------
inline const Ray generateRandomDirection(
    const ShadeAPI& shadeApi, const size_t pitchSamples,
    const float rayPositionOffset, const size_t yawSamples, const size_t i,
    const ShadeStackFrame& frame, const float ignoreRaysCloseToSurface = 0.9f)
{
    const size_t x = i % pitchSamples;  // Pitch iteration
    const size_t y = i / pitchSamples;  // Yaw iteration

    const SurfaceFrame& surfaceFrame =
        shadeApi.shade_getSurfaceFrame(frame.m_traceResult.m_geoId);

    // Work out the position of the previous intersection point.
    const Vector3<float> previousIntersectionPoint =
        frame.m_ray.computePointOnRay(frame.m_traceResult.m_distanceAlongRay);

    const Vector3<float> rayDirection =
        // Generate a stratified ray direction around our surface normal.
        generateStratifiedDirection<generateRandomFloat>(
            ignoreRaysCloseToSurface, yawSamples, pitchSamples, y, x)
            .transform(surfaceFrame.m_tangent, surfaceFrame.m_normal,
                       surfaceFrame.m_bitangent);

    // Modify the ray direction to perhaps point towards a light source.
    // The chances of

    // Offset the starting position of the ray so that we don't
    // intersect with ourselves.
    const Vector3<float> rayPosition =
        previousIntersectionPoint + (rayDirection * rayPositionOffset);

    // Build the ray itself.
    return Ray(rayDirection, rayPosition);
}

//------------------------------------------------------------------------------
void storeShadeResult(const ShadeAPI& shadeApi, ShadeStack& shadeStack,
                      ShadeStackFrame& frame, const SampledSpectrum& color)
{
    // If we're not at the bottom of the shade stack, then add our
    // shade result to our parent shader. Our only purpose is to
    // contribute to the radiance of our parent shader.
    ShadeStackFrame& parentFrame = shadeStack.top(1);
    const Shader& parentShader =
        shadeApi.shade_getSurfaceShader(parentFrame.m_traceResult.m_geoId);
    const Radiance radiance(frame.m_traceResult, color);
    parentShader.accumulate(parentFrame.m_traceResult, radiance, frame.m_ray,
                            shadeApi, parentFrame.m_radianceSum);
}

//------------------------------------------------------------------------------
// computeColor
//------------------------------------------------------------------------------
SampledSpectrum computeColor(const ShadeAPI& shadeApi,
                             const ShadeStackFrame& frame)
{
    const SampledSpectrum white(1.0f);

    // Shadeable
    if (frame.m_traceResult.hasHitSomething())
    {
        // Shade the current point and store the result in 'color'.
        const Shader& shader =
            shadeApi.shade_getSurfaceShader(frame.m_traceResult.m_geoId);
        const SampledSpectrum color =
            shader.shade(frame.getI(), frame.m_radianceSum, frame.m_traceResult,
                         frame.m_ray, white);
        return color;
    }

    // We didn't hit anything.
    return SampledSpectrum(0.0f);
}

//------------------------------------------------------------------------------
// Integrator
//------------------------------------------------------------------------------
Integrator::Integrator(const GeoAPI& geoApi, const ShadeAPI& shadeApi,
                       SearchCache& searchCache, ShadeStack& shadeStack,
                       const size_t maxRayDepth, const size_t qualityLevel,
                       const float rayPositionOffset)
    :
      // Our starting values.
      m_pitchSamples(qualityLevel),
      m_yawSamples(qualityLevel * 4),
      m_numSamples(m_pitchSamples * m_yawSamples),
      m_geoAPI(geoApi),
      m_shadeAPI(shadeApi),
      m_searchCache(searchCache),
      m_shadeStack(shadeStack),
      m_maxRayDepth(maxRayDepth),
      m_qualityLevel(qualityLevel),
      m_rayPositionOffset(rayPositionOffset)
{
    assert(shadeStack.empty());
}

//------------------------------------------------------------------------------
bool Integrator::finalValueIsReady() const
{
    // We have the bottom frame left in the shade stack but we've finished
    // computing it's value.
    assert(m_shadeStack.size() == 1);

    const ShadeStackFrame& bottomFrame = m_shadeStack.back();
    if (bottomFrame.done())
    {
        return true;
    }

    return false;
}

//------------------------------------------------------------------------------
bool Integrator::next(ShadeStackFrame & rootFrame) const
{
    assert(m_shadeStack.empty());

    m_shadeStack.push_back(rootFrame);

    // We have the bottom frame left in the shade stack but we've finished
    // computing it's value.
    if (finalValueIsReady())
    {
        m_shadeStack.pop_back();
        return false;
    }

    // We have work to do to compute the shade stack.
    do
    {
        ShadeStackFrame& frame = m_shadeStack.top();

        const bool maxSamplesReached = frame.getI() == m_numSamples;
        const bool maxRayDepthReached = m_shadeStack.size() == m_maxRayDepth;
        const bool surfaceWasHit = frame.m_traceResult.hasHitSomething();

        // If I need more rays then push another ray onto the stack.
        //
        if (surfaceWasHit && !maxRayDepthReached && !maxSamplesReached &&
            m_shadeAPI.shade_getSurfaceShader(frame.m_traceResult.m_geoId)
                .needsRays())
        {
            const Ray randomRay = generateRandomDirection(
                m_shadeAPI, m_pitchSamples, m_rayPositionOffset, m_yawSamples,
                frame.getI(), frame);

            frame.incrI();

            // Trace the ray.
            const TraceResult traceResult =
                m_geoAPI.geo_trace(m_searchCache, randomRay);

            // Store the new result on the shade stack.
            const ShadeStackFrame newRay(randomRay, traceResult);
            m_shadeStack.push_back(newRay);
        }

        // I don't need any more rays, so I can shade myself, store the result
        // in my parent, and pop the shadeStack.
        else
        {
            if (m_shadeStack.size() > 1)
            {
                const SampledSpectrum color = computeColor(m_shadeAPI, frame);
                storeShadeResult(m_shadeAPI, m_shadeStack, frame, color);
                m_shadeStack.pop_back();
            }
            else
            {
                frame.setDone();
            }
        }

    } while (m_shadeStack.size() != 1);

    rootFrame.m_i = m_shadeStack.back().m_i;
    rootFrame.m_radianceSum = m_shadeStack.back().m_radianceSum;
    m_shadeStack.pop_back();

    return true;
}

//------------------------------------------------------------------------------
const TraceResult& Integrator::getTraceResult() const
{
    assert(m_shadeStack.size() == 1);

    const ShadeStackFrame& frame = m_shadeStack.back();

    return frame.m_traceResult;
}

//------------------------------------------------------------------------------
void Integrator::computeSampledSpectrum(SampledSpectrum& sampledSpectrum,
                                        const ShadeStackFrame& frame) const
{
    sampledSpectrum = computeColor(m_shadeAPI, frame);
}

//------------------------------------------------------------------------------
size_t Integrator::getNumSamples() const
{
    return m_numSamples;
}

}  // namespace shade

}  // namespace tc
