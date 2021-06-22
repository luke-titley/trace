//------------------------------------------------------------------------------
// Copywrite Luke Titley 2015
//------------------------------------------------------------------------------
#ifndef TC_TRIANGLE
#define TC_TRIANGLE
//------------------------------------------------------------------------------
#include "trace/bounds.h"
#include "trace/constvector.h"
#include "trace/vector.h"

namespace tc
{

//------------------------------------------------------------------------------
// Triangle
//------------------------------------------------------------------------------
/// \brief Describes a triangle in three dimension space.
///
/// Currently the A, B and C veritices are stored inside the triangle.
/// In the future the triangle vertices may reference shared vertices.
//------------------------------------------------------------------------------
class Triangle
{
public:
    inline Triangle(const Vector3<float>& a, const Vector3<float>& b,
                    const Vector3<float>& c);
    inline Triangle(const Triangle& triangle);

    /// \return A Bounding Box around this triangle instance.
    inline const BoundsF computeBounds() const;

    /// \return A newly computed normal for this triangle instance. m_a, m_b and
    /// m_c are assumed to be defined clockwise.
    inline const Vector3<float> computeNormal() const;

    /// \brief Vertex A of the triangle.
    const Vector3<float> m_a;
    /// \brief Vertex B of the triangle.
    const Vector3<float> m_b;
    /// \brief Vertex C of the triangle.
    const Vector3<float> m_c;
};

//------------------------------------------------------------------------------
inline Triangle::Triangle(const Vector3<float>& a, const Vector3<float>& b,
                          const Vector3<float>& c)
    : m_a(a), m_b(b), m_c(c)
{
}

//------------------------------------------------------------------------------
inline Triangle::Triangle(const Triangle& triangle)
    : m_a(triangle.m_a), m_b(triangle.m_b), m_c(triangle.m_c)
{
}

//------------------------------------------------------------------------------
inline const BoundsF Triangle::computeBounds() const
{
    BoundsBuilderF boundsBuilder;
    boundsBuilder.expandBounds(m_a);
    boundsBuilder.expandBounds(m_b);
    boundsBuilder.expandBounds(m_c);
    return BoundsF(boundsBuilder);
}

//------------------------------------------------------------------------------
inline const Vector3<float> Triangle::computeNormal() const
{
    const Vector3<float> aToB = (m_b - m_a).normalized();
    const Vector3<float> aToC = (m_c - m_a).normalized();
    const Vector3<float> result = aToB.cross(aToC).normalized();
    return result;
}

//------------------------------------------------------------------------------
// Triangles
//------------------------------------------------------------------------------
typedef ConstVector<Triangle> Triangles;

}  // namespace tc
#endif  // TC_TRIANGLE
