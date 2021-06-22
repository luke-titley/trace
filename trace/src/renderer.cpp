//------------------------------------------------------------------------------
// Copywrite Luke Titley 2015
//------------------------------------------------------------------------------
#include "trace/image.h"
#include "trace/renderer.h"
//------------------------------------------------------------------------------
#include <cfloat>
#include <cmath>
#include <cstdlib>
#include <ctime>
#include <iostream>
#include <vector>
//------------------------------------------------------------------------------

namespace tc
{

//------------------------------------------------------------------------------
// Renderer
//------------------------------------------------------------------------------
Renderer::Renderer(const tc::LogContext& logContext, Image & image,
                   const size_t samplesPerPixel, const size_t qualityLevel,
                   const size_t maxRayDepth, const GeoAPI& geoApi,
                   const ShadeAPI& shadeApi, const size_t threadCount)
    : m_hasNewContent(false),
      m_renderThreads(Range(0, image.getHeight()), geoApi, shadeApi, image,
                      m_arrayLock, m_hasNewContent, logContext, maxRayDepth,
                      qualityLevel, samplesPerPixel, threadCount),
      m_renderProgress(m_renderThreads,
                       image.getWidth() * image.getHeight() * samplesPerPixel),
      m_state(kStart)
{
}

//------------------------------------------------------------------------------
bool Renderer::next()
{
    switch (m_state)
    {
        // Start render
        case kRenderingStart:
            m_renderThreads.start();
            m_state = kRendering;
            break;

        // Report on render progress
        case kRendering:
            m_state = m_renderProgress.next() ? m_state : kRenderingFinish;
            break;

        // Rendering is finished, join the threads to this main thread
        case kRenderingFinish:
            m_renderThreads.join();
            m_state = kFinished;
            break;

        // We are finished, nothing to do
        case kFinished:
            break;

        default:
            break;
    };
    return m_state != kFinished;
}

//------------------------------------------------------------------------------
float Renderer::computePercentComplete() const
{
    return m_renderProgress.computePercentComplete();
}

//------------------------------------------------------------------------------
bool Renderer::beginReadArray()
{
    m_arrayLock.readLock();
    return m_hasNewContent;
}

//------------------------------------------------------------------------------
void Renderer::endReadArray()
{
    m_hasNewContent = false;
    m_arrayLock.unlock();
}

}  // namespace tc
