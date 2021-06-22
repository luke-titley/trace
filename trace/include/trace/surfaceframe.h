//------------------------------------------------------------------------------
// Copywrite Luke Titley 2015
//------------------------------------------------------------------------------
#ifndef TC_SURFACEFRAME
#define TC_SURFACEFRAME
//------------------------------------------------------------------------------
#include "trace/vector.h"
//------------------------------------------------------------------------------

namespace tc
{

//------------------------------------------------------------------------------
// SurfaceFrame
//------------------------------------------------------------------------------
/// \brief Contains the 'Tangent', 'Normal' and 'Bi-Tangent' of a point on a
/// surface.
///
/// Useful for computing vectors on a hemisphere around a surface point.
//------------------------------------------------------------------------------
class SurfaceFrame
{
public:
    /// \brief A vector that is orthonoromal to the bi-tangent and the normal.
    const Vector3<float> m_tangent;
    /// \brief A vector which represents the direction a point on a surface is
    /// facing.
    const Vector3<float> m_normal;
    /// \brief A vector that is orthonormal to the normal and the tangent.
    const Vector3<float> m_bitangent;

    /// \brief Initialises the tc::SurfaceFrame with the given vectors.
    /// \param tangent A vector that is orthonormal to the bi-tangent and the
    ///     normal.
    /// \param normal A vector which represents the direction a point on a
    ///     surface is facing.
    /// \param bitangent A vector that is orthonormal to the normal and the
    ///     tangent.
    SurfaceFrame(const Vector3<float>& tangent, const Vector3<float>& normal,
                 const Vector3<float>& bitangent)
        : m_tangent(tangent), m_normal(normal), m_bitangent(bitangent)
    {
    }
};

}  // namespace tc
#endif  // TC_SURFACEFRAME
