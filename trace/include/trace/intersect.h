//------------------------------------------------------------------------------
// Copywrite Luke Titley 2015
//------------------------------------------------------------------------------
/// \file intersect.h
/// Provides a collection of ray to primitive intersection routines.
//------------------------------------------------------------------------------
#ifndef TC_INTERSECT
#define TC_INTERSECT
//------------------------------------------------------------------------------
#include "trace/bounds.h"
#include "trace/vector.h"

namespace tc
{
class Ray;
class Triangle;

//------------------------------------------------------------------------------
// intersect_sphere
//------------------------------------------------------------------------------
/// \brief Computes the intersection of a ray against a sphere with the given
/// radius and positioned at the origin.
///
/// \param resultDelta[out]: A reference to a float. If an intersection is found
/// that is closer along the ray than the value currently stored in resultData,
/// then the new intersection distance along the ray will be stored in
/// resultDelta. If the position of the intersection point is further along the
/// ray than the value contained in resultDelta, then the intersection will fail
/// and the contents of resultDelta will be untouched.
///
/// \param ray A ray to intersect with a sphere.
/// \param sphereRadius The radius of the sphere to intersect. The sphere is
/// positioned at the origin.
///
/// \return true if the ray intersects a sphere positioned at the origin with
/// the given radius and the distance of the intersection point is smaller than
/// resultDelta.
///
/// <b>Example</b>
/// \snippet test_intersect.cpp test_intersect sphereAtOrigin
//------------------------------------------------------------------------------
bool intersect_sphere(float& resultDelta, const Ray& ray,
                      const float sphereRadius);

//------------------------------------------------------------------------------
// intersect_sphere
//------------------------------------------------------------------------------
/// \brief Computes the intersection of a ray against a sphere with the given
/// radius and with the given position.
///
/// \param resultDelta[out]: A reference to a float. If an intersection is found
/// that is closer along the ray than the value currently stored in resultData,
/// then the new intersection distance along the ray will be stored in
/// resultDelta. If the position of the intersection point is further along the
/// ray than the value contained in resultDelta, then the intersection will fail
/// and the contents of resultDelta will be untouched.
///
/// \param ray A ray to intersect with a sphere.
/// \param sphereRadius The radius of the sphere to intersect.
/// \param spherePosition The position of the sphere to intersect.
///
/// \return true if the ray intersects a sphere at the given position with the
/// given radius and the distance of the intersection point is smaller than
/// resultDelta. Otherwise return false.
///
/// <b>Example</b>
/// \snippet test_intersect.cpp test_intersect sphereWithPosition
//------------------------------------------------------------------------------
bool intersect_sphere(float& resultDelta, const Ray& ray,
                      const Vector3<float>& spherePosition,
                      const float sphereRadius = 0.75f);

//------------------------------------------------------------------------------
// intersect_plane
//------------------------------------------------------------------------------
/// \brief Computes the intersection of a ray against a plane.
///
/// \param resultDelta[out]: A reference to a float. If an intersection is found
/// that is closer along the ray than the value currently stored in resultData,
/// then the new intersection distance along the ray will be stored in
/// resultDelta. If the position of the intersection point is further along the
/// ray than the value contained in resultDelta, then the intersection will fail
/// and the contents of resultDelta will be untouched.
///
/// \param ray A ray to intersect with a plane.
/// \param planeAxis The axis that the plane is positioned on X=0, Y=1, Z=2.
/// \param planePosition The position of the plane on the given axis.
///
/// \return true if the ray intersects a plane with the given position on the
/// given axis and the distance of the intersection point is smaller than
/// resultDelta. Otherwise return false.
///
/// <b>Example</b>
/// \snippet test_intersect.cpp test_intersect plane
//------------------------------------------------------------------------------
bool intersect_plane(float& resultDelta, const Ray& ray, const size_t planeAxis,
                     const float planePosition);

//------------------------------------------------------------------------------
// intersect_triangle
//------------------------------------------------------------------------------
/// \brief Computes the intersection of a ray against a triangle.
///
/// \param resultDelta[out]: A reference to a float. If an intersection is found
/// that is closer along the ray than the value currently stored in resultData,
/// then the new intersection distance along the ray will be stored in
/// resultDelta. If the position of the intersection point is further along the
/// ray than the value contained in resultDelta, then the intersection will fail
/// and the contents of resultDelta will be untouched.
///
/// \param ray A ray to intersect with a triangle.
/// \param tri The triangle to intersect with a ray.
///
/// \return true if the ray intersects the given triangle and the distance of
/// the intersection point is smaller than resultDelta. Otherwise return false.
///
/// <b>Example</b>
/// \snippet test_intersect.cpp test_intersect triangle
//------------------------------------------------------------------------------
bool intersect_triangle(float& resultDelta, const Ray& ray,
                        const Triangle& tri);

//------------------------------------------------------------------------------
// intersect_bounds
//------------------------------------------------------------------------------
/// \brief Computes whether the given ray intersects the given bounding box.
/// Does not compute an intersection position, or a distance along the ray.
///
/// \param ray A ray to intersect with a triangle.
/// \param bounds A bounding box to intersect with the given ray.
///
/// \return true if the given ray intersects the given bounding box, false if
/// not.
///
/// <b>Example</b>
/// \snippet test_intersect.cpp test_intersect boundingBox
//------------------------------------------------------------------------------
bool intersect_bounds(const Ray& ray, const BoundsF& bounds,
                      const float epsilon = 0.001f);

//------------------------------------------------------------------------------
// Runs all the unit tests for the 'constvector' header file.
/// \cond
void intersectRunUnitTests(const tc::LogContext& logContext);
/// \endcond

}  // namespace tc
#endif  // TC_INTERSECT
