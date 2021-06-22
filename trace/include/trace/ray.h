//------------------------------------------------------------------------------
// Copywrite Luke Titley 2015
//------------------------------------------------------------------------------
#ifndef TC_RAY
#define TC_RAY

//------------------------------------------------------------------------------
#include "trace/vector.h"
//------------------------------------------------------------------------------

namespace tc
{

//------------------------------------------------------------------------------
// Ray
//------------------------------------------------------------------------------
/// \brief Stores a three dimensional position and direction.
///
/// As a utility, tc::Ray will also compute the position of a point, given a
/// distance ('t') along the ray.
//------------------------------------------------------------------------------
class Ray
{
public:
    /// \brief The direction the ray is pointing in.
    const Vector3<float> m_direction;

    /// \brief The starting position of the ray.
    const Vector3<float> m_position;

    Ray(): m_direction(0.0f), m_position(0.0f)
    {
    }

    /// \brief Initializes a ray with the given direction and position.
    /// \param direction The direction the ray is pointing in.
    /// \param position The starting position of the ray.
    Ray(const Vector3<float>& direction, const Vector3<float>& position)
        : m_direction(direction), m_position(position)
    {
    }

    /// Computes the position of a point, given a distance ('t') along the ray.
    /// \param t Specifies where along the ray the point should be computed.
    /// \return A point on the ray.
    Vector3<float> computePointOnRay(const float t) const
    {
        Vector3<float> result = m_position + (m_direction * t);
        return result;
    }
};

}  // namespace tc
#endif  // TC_RAY
