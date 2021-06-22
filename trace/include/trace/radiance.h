//------------------------------------------------------------------------------
// Copywrite Luke Titley 2015
//------------------------------------------------------------------------------
#ifndef TC_RADIANCE
#define TC_RADIANCE
//------------------------------------------------------------------------------
#include "trace/sampledspectrum.h"
#include "trace/traceResult.h"
#include "trace/vector.h"
//------------------------------------------------------------------------------
#include <cstdlib>
//------------------------------------------------------------------------------
namespace tc
{

//------------------------------------------------------------------------------
// Radiance
//------------------------------------------------------------------------------
/// \brief Stores the information necessary to determine the intersection point
/// of a given ray with geometry and the intensities of the visible
/// electromagnetic wavelengths for the given point.
///
/// A tc::Radiance class instance is produced when a camera ray intersects a
/// point in the scene and the point is shaded. Typically we sample multiple
/// radiance values for a single pixel on the camera plane.
//------------------------------------------------------------------------------
class Radiance
{
public:
    /// Contains the information necessary to determin the intersection point of
    /// a ray with scene geometry.
    const TraceResult m_traceResult;

    /// Contains the intensity of the light at the point defined in
    /// m_traceResult.
    const SampledSpectrum m_color;

    /// Initializes tc::Radiance
    /// \param traceResult Contains the information necessary to determin the
    /// intersection point of a ray with scene geometry.
    /// \param sampledSpectrum The intensity of the light at the point defined
    /// by traceResult.
    Radiance(const TraceResult& traceResult = TraceResult(),
             const SampledSpectrum sampledSpectrum = SampledSpectrum())
        : m_traceResult(traceResult), m_color(sampledSpectrum)
    {
    }
};

}  // namespace tc
#endif  // TC_RADIANCE
