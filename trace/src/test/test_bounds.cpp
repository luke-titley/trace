//------------------------------------------------------------------------------
// Copywrite Luke Titley 2015
//------------------------------------------------------------------------------
#include "trace/log.h"
#include "trace/test.h"
#include "trace/bounds.h"
//------------------------------------------------------------------------------

namespace
{

//------------------------------------------------------------------------------
void constructors(const tc::LogContext& logContext)
{
    /// [test_bounds constructors]
    // Setup
    const float epsilon = 0.001f;
    const tc::Vector3<float> pMax(1.0f, 1.0f, 1.0f);
    const tc::Vector3<float> pMin(-1.0f, -1.0f, -1.0f);
    tc::BoundsBuilderF boundsBuilder;
    boundsBuilder.expandBounds(pMax, epsilon);
    boundsBuilder.expandBounds(pMin, epsilon);
    // /Setup

    {
        const tc::BoundsF bounds(boundsBuilder);
        TC_IS(logContext, bounds.m_max == (pMax + epsilon).overwrite(
                                              tc::Vector3<float>::kW, 0.0f));
        TC_IS(logContext, bounds.m_min == (pMin - epsilon).overwrite(
                                              tc::Vector3<float>::kW, 0.0f));
    }

    {
        const tc::BoundsF bounds(pMin, pMax);
        TC_IS(logContext, bounds.m_max == pMax);
        TC_IS(logContext, bounds.m_min == pMin);
    }
    /// [test_bounds constructors]
}

//------------------------------------------------------------------------------
void contains(const tc::LogContext& logContext)
{
    /// [test_bounds contains]
    // Setup
    const tc::Vector3<float> pMax(2.0f, 2.0f, 2.0f);
    const tc::Vector3<float> pMin(-2.0f, -2.0f, -2.0f);
    const tc::BoundsF bounds(pMin, pMax);
    // /Setup

    TC_IS(logContext, bounds.contains(tc::Vector3<float>(0.0f, 0.0f, 0.0f)));
    TC_IS(logContext, bounds.contains(tc::BoundsF(tc::Vector3<float>(-1.0f),
                                                  tc::Vector3<float>(1.0f))));
    /// [test_bounds contains]
}

//------------------------------------------------------------------------------
void equals(const tc::LogContext& logContext)
{
    /// [test_bounds equals]
    // Setup
    const tc::Vector3<float> pMax(2.0f, 2.0f, 2.0f);
    const tc::Vector3<float> pMin(-2.0f, -2.0f, -2.0f);
    const tc::BoundsF boundsA(pMin, pMax);
    const tc::BoundsF boundsB(pMin, pMax);
    // /Setup

    TC_IS(logContext, boundsA.equals(boundsB));

    /// [test_bounds equals]
}

//------------------------------------------------------------------------------
void computeSurfaceArea(const tc::LogContext& logContext)
{
    /// [test_bounds equals]
    // Setup
    const tc::Vector3<float> pMax(1.0f, 2.0f, 3.0f);
    const tc::Vector3<float> pMin(0.0f, 0.0f, 0.0f);
    const tc::BoundsF bounds(pMin, pMax);
    // /Setup

    TC_IS(logContext, bounds.computeSurfaceArea() == 22.0f);

    /// [test_bounds equals]
}

//------------------------------------------------------------------------------
void split(const tc::LogContext& logContext)
{
    /// [test_bounds split]

    tc::BoundsF bounds(tc::Vector3<float>(-1.0f, -1.0f, -1.0f),
                       tc::Vector3<float>(1.0f, 1.0f, 1.0f));

    tc::Pair<tc::BoundsF> splitA = bounds.split(0, 0.0f);

    TC_IS(logContext,
          splitA.m_left.m_min == tc::Vector3<float>(-1.0f, -1.0f, -1.0f));
    TC_IS(logContext,
          splitA.m_left.m_max == tc::Vector3<float>(0.0f, 1.0f, 1.0f));
    TC_IS(logContext,
          splitA.m_right.m_min == tc::Vector3<float>(0.0f, -1.0f, -1.0f));
    TC_IS(logContext,
          splitA.m_right.m_max == tc::Vector3<float>(1.0f, 1.0f, 1.0f));
    /// [test_bounds split]
}

//------------------------------------------------------------------------------
void operator_std_string(const tc::LogContext& logContext)
{
    tc::BoundsF bounds(tc::Vector3<float>(-1.0f, -1.0f, -1.0f),
                       tc::Vector3<float>(1.0f, 1.0f, 1.0f));
    TC_IS(logContext, std::string(bounds) == "(-1 -1 -1 0),(1 1 1 0)");
}

}  // namespace

//------------------------------------------------------------------------------
void tc::boundsRunUnitTests(const tc::LogContext& logContext)
{
    constructors(logContext);
    contains(logContext);
    equals(logContext);
    computeSurfaceArea(logContext);
    split(logContext);
    operator_std_string(logContext);
}
