//------------------------------------------------------------------------------
// Copywrite Luke Titley 2015
//------------------------------------------------------------------------------
#ifndef TC_SHADERSWHITELIGHT
#define TC_SHADERSWHITELIGHT
//------------------------------------------------------------------------------
#include "trace/sampledspectrum.h"
#include "trace/shader.h"
//------------------------------------------------------------------------------
#include <cstdlib>
//------------------------------------------------------------------------------
namespace tc
{
namespace shaders
{
//------------------------------------------------------------------------------
// WhiteLight
//------------------------------------------------------------------------------
/// \brief Emits white light.
//------------------------------------------------------------------------------
class WhiteLight : public Shader
{
public:
    inline WhiteLight(const float intensity) : m_intensity(intensity) {};

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

    /// \brief Does nothing. Implemented only because tc::shaders::WhiteLight
    /// inherits tc::Shader.
    void accumulate(const TraceResult& traceResult, const Radiance& radiance,
                    const Ray& ray, const ShadeAPI& shadeApi,
                    SampledSpectrum& result) const
    {
    }

    /// \return False always.
    bool needsRays() const
    {
        return false;
    }

private:
    const float m_intensity;
};
}  // namespace shaders
}  // namespace tc
#endif  // TC_SHADERSWHITELIGHT
