//------------------------------------------------------------------------------
// Copywrite Luke Titley 2015
//------------------------------------------------------------------------------
#include "trace/bounds.h"
#include "trace/intersect.h"
#include "trace/log.h"
#include "trace/ray.h"
#include "trace/test.h"
#include "trace/triangle.h"
//------------------------------------------------------------------------------
#include <cfloat>

namespace
{

//------------------------------------------------------------------------------
void sphereAtOrigin(const tc::LogContext& logContext)
{
    /// [test_intersect sphereAtOrigin]
    float resultDelta = FLT_MAX;
    const tc::Ray ray(tc::Vector3<float>(0.0f, 0.0f, 1.0f),
                      tc::Vector3<float>(0.0f, 0.0f, -2.0f));

    TC_IS(logContext, tc::intersect_sphere(resultDelta, ray, 1.0f) == true);
    TC_IS(logContext, resultDelta == 1.0f);
    /// [test_intersect sphereAtOrigin]
}

//------------------------------------------------------------------------------
void sphereWithPosition(const tc::LogContext& logContext)
{
    /// [test_intersect sphereWithPosition]
    float resultDelta = FLT_MAX;
    const tc::Ray ray(tc::Vector3<float>(0.0f, 0.0f, 1.0f),
                      tc::Vector3<float>(0.0f, 0.0f, -3.0f));
    tc::Vector3<float> spherePosition(0.0f, 0.0f, -1.0f);

    TC_IS(logContext,
          tc::intersect_sphere(resultDelta, ray, spherePosition, 1.0f) == true);
    TC_IS(logContext, resultDelta == 1.0f);
    /// [test_intersect sphereWithPosition]
}

//------------------------------------------------------------------------------
void plane(const tc::LogContext& logContext)
{
    /// [test_intersect plane]
    enum
    {
        X = 0,
        Y = 1,
        Z = 2
    };

    // X Axis
    {
        float resultDelta = FLT_MAX;
        const tc::Ray ray(tc::Vector3<float>(1.0f, 0.0f, 0.0f),
                          tc::Vector3<float>(0.0f, 0.0f, 0.0f));
        TC_IS(logContext,
              tc::intersect_plane(resultDelta, ray, X, 1.0f) == true);
        TC_IS(logContext, resultDelta == 1.0f);
    }
    // Y Axis
    {
        float resultDelta = FLT_MAX;
        const tc::Ray ray(tc::Vector3<float>(0.0f, 1.0f, 0.0f),
                          tc::Vector3<float>(0.0f, 0.0f, 0.0f));
        TC_IS(logContext,
              tc::intersect_plane(resultDelta, ray, Y, 1.0f) == true);
        TC_IS(logContext, resultDelta == 1.0f);
    }
    // Z Axis
    {
        float resultDelta = FLT_MAX;
        const tc::Ray ray(tc::Vector3<float>(0.0f, 0.0f, 1.0f),
                          tc::Vector3<float>(0.0f, 0.0f, 0.0f));
        TC_IS(logContext,
              tc::intersect_plane(resultDelta, ray, Z, 1.0f) == true);
        TC_IS(logContext, resultDelta == 1.0f);
    }
    /// [test_intersect plane]
}

//------------------------------------------------------------------------------
void triangle(const tc::LogContext& logContext)
{
    /// [test_intersect triangle]
    float resultDelta = FLT_MAX;
    const tc::Ray ray(tc::Vector3<float>(0.0f, 0.0f, 1.0f),
                      tc::Vector3<float>(0.0f, 0.0f, -1.0f));
    const tc::Vector3<float> a(-1.0f, -1.0f, 0.0f);
    const tc::Vector3<float> b(0.0f, 1.0f, 0.0f);
    const tc::Vector3<float> c(1.0f, -1.0f, 0.0f);
    tc::Triangle triangle(a, b, c);

    TC_IS(logContext,
          tc::intersect_triangle(resultDelta, ray, triangle) == true);
    TC_IS(logContext, resultDelta == 1.0f);
    /// [test_intersect triangle]
}

//------------------------------------------------------------------------------
void boundingBox(const tc::LogContext& logContext)
{
    /// [test_intersect boundingBox]
    const tc::Ray ray0(tc::Vector3<float>(0.0f, 0.0f, 1.0f),
                      tc::Vector3<float>(0.0f, 0.0f, -2.0f));
    const tc::Ray ray1(tc::Vector3<float>(0.0f, 0.0f,-1.0f),
                      tc::Vector3<float>(0.0f, 0.0f, -2.0f));
    const tc::Vector3<float> max(1.0f, 1.0f, 1.0f);
    const tc::Vector3<float> min(-1.0f, -1.0f, -1.0f);
    const tc::BoundsF bounds(min, max);
    TC_IS(logContext, tc::intersect_bounds(ray0, bounds) == true);
    TC_IS(logContext, tc::intersect_bounds(ray1, bounds) == false);
    /// [test_intersect boundingBox]
}

}  // namespace

//------------------------------------------------------------------------------
void tc::intersectRunUnitTests(const tc::LogContext& logContext)
{
    sphereAtOrigin(logContext);
    sphereWithPosition(logContext);
    plane(logContext);
    triangle(logContext);
    boundingBox(logContext);
}
