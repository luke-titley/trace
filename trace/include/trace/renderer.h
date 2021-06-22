//------------------------------------------------------------------------------
// Copywrite Luke Titley 2015
//------------------------------------------------------------------------------
#ifndef TC_RENDERER
#define TC_RENDERER
//------------------------------------------------------------------------------
#include "trace/renderThreads.h"
//------------------------------------------------------------------------------
#include <cstdlib>
//------------------------------------------------------------------------------

namespace tc
{

class GeoAPI;
class Image;
class PixelIteratorFactory;
class Renderer;
class ShadeAPI;
class TriangleIterator;

//------------------------------------------------------------------------------
// Renderer
//------------------------------------------------------------------------------
/// \brief Manages the separate threads needed to render an image.
///
/// Provides an iterator like interface which permits progressively viewing the
/// image as it is being rendered.
///
/// tc::Renderer::beginReadArray and tc::Renderer::endReadArray must be called
/// when accessing the image whilst rendering.
///
/// \code
/// Renderer renderer(logContext,
///                   image,
///                   linearIteratorFactory,
///                   true,     // Yes to report progress
///                   16,       // 16 samples per pixel
///                   3,        // 3 samples per fragment
///                   2,        // ray depth
///                   geoApi,   // the geometry
///                   shadeApi, // close-up geoemtry
///                   4)        // number of threads to use
///
/// time_t lastTime = time();
/// while(renderer.next())
/// {
///     time_t currentTime = time();
///
///     // Every two seconds, read the image.
///     if(currentTime - lastTime > 2)
///     {
///         bool hasChanged = renderer.beginReadArray();
///         if(hasChanged)
///         {
///             // The image has some rendered pixels written to it
///             // write this to a qt window to display it.
///         }
///         renderer.endReadArray();
///         lastTime = currentTime;
///     }
/// }
/// \endcode
class Renderer
{
public:
    /// \brief Initialises a Renderer instance.
    ///
    /// \param logContext A logger, which is used for reporting render progress
    /// information and errors.
    /// \param array This is where the image will be rendered to. The array is
    /// the final image. This need not be thread safe as is protected internally
    /// in the renderer by a read/write lock.
    /// \param samplesPerPixel The number of samples to render and then average
    /// in order to determin the final pixel value. A value of 16 or above is
    /// needed for smooth antialiasing.
    /// \param qualityLevel A hint for the number of rays to fire per hemisphere
    /// in order to estimate the radiance of a particular point.
    /// The number of rays per hemisphere is: qualityLevel * (qualityLevel * 4).
    /// \param maxRayDepth The maximum number of bounces permitted for a camera
    /// ray.
    /// \param geoApi The geometry interface. This provides a thread safe
    /// 'trace' function, for casting rays in the scene.
    /// \param shadeApi The shade interface. This provides a thread safe set of
    /// functions for querying additional geoemtry information such as the
    /// normal and tangents for a particular point.
    /// \param threadCount Gives us the number of threads to use for rendering.
    Renderer(const tc::LogContext& logContext, Image & image,
             const size_t samplesPerPixel, const size_t qualityLevel,
             const size_t maxRayDepth, const GeoAPI& geoApi,
             const ShadeAPI& shadeApi, const size_t threadCount);

    /// \return The total progress of the render as a percentage.
    float computePercentComplete() const;

    /// Allows for iteration until rendering has finished. Rendering progress is
    /// written to the tc::LogContext passed into tc::Renderer constructor.
    /// \return false when rendering is done, otherwise return true.
    bool next();

    /// For thread safe access to the image (tc::Array).
    /// \usage beginReadArray must be called prior to accessing the image for
    /// reading.
    /// \return true if the array has changed since the last time beginReadArray
    /// was invoked and false if nothing has changed. This can be used to avoid
    /// copying the latest image to the UI if nothing has changed.
    bool beginReadArray();

    /// For thread safe access to the image (tc::Array).
    /// \usage endReadArray must be called after accessing the image for
    /// reading.
    void endReadArray();

private:
    RWLock m_arrayLock;
    bool m_hasNewContent;
    RenderThreads m_renderThreads;
    ProgressReader m_renderProgress;

    enum State
    {
        kRenderingStart = 0,
        kRendering = 1,
        kRenderingFinish = 2,
        kFinished = 3,
        kStart = kRenderingStart
    };
    State m_state;
};

//------------------------------------------------------------------------------
// ArrayReadLock
//------------------------------------------------------------------------------
/// \brief RAII object for protecting the array being rendered to whilst it is
/// being read.
///
/// Can be used instead of tc::Renderer::beginReadArray and
/// tc::Renderer::endReadArray.
///
/// The benefit of using a tc::ArrayReadLock vs tc::Renderer::beginReadArray and
/// tc::Renderer::endReadArray is that is uses the c++ scope block to ensure
/// the cleanup code is always invoked. ArrayReadLock is the best construct to
/// use when working with exceptions.
///
/// \code
/// Renderer renderer(logContext,
///                   image,
///                   linearIteratorFactory,
///                   true,     // Yes to report progress
///                   16,       // 16 samples per pixel
///                   3,        // 3 samples per fragment
///                   2,        // ray depth
///                   geoApi,   // the geometry
///                   shadeApi, // close-up geoemtry
///                   4)        // number of threads to use
///
/// time_t lastTime = time();
/// while(renderer.next())
/// {
///     time_t currentTime = time();
///
///     // Every two seconds, read the image.
///     if(currentTime - lastTime > 2)
///     {
///         ArrayReadLock lock(renderer);
///         if(lock.hasContentChanged())
///         {
///             // The image has some rendered pixels written to it
///             // write this to a qt window to display it.
///         }
///     }
/// }
/// \endcode
//------------------------------------------------------------------------------
class ArrayReadLock
{
public:
    /// Initializes a new ArrayReadLock
    /// \param renderer The renderer whose image array will be read from.
    inline ArrayReadLock(Renderer& renderer);
    inline ~ArrayReadLock();

    /// Returns true if the image array has changed since the last time it was
    /// accessed. Return false if it hasn't. This can be used to avoid updating
    /// the UI unnecessarily if nothing has changed in the image.
    inline bool hasNewContent() const;

private:
    Renderer& m_renderer;
    const bool m_hasNewContent;
};

//------------------------------------------------------------------------------
inline ArrayReadLock::ArrayReadLock(Renderer& renderer)
    : m_renderer(renderer), m_hasNewContent(renderer.beginReadArray())
{
}

//------------------------------------------------------------------------------
inline bool ArrayReadLock::hasNewContent() const
{
    return m_hasNewContent;
}

//------------------------------------------------------------------------------
inline ArrayReadLock::~ArrayReadLock()
{
    m_renderer.endReadArray();
}

}  // namespace tc
#endif
