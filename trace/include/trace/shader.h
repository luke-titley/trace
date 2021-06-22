//------------------------------------------------------------------------------
// Copywrite Luke Titley 2015
//------------------------------------------------------------------------------
#ifndef TC_SHADER
#define TC_SHADER
//------------------------------------------------------------------------------
#include <cstdlib>

namespace tc
{
class Radiance;
class Ray;
class ShadeAPI;
class TraceResult;
class SampledSpectrum;

//------------------------------------------------------------------------------
// Shader
//------------------------------------------------------------------------------
/// \brief An abstract interface for working with a shader.
///
/// A concrete tc::Shader implementation must implement two methods.
/// tc::Shader::shade and tc::Shader::accumulate.
/// In order to compute the radiance at a particular point effectively the
/// internal 'shade' mechanism splits the shading process of a point into two
/// steps.
/// - accumulate
/// - shade
///
/// The 'accumulation' step is for combining the radiance values of the
/// reflected
/// rays that contribute to the final radiance of the current point.
/// The 'shade' step takes this value and along with additional geometry
/// information produces a final radiance value.
///
/// An exampe of this is a simple diffuse shader.
/// In the 'accumulate' method we weight our incoming radiance value and add the
/// result to a variable containing an aggregation of all the results.
/// In the 'shade' method we average this result to produce a final radiance
/// value.
//------------------------------------------------------------------------------
class Shader
{
public:
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
    virtual SampledSpectrum shade(size_t radianceCount,
                                  const SampledSpectrum& radianceSum,
                                  const TraceResult& traceResult,
                                  const Ray& ray,
                                  const SampledSpectrum& localColor) const = 0;

    /// \brief Combines the radiance values of incoming rays of light into a
    /// single value.
    ///
    /// \param traceResult The ray intersection result. Contains the distance
    /// along the ray that the intersection occurs and a reference to the item
    /// of geometry that has been hit.
    /// \param radiance The radiance value of an incoming ray of light.
    /// \param ray A contributing ray.
    /// \param shadeApi An interface for querying local geometry information.
    /// Use traceResult.m_geoId as the geoId passed into shadeApi methods.
    virtual void accumulate(const TraceResult& traceResult,
                            const Radiance& radiance, const Ray& ray,
                            const ShadeAPI& shadeApi,
                            SampledSpectrum& result) const = 0;

    /// \return True if this shader needs to trace rays in order to estimate its
    /// integral. False if no ray tracing is needed.
    virtual bool needsRays() const = 0;
};
}  // namespace tc
#endif  // TC_SHADER
