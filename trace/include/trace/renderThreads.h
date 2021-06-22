//------------------------------------------------------------------------------
// Copywrite Luke Titley 2015
//------------------------------------------------------------------------------
#ifndef TC_RENDERTHREADS
#define TC_RENDERTHREADS
//------------------------------------------------------------------------------
#include "trace/array.h"
#include "trace/shadestack.h"
#include "trace/thread.h"
#include "trace/vector.h"
//------------------------------------------------------------------------------

namespace tc
{

class GeoAPI;
class Image;
class LogContext;
class PixelIteratorFactory;
class ShadeAPI;

//------------------------------------------------------------------------------
// RenderThreads
//------------------------------------------------------------------------------
/// \brief A concrete 'ThreadBundle' implementation, which handles actual
/// rendering of the image.
///
/// A thread safe progress value can be got by calling
/// tc::ThreadBundle::progressRead.
//------------------------------------------------------------------------------
class RenderThreads : public ThreadBundle
{
public:
    /// Initializes a tc::RenderThreads instance.
    /// \param range A 'Range' value going from 0 to image height. This will
    ///  be split up and shared between render threads.
    /// \param geoApi An interface which implements a fast 'trace' function
    /// for casting rays into the scene.
    /// \param shadeApi And interface which implements methods for quickly
    /// query additional geoemtry information, like the tangent and normal
    /// of a given point.
    /// \param image The image to render to.
    /// \param arrayLock A Read/Write lock for protecting access to the
    /// image.
    /// \param hasNewContent A reference to a boolean value which is set to
    /// 'true' when the array is written to and false when the array is
    /// read.
    /// \param logContext A logger for writing progress and error
    /// information.
    /// \param maxRayDepth The maximum number of bounces a camera ray is
    /// allowed.
    /// \param qualityLevel A hint for the number of rays to fire per hemisphere
    /// in order to estimate the radiance of a particular point.
    /// The number of rays per hemisphere is: qualityLevel * (qualityLevel * 4)
    /// \param samplesPerPixel The number of samples to shade and average in
    /// order to obtain a pixel color. This needs to be 16 or above to
    /// obtain smooth antialiasing.
    /// \param threadCount The number of threads to use for rendering.
    ///
    RenderThreads(const Range& range, const GeoAPI& geoApi,
                  const ShadeAPI& shadeApi, Image& image,
                  RWLock& arrayLock, bool& hasNewContent,
                  const tc::LogContext& logContext, const size_t maxRayDepth,
                  const size_t qualityLevel, const size_t samplesPerPixel,
                  const size_t threadCount);

    virtual ~RenderThreads();

    size_t getBlock();

private:
    size_t m_blocks;
    const GeoAPI& m_geoApi;
    const ShadeAPI& m_shadeApi;
    Image& m_image;
    RWLock& m_arrayLock;
    bool& m_hasNewContent;
    const LogContext& m_logContext;
    const size_t m_maxRayDepth;
    const size_t m_qualityLevel;
    const size_t m_samplesPerPixel;
    virtual void run(const size_t threadIndex, const Range& range);
};

}  // namespace tc
#endif  // TC_RENDERTHREADS
