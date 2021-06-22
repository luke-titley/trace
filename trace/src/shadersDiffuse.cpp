//------------------------------------------------------------------------------
// Copywrite Luke Titley 2015
//------------------------------------------------------------------------------
#include "trace/shadersDiffuse.h"
//------------------------------------------------------------------------------
#include "trace/assert.h"
#include "trace/geoid.h"
#include "trace/radiance.h"
#include "trace/ray.h"
#include "trace/shadeAPI.h"
#include "trace/surfaceframe.h"
#include "trace/traceResult.h"
#include "trace/intersect.h"

#include <cfloat>

namespace tc
{

//------------------------------------------------------------------------------
// shaders::Diffuse
//------------------------------------------------------------------------------
void shaders::Diffuse::accumulate(const TraceResult& traceResult,
                                  const Radiance& radiance, const Ray& ray,
                                  const ShadeAPI& shadeApi,
                                  SampledSpectrum& result) const
{

    if (radiance.m_traceResult.hasHitSomething())
    {
        const SurfaceFrame& surfaceFrame =
            shadeApi.shade_getSurfaceFrame(traceResult.m_geoId);
        const float angularFalloff = ray.m_direction.dot(surfaceFrame.m_normal);

        const float distanceFalloff =
            radiance.m_traceResult.m_distanceAlongRay >= 1.0f
                ? 1.0f / radiance.m_traceResult.m_distanceAlongRay
                : 1.0f;

        const SampledSpectrum color =
            radiance.m_color * (angularFalloff * distanceFalloff);
        result += color;
    }
}

//------------------------------------------------------------------------------
bool shaders::Diffuse::needsRays() const
{
    return true;
}

//------------------------------------------------------------------------------
SampledSpectrum shaders::Diffuse::shade(const size_t radianceCount,
                                        const SampledSpectrum& radianceSum,
                                        const TraceResult& traceResult,
                                        const Ray& ray,
                                        const SampledSpectrum& localColor) const
{
    if (radianceCount == 0)
    {
        return SampledSpectrum(0.0f);
    }

    const float radianceCountFloat = static_cast<float>(radianceCount);
    const SampledSpectrum luminence = radianceSum / radianceCountFloat;
    const SampledSpectrum coloredSample = (localColor * luminence);

    return coloredSample;
}

}  // namespace tc
