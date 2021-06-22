//------------------------------------------------------------------------------
// Copywrite Luke Titley 2015
//------------------------------------------------------------------------------
#ifndef TC_SPHERE_H
#define TC_SPHERE_H
//------------------------------------------------------------------------------
#include "trace/bounds.h"
#include "trace/vector.h"
//------------------------------------------------------------------------------
#include <cstdlib>
#include <vector>

//------------------------------------------------------------------------------
namespace tc
{

//------------------------------------------------------------------------------
// Sphere
//------------------------------------------------------------------------------
/// \brief Defines a sphere.
//------------------------------------------------------------------------------
class Sphere
{
public:
    /// \brief The position of the centre of the sphere.
    const Vector3<float> m_point;

    /// \brief The radius of the sphere.
    const float m_radius;

    /// \brief Initializes a new Sphere.
    /// \param point The center of the Sphere.
    /// \param radius The radius of the Sphere.
    Sphere(const Vector3<float>& point, const float radius)
        : m_point(point), m_radius(radius)
    {
    }

    /// \brief Initializes a new Sphere by copying a given source sphere.
    /// \param sphere The source sphere to copy.
    Sphere(const Sphere& sphere)
        : m_point(sphere.m_point), m_radius(sphere.m_radius)
    {
    }

    /// \brief Computes the bounding box of this sphere in world space
    /// coordinates.
    /// \return The bounding box of this sphere.
    BoundsF computeBoundsF() const
    {
        const Vector3<float> min = (m_point - m_radius);
        const Vector3<float> max = (m_point + m_radius);
        return BoundsF(min, max);
    }
};

}  // namespace tc
#endif  // TC_SPHERE_H
