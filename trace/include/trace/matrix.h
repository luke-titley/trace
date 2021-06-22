//------------------------------------------------------------------------------
// Copywrite Luke Titley 2015
//------------------------------------------------------------------------------
#ifndef TC_MATRIX
#define TC_MATRIX
//------------------------------------------------------------------------------
#include "trace/vector.h"
//------------------------------------------------------------------------------
namespace tc
{

//------------------------------------------------------------------------------
// Matrix
//------------------------------------------------------------------------------
/// \brief A 4x4 matrix.
///
/// Currently this is barely used and so does not come with the usual set of
/// methods expected of a Matrix class.
//------------------------------------------------------------------------------
template <typename T>
struct Matrix
{
    /// \brief Matrix row 0, the x axis of the orientation frame.
    Vector3<T> m_x;
    /// \brief Matrix row 1, the y axis of the orientation frame.
    Vector3<T> m_y;
    /// \brief Matrix row 2, the z axis of the orientation frame.
    Vector3<T> m_z;
    /// \brief Matrix row 3, the position of the matrix.
    Vector3<T> m_w;

    /// \brief Initializes a new tc::Matrix instance.
    /// \param x Matrix row 0, the x axis of the orientation frame.
    /// \param y Matrix row 1, the y axis of the orientation frame.
    /// \param z Matrix row 2, the z axis of the orientation frame.
    /// \param w Matrix row 3, the position of the matrix.
    Matrix(const Vector3<T>& x, const Vector3<T>& y, const Vector3<T>& z,
           const Vector3<T>& w = Vector3<T>(0.0f, 0.0f, 0.0f, 1.0f))
        : m_x(x), m_y(y), m_z(z), m_w(w)
    {
    }
};
}  // namespace tc
#endif  // TC_MATRIX
