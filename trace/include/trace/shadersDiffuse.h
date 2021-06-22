//------------------------------------------------------------------------------
// Copywrite Luke Titley 2015
//------------------------------------------------------------------------------
#ifndef TC_SHADERSDIFFUSE
#define TC_SHADERSDIFFUSE
//------------------------------------------------------------------------------
#include "trace/shader.h"
//------------------------------------------------------------------------------
#include <cstdlib>
//------------------------------------------------------------------------------
namespace tc
{
namespace shaders
{
//------------------------------------------------------------------------------
// Diffuse
//------------------------------------------------------------------------------
/// \brief A simple diffuse shader. Provides a concrete implementation of
/// tc::Shader.
//------------------------------------------------------------------------------
class Diffuse : public Shader
{
public:
    inline Diffuse() {};

    /// \brief Computes the radiance of a point whose position is inferred from
    /// ray & traceResult.
    ///
    /// \param radianceCount The number of reflected rays for the point that is
    /// being shaded.
    /// \param radianceSum The sum of the radiance values of the reflected rays.
    /// \param traceResult The ray intersection result. Contains the distance
    /// along the ray that the intersection occurs and a reference to the item
    /// of geometry that has been hit.
    /// \param ray The current camera ray.
    /// \param localColor A multiplier which specifies how much of each
    /// wavelength will be reflected along the camera ray.
    SampledSpectrum shade(size_t radianceCount,
                          const SampledSpectrum& radianceSum,
                          const TraceResult& traceResult, const Ray& ray,
                          const SampledSpectrum& localColor) const;

    /// Combines the radiance values of incoming rays of light into a single
    /// value.
    ///
    /// \param traceResult The ray intersection result. Contains the distance
    /// along the ray that the intersection occurs and a reference to the item
    /// of geometry that has been hit.
    /// \param radiance The radiance value of an incoming ray of light.
    /// \param ray A contributing ray.
    /// \param shadeApi An interface for querying local geometry information.
    /// Use traceResult.m_geoId as the geoId passed into shadeApi methods.
    void accumulate(const TraceResult& traceResult, const Radiance& radiance,
                    const Ray& ray, const ShadeAPI& shadeApi,
                    SampledSpectrum& result) const;

    /// \return True always.
    bool needsRays() const;
};
}  // namespace shaders
}  // namespace tc
#endif  // TC_SHADERSDIFFUSE
