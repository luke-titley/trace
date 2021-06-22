//------------------------------------------------------------------------------
// Copywrite Luke Titley 2015
//------------------------------------------------------------------------------
#include "trace/intersect.h"
//------------------------------------------------------------------------------
#include "trace/bounds.h"
#include "trace/ray.h"
#include "trace/triangle.h"
//------------------------------------------------------------------------------
#include <cmath>
#include <cfloat>
//------------------------------------------------------------------------------

namespace tc
{

namespace
{
//------------------------------------------------------------------------------
template <typename T>
T sq(T a)
{
    return a * a;
}
}

//------------------------------------------------------------------------------
// intersect_sphere
//------------------------------------------------------------------------------
bool intersect_sphere(float& resultDelta, const Ray& ray,
                      const float sphereRadius)
{
    const float root = sq(ray.m_direction.dot(ray.m_position)) -
                       ray.m_position.dot(ray.m_position) + sq(sphereRadius);
    if (root < 0)
    {
        return false;
    }

    float d_a = -ray.m_direction.dot(ray.m_position) + sqrt(root);
    float d_b = -ray.m_direction.dot(ray.m_position) - sqrt(root);

    // Intersections behind the ray starting position
    // don't count.
    //
    d_a = d_a < 0.0f ? FLT_MAX : d_a;
    d_b = d_b < 0.0f ? FLT_MAX : d_b;

    const float d = d_a < d_b ? d_a : d_b;
    if (d < 0 || d > resultDelta)
    {
        return false;
    }

    resultDelta = d;

    return true;
}

//------------------------------------------------------------------------------
// intersect_sphere
//------------------------------------------------------------------------------
bool intersect_sphere(float& resultDelta, const Ray& ray,
                      const Vector3<float>& spherePosition,
                      const float sphereRadius)
{
    const Vector3<float> center = spherePosition - ray.m_position;
    const float r = sphereRadius;
    const float root = sq(ray.m_direction.dot(center.inverse())) -
                       center.inverse().dot(center.inverse()) + sq(r);
    if (root < 0)
    {
        return false;
    }

    float d_a = -ray.m_direction.dot(center.inverse()) + sqrt(root);
    float d_b = -ray.m_direction.dot(center.inverse()) - sqrt(root);

    // Intersections behind the camera
    // don't count.
    //
    d_a = d_a < 0.0f ? FLT_MAX : d_a;
    d_b = d_b < 0.0f ? FLT_MAX : d_b;
    float d = d_a < d_b ? d_a : d_b;

    // Which result is the closest ?
    //
    if (d < 0 || d > resultDelta)
    {
        return false;
    }

    resultDelta = d;

    return true;
}

//------------------------------------------------------------------------------
// intersect_plane
//------------------------------------------------------------------------------
bool intersect_plane(float& resultDelta, const Ray& ray, const size_t planeAxis,
                     const float planePosition)
{
    Vector3<float> plane(0.0f);
    plane[planeAxis] = planePosition;

    Vector3<float> plane_normal(0.0f);
    plane_normal[planeAxis] = -planePosition;
    plane_normal = plane_normal.normalized();

    const float bottom = ray.m_direction.dot(plane_normal);

    // If the ray is parrelel to the plane.
    // They'll be no intersection
    if (bottom == 0.0f)
    {
        return false;
    }
    const float top = (plane - ray.m_position).dot(plane_normal);
    const float d = top / bottom;

    // If we're behind the ray
    // or something closer
    // has already intersected
    // then fail to intersect.
    if (d < 0.0f || d > resultDelta)
    {
        return false;
    }

    // Fill in the information
    resultDelta = d;

    return true;
}

//------------------------------------------------------------------------------
// intersect_triangle
//------------------------------------------------------------------------------
bool intersect_triangle(float& resultDelta, const Ray& ray, const Triangle& tri)
{
    const Vector3<float> pa = tri.m_a - ray.m_position;
    const Vector3<float> pb = tri.m_b - ray.m_position;
    const Vector3<float> pc = tri.m_c - ray.m_position;

    // Test if ray.m_direction is inside the edges bc, ca and ab. Done by
    // testing
    // that the signed tetrahedral volumes, computed using scalar triple
    // products, are all positive
    float u = ray.m_direction.scalarTriple(pc, pb);
    if (u < 0.0f)
    {
        return false;
    }
    float v = ray.m_direction.scalarTriple(pa, pc);
    if (v < 0.0f)
    {
        return false;
    }
    float w = ray.m_direction.scalarTriple(pb, pa);
    if (w < 0.0f)
    {
        return false;
    }

    // Compute the barycentric coordinates (u, v, w) determining the
    // intersection point r, r = u*a + v*b + w*c
    const float denom = 1.0f / (u + v + w);
    u *= denom;
    v *= denom;
    w *= denom;

    // Work out the intersection position
    const Vector3<float> p = (tri.m_a * u) + (tri.m_b * v) + (tri.m_c * w);
    const float t =
        (p - ray.m_position).dot(ray.m_direction);  // Project p onto the ray
                                                    // direction to get the
                                                    // distance along the ray

    assert(!isnan(t));

    if (t < 0.0f || t > resultDelta)
    {
        return false;
    }

    // Fill in the information
    resultDelta = t;

    return true;
}

//------------------------------------------------------------------------------
// intersect_bounds
//------------------------------------------------------------------------------
bool intersect_bounds(const Ray& ray, const BoundsF& bounds,
                      const float epsilon)
{
    float tmin = 0.0f;     // The closest
    float tmax = FLT_MAX;  // set to max distance ray can travel (for segment)

    // For all three slabs
    for (int i = 0; i < 3; i++)
    {
        if (fabs(ray.m_direction[i]) < epsilon)
        {
            // Ray is parallel to slab. No hit if origin not within slab
            if (ray.m_position[i] < bounds.m_min[i] ||
                ray.m_position[i] > bounds.m_max[i])
            {
                return 0;
            }
        }
        else
        {
            // Compute intersection t value of ray with near and far plane of
            // slab
            const float ood = 1.0f / ray.m_direction[i];
            float t1 = (bounds.m_min[i] - ray.m_position[i]) * ood;
            float t2 = (bounds.m_max[i] - ray.m_position[i]) * ood;

            // Make t1 be intersection with near plane, t2 with far plane
            if (t1 > t2)
            {
                const float tmp = t1;
                t1 = t2;
                t2 = tmp;
            }
            // Compute the intersection of slab intersection intervals
            if (t1 > tmin)
            {
                tmin = t1;
            }
            if (t2 < tmax)
            {
                tmax = t2;
            }
            // Exit with no collision as soon as slab intersection becomes empty
            if (tmin > tmax)
            {
                return false;
            }
        }
    }

    return true;
}

}  // namespace tc
