//------------------------------------------------------------------------------
// Copywrite Luke Titley 2015
//------------------------------------------------------------------------------
#include "trace/log.h"
#include "trace/test.h"
#include "trace/vector.h"
//------------------------------------------------------------------------------

namespace
{
//------------------------------------------------------------------------------
void constructor_XYWW(const tc::LogContext& logContext)
{
    /// [test_vector constructor_XYWW]
    {
        const tc::Vector3<float> t(1.0f, 2.0f);
        TC_IS(logContext,
              t.x == 1.0f && t.y == 2.0f && t.z == 0.0f && t.w == 0.0f);
    }
    {
        const tc::Vector3<float> t(1.0f, 2.0f, 3.0f);
        TC_IS(logContext,
              t.x == 1.0f && t.y == 2.0f && t.z == 3.0f && t.w == 0.0f);
    }
    {
        const tc::Vector3<float> t(1.0f, 2.0f, 3.0f, 4.0f);
        TC_IS(logContext,
              t.x == 1.0f && t.y == 2.0f && t.z == 3.0f && t.w == 4.0f);
    }
    /// [test_vector constructor_XYWW]
}

//------------------------------------------------------------------------------
void constructor_All_One_Value(const tc::LogContext& logContext)
{
    /// [test_vector constructor_All_One_Value]

    // '9.0f' becomes the default for all values.
    const tc::Vector3<float> t(9.0f);
    TC_IS(logContext, t.x == 9.0f && t.y == 9.0f && t.z == 9.0f && t.w == 9.0f);

    /// [test_vector constructor_All_One_Value]
}

//------------------------------------------------------------------------------
void comparison_equals(const tc::LogContext& logContext)
{
    /// [test_vector comparison_equals]
    {
        const tc::Vector3<float> a(1.0f, 2.0f, 3.0f, 4.0f);
        const tc::Vector3<float> b(1.0f, 2.0f, 3.0f, 4.0f);
        const tc::Vector3<float> c(0.0f);
        TC_IS(logContext, a == b);
        TC_IS(logContext, a != c);
    }
    {
        const tc::Vector3<float> a(9.0f);
        TC_IS(logContext, a == 9.0f);
        TC_IS(logContext, a != 0.0f);
    }
    /// [test_vector comparison_equals]
}

//------------------------------------------------------------------------------
void comparison_lessThanGreaterThan(const tc::LogContext& logContext)
{
    /// [test_vector comparison_lessThanGreaterThan]
    const tc::Vector3<float> large(255.0f);
    const tc::Vector3<float> mid(128.0f);
    const tc::Vector3<float> small(0.0f);

    TC_IS(logContext, mid < large);
    TC_IS(logContext, mid > small);
    TC_IS(logContext, mid >= tc::Vector3<float>(128.0f));
    TC_IS(logContext, mid <= tc::Vector3<float>(128.0f));

    /// [test_vector comparison_lessThanGreaterThan]
}

//------------------------------------------------------------------------------
void arithmetic(const tc::LogContext& logContext)
{
    /// [test_vector arithmetic]
    {
        tc::Vector3<float> a(2.0f);
        const tc::Vector3<float> one(1.0f);
        const tc::Vector3<float> three(3.0f);
        a += one;
        TC_IS(logContext, a == three);
    }
    {
        tc::Vector3<float> a(3.0f);
        const tc::Vector3<float> one(1.0f);
        const tc::Vector3<float> two(2.0f);
        a -= one;
        TC_IS(logContext, a == two);
    }
    {
        tc::Vector3<float> a(2.0f);
        const tc::Vector3<float> three(3.0f);
        const tc::Vector3<float> six(6.0f);
        a *= three;
        TC_IS(logContext, a == six);
    }
    {
        tc::Vector3<float> a(12.0f);
        const tc::Vector3<float> two(2.0f);
        const tc::Vector3<float> six(6.0f);
        a /= two;
        TC_IS(logContext, a == six);
    }
    /// [test_vector arithmetic]
}

//------------------------------------------------------------------------------
void arithmetic_newResult(const tc::LogContext& logContext)
{
    /// [test_vector arithmetic_newResult]
    {
        const tc::Vector3<float> two(2.0f);
        const tc::Vector3<float> one(1.0f);
        const tc::Vector3<float> three(3.0f);
        const tc::Vector3<float> result = two + one;
        TC_IS(logContext, result == three);
    }
    {
        const tc::Vector3<float> three(3.0f);
        const tc::Vector3<float> one(1.0f);
        const tc::Vector3<float> two(2.0f);
        const tc::Vector3<float> result = three - one;
        TC_IS(logContext, result == two);
    }
    {
        const tc::Vector3<float> three(3.0f);
        const tc::Vector3<float> six(6.0f);
        const tc::Vector3<float> two(2.0f);
        const tc::Vector3<float> result = two * three;
        TC_IS(logContext, result == six);
    }
    {
        const tc::Vector3<float> twelve(12.0f);
        const tc::Vector3<float> two(2.0f);
        const tc::Vector3<float> six(6.0f);
        const tc::Vector3<float> result = twelve / two;
        TC_IS(logContext, result == six);
    }
    /// [test_vector arithmetic_newResult]
}

//------------------------------------------------------------------------------
void indices(const tc::LogContext& logContext)
{
    /// [test_vector indices]
    {
        const tc::Vector3<float> a(1.0f, 2.0f, 3.0f, 4.0f);
        TC_IS(logContext, a[0] == 1.0f);
        TC_IS(logContext, a[1] == 2.0f);
        TC_IS(logContext, a[2] == 3.0f);
        TC_IS(logContext, a[3] == 4.0f);
    }
    {
        const tc::Vector3<float> a(1.0f, 2.0f, 3.0f, 4.0f);
        TC_IS(logContext, a[-1] == 4.0f);
        TC_IS(logContext, a[-2] == 3.0f);
        TC_IS(logContext, a[-3] == 2.0f);
        TC_IS(logContext, a[-4] == 1.0f);
    }
    {
        const tc::Vector3<float> a(1.0f, 2.0f, 3.0f, 4.0f);
        TC_IS(logContext, a[4] == 1.0f);
        TC_IS(logContext, a[5] == 2.0f);
        TC_IS(logContext, a[6] == 3.0f);
        TC_IS(logContext, a[7] == 4.0f);
    }
    /// [test_vector indices]
}

//------------------------------------------------------------------------------
void dot(const tc::LogContext& logContext)
{
    /// [test_vector dot]
    // Normal
    {
        const tc::Vector3<float> a(1.0f, 2.0f, 3.0f, 4.0f);
        const tc::Vector3<float> two(2.0f);
        const float result = a.dot(two);
        TC_IS(logContext, result == 20.0f);
    }

    // Negative
    {
        const tc::Vector3<float> a(1.0f, 0.0f, 0.0f);
        const tc::Vector3<float> b =
            tc::Vector3<float>(-0.5f, 0.5f, 0.0f).normalized();
        const tc::Vector3<float> c =
            tc::Vector3<float>(0.5f, 0.5f, 0.0f).normalized();

        const float adotBTest = (a.dot(b) + 0.7f);
        const float adotCTest = (a.dot(c) - 0.7f);

        TC_IS(logContext, (adotBTest > -0.01f) && (adotBTest < 0.01f));
        TC_IS(logContext, (adotCTest > -0.01f) && (adotCTest < 0.01f));
    }
    /// [test_vector dot]
}

//------------------------------------------------------------------------------
void cross(const tc::LogContext& logContext)
{
    /// [test_vector cross]
    const tc::Vector3<float> x(1.0f, 0.0f, 0.0f, 0.0f);
    const tc::Vector3<float> y(0.0f, 1.0f, 0.0f, 0.0f);
    const tc::Vector3<float> z(0.0f, 0.0f, 1.0f, 0.0f);
    const tc::Vector3<float> result = x.cross(y);
    TC_IS(logContext, result == z);
    /// [test_vector cross]
}

//------------------------------------------------------------------------------
void scalarTriple(const tc::LogContext& logContext)
{
    const tc::Vector3<float> x(3.0f, 0.0f, 0.0f);
    const tc::Vector3<float> y(0.0f, 3.0f, 0.0f);
    const tc::Vector3<float> z(0.0f, 0.0f, 3.0f);
    TC_IS(logContext, x.scalarTriple(y, z) == 27.0f);
}

//------------------------------------------------------------------------------
void overwrite(const tc::LogContext& logContext)
{
    /// [test_vector overwrite]
    const tc::Vector3<float> opaque(255.0f);
    const tc::Vector3<float> transparent =
        opaque.overwrite(tc::Vector3<float>::kA, 0.0f);
    TC_IS(logContext,
          transparent == tc::Vector3<float>(255.0, 255.0f, 255.0, 0.0f));
    /// [test_vector cross]
}

//------------------------------------------------------------------------------
void equals(const tc::LogContext& logContext)
{
    /// [test_vector equals]
    const tc::Vector3<float> a(0.0f, 1.0f, 2.0f, 3.0f);
    TC_IS(logContext,
          a.equals(tc::Vector3<float>(0.0005, 1.0005, 2.0005, 3.0005)));
    TC_IS(logContext,
          !a.equals(tc::Vector3<float>(0.0015, 1.0015, 2.0015, 3.0015)));
    /// [test_vector equals]
}

//------------------------------------------------------------------------------
void lerp(const tc::LogContext& logContext)
{
    /// [test_vector lerp]
    const tc::Vector3<float> one(1.0f, 0.0f, 0.0f, 0.0f);
    const tc::Vector3<float> three(3.0f, 0.0f, 0.0f, 0.0f);
    const tc::Vector3<float> two(2.0f, 0.0f, 0.0f, 0.0f);
    const tc::Vector3<float> result = one.lerp(three, 0.5f);
    TC_IS(logContext, result == two);
    /// [test_vector lerp]
}

//------------------------------------------------------------------------------
void tangentAndBitangent(const tc::LogContext& logContext)
{
    /// [test_vector tangentAndBitangent]
    const tc::Vector3<float> x(1.0f, 0.0f, 0.0f);
    tc::Vector3<float> tangent;
    tc::Vector3<float> biTangent;

    x.tangentAndBitangent(tangent, biTangent);
    TC_IS(logContext, tangent == tc::Vector3<float>(0.0f, 0.0f, 1.0f));
    TC_IS(logContext, biTangent == tc::Vector3<float>(0.0f, 1.0f, 0.0f));
    /// [test_vector tangentAndBitangent]
}

//------------------------------------------------------------------------------
void transform(const tc::LogContext& logContext)
{
    /// [test_vector transform]
    const tc::Vector3<float> x(1.0f, 0.0f, 0.0f);
    /// Rotate around the z axis by 90 degrees
    const tc::Vector3<float> i(0.0f, -1.0f, 0.0f);
    const tc::Vector3<float> j(1.0f, 0.0f, 0.0f);
    const tc::Vector3<float> k(0.0f, 0.0f, 1.0f);
    const tc::Vector3<float> result = x.transform(i, j, k);
    TC_IS(logContext, result == tc::Vector3<float>(0.0f, -1.0f, 0.0f));
    /// [test_vector transform]
}

//------------------------------------------------------------------------------
void magSq(const tc::LogContext& logContext)
{
    /// [test_vector magSq]
    const tc::Vector3<float> a(3.0, 4.0f, 5.0f);
    TC_IS(logContext, a.magSq() == 50.0f);
    /// [test_vector magSq]
}

//------------------------------------------------------------------------------
void mag(const tc::LogContext& logContext)
{
    /// [test_vector mag]
    const tc::Vector3<float> a(3.0, 4.0f, 5.0f);
    TC_IS(logContext, tc::Vector3<float>(7.071).equals(a.mag()));
    /// [test_vector mag]
}

//------------------------------------------------------------------------------
void normalized(const tc::LogContext& logContext)
{
    /// [test_vector normalized]
    const tc::Vector3<float> two(2.0f, 0.0f, 0.0f, 0.0f);
    const tc::Vector3<float> one(1.0f, 0.0f, 0.0f, 0.0f);
    TC_IS(logContext, two.normalized() == one);
    /// [test_vector normalized]
}

//------------------------------------------------------------------------------
void inverse(const tc::LogContext& logContext)
{
    /// [test_vector inverse]
    const tc::Vector3<float> one(1.0f, 0.0f, 0.0f, 0.0f);
    const tc::Vector3<float> minus_one(-1.0f, 0.0f, 0.0f, 0.0f);
    TC_IS(logContext, one.inverse() == minus_one);
    /// [test_vector inverse]
}

//------------------------------------------------------------------------------
void volume(const tc::LogContext& logContext)
{
    /// [test_vector volume]
    const tc::Vector3<float> a(2.0f, 2.0f, 2.0f);
    TC_IS(logContext, a.volume() == 8.0f);
    /// [test_vector volume]
}

//------------------------------------------------------------------------------
void area(const tc::LogContext& logContext)
{
    /// [test_vector area]
    const tc::Vector3<float> a(2.0f, 2.0f);
    TC_IS(logContext, a.area() == 4.0f);
    /// [test_vector area]
}

#if 0
//------------------------------------------------------------------------------
void surfaceArea(const tc::LogContext& logContext)
{
    /// [test_vector surfaceArea]
    const tc::Vector3<float> a(2.0f, 2.0f, 2.0f);
    TC_IS(logContext, a.surfaceArea() == 24.0f);
    /// [test_vector surfaceArea]
}
#endif

//------------------------------------------------------------------------------
void map(const tc::LogContext& logContext)
{
    /// [test_vector map]
    const tc::Vector3<float> a(25.0f);
    const tc::Vector3<float> result = a.map(sqrt);
    TC_IS(logContext, result == tc::Vector3<float>(5.0f));
    /// [test_vector map]
}

//------------------------------------------------------------------------------
void cast(const tc::LogContext& logContext)
{
    /// [test_vector cast]
    const tc::Vector3<float> aFloat(2.0f);
    const tc::Vector3<int> aInt = aFloat.cast<int>();
    TC_IS(logContext, aInt == tc::Vector3<int>(2));
    /// [test_vector cast]
}

//------------------------------------------------------------------------------
void operator_std_string(const tc::LogContext& logContext)
{
    /// [test_vector operator_std_string]
    const tc::Vector3<size_t> a(255);
    std::string result = static_cast<std::string>(a);
    TC_IS(logContext, result == std::string("255 255 255 255"));
    /// [test_vector operator_std_string]
}

}  // namespace

//------------------------------------------------------------------------------
void tc::vectorRunUnitTests(const tc::LogContext& logContext)
{
    constructor_XYWW(logContext);
    constructor_All_One_Value(logContext);
    comparison_equals(logContext);
    comparison_lessThanGreaterThan(logContext);
    arithmetic(logContext);
    arithmetic_newResult(logContext);
    indices(logContext);
    dot(logContext);
    cross(logContext);
    overwrite(logContext);
    equals(logContext);
    scalarTriple(logContext);
    lerp(logContext);
    tangentAndBitangent(logContext);
    transform(logContext);
    magSq(logContext);
    mag(logContext);
    normalized(logContext);
    inverse(logContext);
    volume(logContext);
    area(logContext);
    map(logContext);
    cast(logContext);
    operator_std_string(logContext);
}
