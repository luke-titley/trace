//------------------------------------------------------------------------------
// Copywrite Luke Titley 2015
//------------------------------------------------------------------------------
#ifndef TC_GEOAPI
#define TC_GEOAPI
//------------------------------------------------------------------------------
#include "trace/traceResult.h"
//------------------------------------------------------------------------------

namespace tc
{
class Ray;
class SearchCache;

//------------------------------------------------------------------------------
/// \brief An abstract interface for ray casting.
///
/// The implementor of this class is reponsible for managing the geometry in a
/// scene. This is a performance hot spot, and so 'geo_trace' must be
/// implemented with this in mind. When shading pixels on the image plane or
/// estimating the lighting integral geo_trace will be called to find ray
/// intersections in the scene.
///
/// Abstracting away the 'trace' operation, allows for complete decoupling of
/// the geometry representation from the shading/rendering code actual.
///
/// The rendering pipeline has limited knowledge of the geometry it is
/// rendering. Any knowledge it does have comes through the tc::GeoAPI and the
/// tc::ShadeAPI.
///
/// SimpleScene provides a concrete implementation of the GeoAPI class.
//------------------------------------------------------------------------------
class GeoAPI
{
public:
    /// Must perform a very fast lookup of the ray intersection with scene
    /// geoemtry.
    /// \param searchCache A KDTree::SearchCache, allows for re-use of dynamic
    /// memory allocated and used during the search process. This assumes use
    /// of a KDTree to organise the geometry. In the future this will become an
    /// abstract 'UserData' value, that may not refer to a KDTree search cache,
    /// \param ray The ray to test for intersections against. If many pieces of
    /// geometry intersect with the scene then the nearest one is stored in
    /// tc::TraceResult.
    /// \return The return value is a TraceResult object which identifies the
    /// object in the scene that has been hit and the sub object inside the
    /// object, that has been hit. For example, polygon 2  + triangle 23. Or
    /// pointcloud + point 64, or NURBS object + patch 5. The TraceResult also
    /// contains 'distanceAlongRay' which is the distance along the given ray
    /// that intersects the geometry object + sub object.
    virtual TraceResult geo_trace(SearchCache& searchCache,  // TODO LT: Replace
                                                             // SearchCache with
                                                             // generic
                                                             // userData.
                                  const Ray& ray) const = 0;
};

}  // namespace tc
#endif  // TC_GEOAPI
