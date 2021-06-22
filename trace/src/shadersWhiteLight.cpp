//------------------------------------------------------------------------------
// Copywrite Luke Titley 2015
//------------------------------------------------------------------------------
#include "trace/shadersWhiteLight.h"
//------------------------------------------------------------------------------

namespace tc
{

//------------------------------------------------------------------------------
// shaders::WhiteLight
//------------------------------------------------------------------------------
SampledSpectrum shaders::WhiteLight::shade(
    const size_t radianceCount, const SampledSpectrum& radianceSum,
    const TraceResult& traceResult, const Ray& ray,
    const SampledSpectrum& localColor) const
{
    return SampledSpectrum(m_intensity);
}

}  // namespace tc
