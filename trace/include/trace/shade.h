//------------------------------------------------------------------------------
// Copywrite Luke Titley 2015
//------------------------------------------------------------------------------
#ifndef TC_SHADE
#define TC_SHADE
//------------------------------------------------------------------------------
#include <cstdlib>
//------------------------------------------------------------------------------
namespace tc
{
class GeoAPI;
class Ray;
class SampledSpectrum;
class SearchCache;
class ShadeAPI;
class ShadeStack;
class ShadeStackFrame;
class TraceResult;

namespace shade
{

//------------------------------------------------------------------------------
// Integrator
//------------------------------------------------------------------------------
/// \brief Estimates the radiance for the nearest point in the scene that
/// intersects the given ray.
class Integrator
{
public:
    /// Initializes the Integrator.
    /// \param geoApi: Implements a fast 'trace' function for ray casting
    /// against scene geoemtry.
    /// \param shadeApi: Implements fast methods for querying shader specific
    /// local geometry information.
    /// \param searchCache: Reusable memory for acceleration structure search
    /// results.
    /// \param shadeStack: For recursive shader evaluation.
    /// \param maxRayDepth: The maximum number of times a ray is allowed to
    /// bounce and shatter before path tracing is stopped.
    /// \param qualityLevel A hint for the number of rays to fire per hemisphere
    /// in order to estimate the radiance of a particular point.
    /// The number of rays per hemisphere is: qualityLevel * (qualityLevel * 4)
    /// \param rayPositionOffset: A tiny delta to offset the bounce ray start
    /// position by, in the direction of the ray. This stops bounce rays from
    /// immediately intersecting with the surface they are emitted from.
    Integrator(const GeoAPI& geoApi, const ShadeAPI& shadeApi,
               SearchCache& searchCache, ShadeStack& shadeStack,
               const size_t maxRayDepth, const size_t qualityLevel,
               const float rayPositionOffset);

    /// \return true if the final radiance value for this integrator has been
    /// computed, false if not.
    bool finalValueIsReady() const;

    /// \return true if the integrator must keep iterating, false if not.
    bool next(ShadeStackFrame& rootFrame) const;

    /// \return The tc::TraceResult object produced when the ray passed to
    /// tc::Integrator::Integrator was traced in the scene.
    const TraceResult& getTraceResult() const;

    /// \param sampledSpectrum: A reference to a tc::SampledSpectrum instance
    /// that will be populated with a value for the currently available
    /// radiance estimate.
    void computeSampledSpectrum(SampledSpectrum& sampledSpectrum,
                                const ShadeStackFrame& frame) const;

    size_t getNumSamples() const;

private:
    // TODO LT: Order this alphabetically and group const members seperately to
    // non-const members.
    const size_t m_pitchSamples;
    const size_t m_yawSamples;
    const size_t m_numSamples;
    const GeoAPI& m_geoAPI;
    const ShadeAPI& m_shadeAPI;
    SearchCache& m_searchCache;
    ShadeStack& m_shadeStack;
    const size_t m_maxRayDepth;
    const size_t m_qualityLevel;
    const float m_rayPositionOffset;
};

}  // namespace shade

}  // namespace tc
#endif  // TC_SHADE
