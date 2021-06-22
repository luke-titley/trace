//------------------------------------------------------------------------------
// Copywrite Luke Titley 2015
//------------------------------------------------------------------------------
#ifndef TC_TRACERESULT
#define TC_TRACERESULT
//------------------------------------------------------------------------------
#include "trace/geoid.h"
#include "trace/vector.h"
//------------------------------------------------------------------------------
#include <cstdlib>
#include <vector>
//------------------------------------------------------------------------------
namespace tc
{

//------------------------------------------------------------------------------
// TraceResult
//------------------------------------------------------------------------------
/// \brief Contains the results of a ray cast operation.
///
/// When a ray is cast into a scene a tc::TraceResult is returned.
/// It contains:
/// - A 'distance' value for how far along the ray the nearest intersection was
/// found.
/// - A GeoId value which identifies the item of geometry that the ray hit.
//------------------------------------------------------------------------------
class TraceResult
{
public:
    inline TraceResult();
    inline TraceResult(const TraceResult& src);
    inline TraceResult(const float distanceAlongRay, const GeoID& geoId);

    /// \brief Returns true if the result of the ray cast is an intersection,
    /// and false if nothing was hit.
    inline bool hasHitSomething() const;

    /// \brief Contains how far along the ray the nearest intersection was
    /// found.
    const float m_distanceAlongRay;

    /// \brief Identifies the item of geometry that has been hit.
    GeoID m_geoId;
};

//------------------------------------------------------------------------------
inline TraceResult::TraceResult() : m_distanceAlongRay(0.0f), m_geoId(0, 0)
{
}

//------------------------------------------------------------------------------
inline TraceResult::TraceResult(const TraceResult& src)
    : m_distanceAlongRay(src.m_distanceAlongRay), m_geoId(src.m_geoId)
{
}

//------------------------------------------------------------------------------
inline TraceResult::TraceResult(const float distanceAlongRay,
                                const GeoID& geoId)
    : m_distanceAlongRay(distanceAlongRay), m_geoId(geoId)
{
}

//------------------------------------------------------------------------------
inline bool TraceResult::hasHitSomething() const
{
    return m_geoId.m_objectIndex != 0;
}

}  // namespace tc
#endif  // TC_TRACERESULT
