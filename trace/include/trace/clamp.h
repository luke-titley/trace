//------------------------------------------------------------------------------
// Copywrite Luke Titley 2015
//------------------------------------------------------------------------------
// A collection of utilities for keeping a number inside of a range.
//------------------------------------------------------------------------------
#ifndef TC_CLAMP
#define TC_CLAMP
//------------------------------------------------------------------------------
#include "trace/vector.h"
//------------------------------------------------------------------------------
namespace tc
{

//------------------------------------------------------------------------------
// clamp
//------------------------------------------------------------------------------
/// \brief Given a value 'a', returns a value guaranteed to be less than max
/// and greater than min. If the value of 'a' is already less than max and
/// greater than min, then returns 'a' as is.
/// \param min The smallest number permitted.
/// \param max The greatest number permitted.
/// \return A value in the range min to max.
//------------------------------------------------------------------------------
template <typename T>
T clamp(T min, T a, T max)
{
    a = a > min ? a : min;
    a = a < max ? a : max;
    return a;
}

//------------------------------------------------------------------------------
/// \brief A specialisation of clamp for Vector3<float>.
//------------------------------------------------------------------------------
Vector3<float> clamp(const Vector3<float> min, const Vector3<float> a,
                     const Vector3<float> max)
{
    const Vector3<float> result(clamp(min.x, a.x, max.x),
                                clamp(min.y, a.y, max.y),
                                clamp(min.z, a.z, max.z));
    return result;
}

//------------------------------------------------------------------------------
/// \param a: A number to compare against b.
/// \param b: A number to compare against a.
/// \return The smallest of the two parameters 'a' and 'b'.
//------------------------------------------------------------------------------
template <typename T>
T min(T a, T b)
{
    T result = a < b ? a : b;
    return result;
}

//------------------------------------------------------------------------------
/// \param a: A number to compare against b.
/// \param b: A number to compare against a.
/// \return The greatest of the two parameters 'a' and 'b'.
//------------------------------------------------------------------------------
template <typename T>
T max(T a, T b)
{
    T result = a > b ? a : b;
    return result;
}

}  // namespace tc

#endif  // TC_CLAMP
