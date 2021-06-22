//------------------------------------------------------------------------------
// Copywrite Luke Titley 2015
//------------------------------------------------------------------------------
#include "trace/log.h"
#include "trace/test.h"
#include "trace/intersect.h"
#include "trace/kdtree.h"
//------------------------------------------------------------------------------

namespace
{

//------------------------------------------------------------------------------
// PrimitiveTest
//------------------------------------------------------------------------------
class PrimitiveTest : public tc::KDTree_PrimitiveIntersect
{
public:
    typedef std::vector<tc::Vector3<float> > Points;
    const Points& m_points;
    const float m_rad;

    PrimitiveTest(const Points& points, const float rad)
        : m_points(points), m_rad(rad)
    {
    }

    bool intersect(float& resultDelta, const tc::Ray& ray,
                   const size_t primitiveId) const
    {
        return tc::intersect_sphere(resultDelta, ray, m_points[primitiveId],
                                    m_rad);
    }
};

//------------------------------------------------------------------------------
void twoSpheres(const tc::LogContext& logContext)
{
    /// [test_kdtree two spheres]

    tc::KDTree kdTree;

    // Point radius
    const float rad = 0.1f;

    // Points
    PrimitiveTest::Points points;
    const size_t p0 = points.size();
    points.push_back(tc::Vector3<float>(0.0f, 1.0f, 1.0f));
    const size_t p1 = points.size();
    points.push_back(tc::Vector3<float>(0.0f, 1.0f, -1.0f));

    // Bounds
    for (size_t i = 0; i != points.size(); ++i)
    {
        const tc::Vector3<float>& p = points[i];
        kdTree.addEntry(tc::BoundsF(p - rad, p + rad), i);
    }

    kdTree.sortTree();

    tc::KDTree_SearchCache searchCache;

    // Find two entries inside the kdTree.
    const tc::Ray ray0(tc::Vector3<float>(0.0f, 0.0f, 1.0f),
                       tc::Vector3<float>(0.0f, 1.0f, -2.0f));

    const tc::KDTree_TraceResult traceResult0 =
        kdTree.findEntries(searchCache, ray0, PrimitiveTest(points, rad));

    TC_IS(logContext, traceResult0.m_distanceAlongRay == 0.9f);
    TC_IS(logContext, traceResult0.m_elementIndex == p1);
    TC_IS(logContext, traceResult0.m_elementIndex != p0);

    const tc::Ray ray1(tc::Vector3<float>(0.0f, 0.0f, -1.0f),
                       tc::Vector3<float>(0.0f, 1.0f, 2.0f));

    const tc::KDTree_TraceResult traceResult1 =
        kdTree.findEntries(searchCache, ray1, PrimitiveTest(points, rad));

    TC_IS(logContext, traceResult1.m_distanceAlongRay == 0.9f);
    TC_IS(logContext, traceResult1.m_elementIndex == p0);
    TC_IS(logContext, traceResult1.m_elementIndex != p1);

    /// [test_kdtree two spheres]
}

#if 0
//------------------------------------------------------------------------------
void eightSpheres(const tc::LogContext& logContext)
{
    /// [test_kdtree eight spheres]

    tc::KDTree kdTree;

    // Point radius
    const float rad = 0.1f;

    // Points
    PrimitiveTest::Points points;
    const size_t p0 = points.size();
    points.push_back(tc::Vector3<float>(0.0f, 1.0f, 2.0f));
    const size_t p1 = points.size();
    points.push_back(tc::Vector3<float>(0.0f, 1.0f, 1.0f));

    // Bounds
    for (size_t i = 0; i != points.size(); ++i)
    {
        const tc::Vector3<float>& p = points[i];
        kdTree.addEntry(tc::BoundsF(p - rad, p + rad), i);
    }

    kdTree.sortTree();

    tc::KDTree_SearchCache searchCache;

    // Find two entries inside the kdTree.
    const tc::Ray ray0(tc::Vector3<float>(0.0f, 0.0f, 1.0f),
                       tc::Vector3<float>(0.0f, 1.0f, 0.0f));

    const tc::KDTree_TraceResult traceResult0 =
        kdTree.findEntries(searchCache, ray0, PrimitiveTest(points, rad));

    TC_IS(logContext, traceResult0.m_distanceAlongRay == 0.9f);
    TC_IS(logContext, traceResult0.m_elementIndex == p1);
    TC_IS(logContext, traceResult0.m_elementIndex != p0);

    const tc::Ray ray1(tc::Vector3<float>(0.0f, -1.0f, 0.0f),
                       tc::Vector3<float>(0.0f, 2.0f, 2.0f));

    const tc::KDTree_TraceResult traceResult1 =
        kdTree.findEntries(searchCache, ray1, PrimitiveTest(points, rad));

    TC_IS(logContext, traceResult1.m_distanceAlongRay == 0.9f);
    TC_IS(logContext, traceResult1.m_elementIndex == p0);
    TC_IS(logContext, traceResult1.m_elementIndex != p1);

    /// [test_kdtree eight spheres]
}
#endif

}  // namespace

//------------------------------------------------------------------------------
void tc::kdtreeRunUnitTests(const tc::LogContext& logContext)
{
    twoSpheres(logContext);
}
