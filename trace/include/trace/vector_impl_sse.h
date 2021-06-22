//------------------------------------------------------------------------------
// Copywrite Luke Titley 2015
//------------------------------------------------------------------------------
// SSE includes
#if 1
#if 0
    #include <mmintrin.h>  // MMX
    #include <xmmintrin.h> // SSE1
    #include <emmintrin.h> // SSE2
    #include <pmmintrin.h> // SSE3
    #include <smmintrin.h> // SSE4
#endif

    #include "trace/int.h"
    #include <immintrin.h> // SSE intrinsics, all supported versions
#endif

namespace tc
{

//------------------------------------------------------------------------------
inline
__m128 Vector3_float_4_cross(__m128 a, __m128 b)
{
    // Everything works in reverse with sse.
    // z, y, x, w
    // Including shuffling.
    const uint32_t YZX = _MM_SHUFFLE(0, 0, 2, 1);
    const uint32_t ZXY = _MM_SHUFFLE(0, 1, 0, 2);

    __m128 tmp0 = _mm_shuffle_ps(a, a, YZX);
    __m128 tmp1 = _mm_shuffle_ps(b, b, ZXY);
    __m128 sum_lhs = _mm_mul_ps(tmp0,tmp1);

    tmp0 = _mm_shuffle_ps(a, a, ZXY);
    tmp1 = _mm_shuffle_ps(b, b, YZX);

    __m128 sum_rhs = _mm_mul_ps(tmp0,tmp1);
    __m128 c = _mm_sub_ps(sum_lhs,sum_rhs);

    return c;
}

//------------------------------------------------------------------------------
inline
__m128 Vector3_float_4_dot(__m128 a, __m128 b)
{
    // There's a bug in gcc 4.6.3 which stops us from being able to use
    // _mm_dp_sp. The third parameter 'mask' of _mm_dp_sp must be a constexpr
    // but gcc 4.6.3 is unable to guarantee a const expression where it would
    // normally be expected. We we have to do things a bit more manually.
    __m128 c = _mm_mul_ps(a,b); // Multiply
    c = _mm_hadd_ps(c,c); // Sum
    c = _mm_hadd_ps(c,c); // Sum
    return c;
}

#if 0

//------------------------------------------------------------------------------
template <typename T, size_t D>
Vector3<T, D>::Vector3(T px, T py, T pz, T pw)
    : x(px), y(py), z(pz), w(pw)
{
}

//------------------------------------------------------------------------------
template <typename T, size_t D>
Vector3<T, D>::Vector3(T default_value)
{
    for (size_t i = 0; i != D; ++i)
    {
        value[i] = default_value;
    }
}

//------------------------------------------------------------------------------
template <typename T, size_t D>
bool Vector3<T, D>::operator==(const Vector3& rhs) const
{
    for (size_t i = 0; i != D; ++i)
    {
        if (value[i] != rhs.value[i])
        {
            return false;
        }
    }
    return true;
}

//------------------------------------------------------------------------------
template <typename T, size_t D>
bool Vector3<T, D>::operator!=(const Vector3& rhs) const
{
    return !operator==(rhs);
}

//------------------------------------------------------------------------------
template <typename T, size_t D>
bool Vector3<T, D>::operator<(const Vector3& rhs) const
{
    return x < rhs.x && y < rhs.y && z < rhs.z;
}

//------------------------------------------------------------------------------
template <typename T, size_t D>
bool Vector3<T, D>::operator>(const Vector3& rhs) const
{
    return x > rhs.x && y > rhs.y && z > rhs.z;
}

//------------------------------------------------------------------------------
template <typename T, size_t D>
bool Vector3<T, D>::operator<=(const Vector3& rhs) const
{
    return x <= rhs.x && y <= rhs.y && z <= rhs.z;
}

//------------------------------------------------------------------------------
template <typename T, size_t D>
bool Vector3<T, D>::operator>=(const Vector3& rhs) const
{
    return x >= rhs.x && y >= rhs.y && z >= rhs.z;
}
#endif

//------------------------------------------------------------------------------
template<>
inline
Vector3<float, 4> Vector3<float, 4>::operator+(const Vector3<float, 4>& rhs) const
{
    //assert((void*)(this->value) % 16 == 0);
    Vector3<float, 4> result = *this;

    __m128* a = (__m128*)this->value;
    __m128* b = (__m128*)rhs.value;
    __m128* c = (__m128*)result.value;

    *c = _mm_add_ps(*a, *b);

    return result;
}

//------------------------------------------------------------------------------
template<>
inline
Vector3<float, 4> Vector3<float, 4>::operator-(const Vector3<float, 4>& rhs) const
{
    Vector3<float, 4> result = *this;

    __m128* a = (__m128*)this->value;
    __m128* b = (__m128*)rhs.value;
    __m128* c = (__m128*)result.value;

    *c = _mm_sub_ps(*a, *b);

    return result;
}

//------------------------------------------------------------------------------
template<>
inline
Vector3<float, 4> Vector3<float, 4>::operator*(const Vector3<float, 4>& rhs) const
{
    Vector3<float, 4> result = *this;

    __m128* a = (__m128*)this->value;
    __m128* b = (__m128*)rhs.value;
    __m128* c = (__m128*)result.value;

    *c = _mm_mul_ps(*a, *b);

    return result;
}

//------------------------------------------------------------------------------
template<>
inline
Vector3<float, 4> Vector3<float, 4>::operator/(const Vector3<float, 4>& rhs) const
{
    Vector3<float, 4> result = *this;

    __m128* a = (__m128*)this->value;
    __m128* b = (__m128*)rhs.value;
    __m128* c = (__m128*)result.value;

    *c = _mm_div_ps(*a, *b);

    return result;
}

//------------------------------------------------------------------------------
template<>
inline
Vector3<float, 4> & Vector3<float, 4>::operator+=(const Vector3<float, 4>& rhs)
{
    *this = *this + rhs;
    return *this;
}

//------------------------------------------------------------------------------
template<>
inline
Vector3<float, 4> & Vector3<float, 4>::operator-=(const Vector3<float, 4>& rhs)
{
    *this = *this - rhs;
    return *this;
}

//------------------------------------------------------------------------------
template<>
inline
Vector3<float, 4> & Vector3<float, 4>::operator*=(const Vector3<float, 4>& rhs)
{
    *this = *this * rhs;
    return *this;
}

//------------------------------------------------------------------------------
template<>
inline
Vector3<float, 4> & Vector3<float, 4>::operator/=(const Vector3<float, 4>& rhs)
{
    *this = *this / rhs;
    return *this;
}

#if 0
//------------------------------------------------------------------------------
template <typename T, size_t D>
T& Vector3<T, D>::operator[](size_t index)
{
    return value[index % D];  // Wrap around
}

//------------------------------------------------------------------------------
template <typename T, size_t D>
const T& Vector3<T, D>::operator[](size_t index) const
{
    return value[index % D];  // Wrap around
}

//------------------------------------------------------------------------------
template <typename T, size_t D>
Vector3<T, D> Vector3<T, D>::overwrite(size_t index, T value) const
{
    Vector3 result = *this;
    result[index % D] = value;  // Wrap around
    return result;
}
#endif

//------------------------------------------------------------------------------
template<>
inline
float Vector3<float, 4>::dot(const Vector3<float,4>& rhs) const
{
    __m128 a = _mm_load_ps(this->value);
    __m128 b = _mm_load_ps(rhs.value);
    __m128 c = Vector3_float_4_dot(a,b);

    float result(0.0f);
    _mm_store_ss(&result, c);
    return result;
}

//------------------------------------------------------------------------------
template<>
inline
Vector3<float, 4> Vector3<float, 4>::cross(const Vector3<float, 4>& rhs) const
{
    __m128 a = _mm_load_ps(this->value);
    __m128 b = _mm_load_ps(rhs.value);
    __m128 c = Vector3_float_4_cross(a,b);

    Vector3<float, 4> result(0.0f);
    _mm_store_ps(result.value, c);
    return result;
}

//------------------------------------------------------------------------------
template<>
inline
float Vector3<float, 4>::scalarTriple(const Vector3<float,4> & b,
                                      const Vector3<float,4> & c) const
{
    __m128 a_ps = _mm_load_ps(this->value);
    __m128 b_ps = _mm_load_ps(b.value);
    __m128 c_ps = _mm_load_ps(c.value);

    __m128 result_ps =
            Vector3_float_4_dot(a_ps, Vector3_float_4_cross(b_ps,c_ps));

    float result(0.0f);
    _mm_store_ss(&result, result_ps);
    return result;
}

#if 0
//------------------------------------------------------------------------------
template <typename T, size_t D>
bool Vector3<T, D>::equals(const Vector3& rhs, const T epsilon) const
{
    for (size_t i = 0; i != D; ++i)
    {
        if (fabs(value[i] - rhs.value[i]) > epsilon)
        {
            return false;
        }
    }
    return true;
}

//------------------------------------------------------------------------------
template <typename T, size_t D>
Vector3<T, D> Vector3<T, D>::lerp(const Vector3& rhs, T t, const T one) const
{
    Vector3<T> result = ((*this) * Vector3(one - t)) + (rhs * Vector3(t));
    return result;
}

//------------------------------------------------------------------------------
template <typename T, size_t D>
void Vector3<T, D>::tangentAndBitangent(Vector3& resultTangent,
                                        Vector3& resultBitangent, const T zero,
                                        const T one) const
{
    Vector3 up(zero, one, zero);
    T vector_dot_up = this->dot(up);
    if (vector_dot_up == one)
    {
        up = Vector3(one, zero, zero);
    }
    else if (vector_dot_up == -one)
    {
        up = Vector3(-one, zero, zero);
    }

    resultTangent = this->cross(up).normalized();
    resultBitangent = resultTangent.cross(*this);
}

//------------------------------------------------------------------------------
template <typename T, size_t D>
Vector3<T, D> Vector3<T, D>::transform(const Vector3& i, const Vector3& j,
                                       const Vector3& k) const
{
    const Vector3 result(((i * x) + (j * y) + (k * z)).normalized());
    return result;
}

//------------------------------------------------------------------------------
template <typename T, size_t D>
T Vector3<T, D>::magSq() const
{
    T result = dot(*this);
    return result;
}

//------------------------------------------------------------------------------
template <typename T, size_t D>
T Vector3<T, D>::mag(const T zero) const
{
    T lengthSq = magSq();
    if (lengthSq == zero)
    {
        return zero;
    }
    T result = sqrt(magSq());
    return result;
}

//------------------------------------------------------------------------------
template <typename T, size_t D>
Vector3<T, D> Vector3<T, D>::normalized() const
{
    Vector3<T> result = *this;
    T mag = sqrt(result.magSq());
    for (size_t i = 0; i != D; ++i)
    {
        result.value[i] /= mag;
    }
    return result;
}

//------------------------------------------------------------------------------
template <typename T, size_t D>
Vector3<T, D> Vector3<T, D>::inverse(const T one) const
{
    Vector3<T> result = *this;
    for (size_t i = 0; i != D; ++i)
    {
        result.value[i] *= -one;
    }
    return result;
}

//------------------------------------------------------------------------------
template <typename T, size_t D>
T Vector3<T, D>::volume() const
{
    return x * y * z;
}

//------------------------------------------------------------------------------
template <typename T, size_t D>
T Vector3<T, D>::area() const
{
    return Vector3(x, y, 1).volume();
}

//------------------------------------------------------------------------------
template <typename T, size_t D>
T Vector3<T, D>::surfaceArea() const
{
    const T areaX = depth * height;
    const T areaY = width * depth;
    const T areaZ = width * height;
    const T halfSurfaceArea = areaX + areaY + areaZ;
    const T surfaceArea = halfSurfaceArea * 2.0f;
    return surfaceArea;
}

//------------------------------------------------------------------------------
template <typename T, size_t D>
template <typename M>
Vector3<T, D> Vector3<T, D>::map(M t) const
{
    Vector3 result;
    for (size_t i = 0; i != D; ++i)
    {
        result.value[i] = t(value[i]);
    }
    return result;
}

//------------------------------------------------------------------------------
template <typename T, size_t D>
template <typename M>
Vector3<M, D> Vector3<T, D>::cast() const
{
    Vector3<M, D> result;
    for (size_t i = 0; i != D; ++i)
    {
        result.value[i] = static_cast<T>(value[i]);
    }
    return result;
}

//------------------------------------------------------------------------------
template <typename T, size_t D>
Vector3<T, D>::operator std::string() const
{
    std::stringstream ss;
    ss << x;
    for (size_t i = 1; i < D; ++i)
    {
        ss << " " << value[i];
    }
    return ss.str();
}
#endif

}  // namespace tc
