//------------------------------------------------------------------------------
// Copywrite Luke Titley 2015
//------------------------------------------------------------------------------
#include "trace/simpleScene.h"
//------------------------------------------------------------------------------
#include "trace/assert.h"
#include "trace/geoid.h"
#include "trace/intersect.h"
#include "trace/objectiterator.h"
#include "trace/vector.h"
//------------------------------------------------------------------------------
#include "trace/shadersDiffuse.h"
#include "trace/shadersWhiteLight.h"
//------------------------------------------------------------------------------

namespace
{
static const tc::shaders::Diffuse shadersDiffuse;
static const tc::shaders::WhiteLight shadersWhiteLight(40.0f);
}

namespace
{
//------------------------------------------------------------------------------
// TriangleIntersect
//------------------------------------------------------------------------------
class TriangleIntersect : public tc::KDTree_PrimitiveIntersect
{
    const tc::Triangles& m_triangles;

public:
    TriangleIntersect(const tc::Triangles& triangles) : m_triangles(triangles)
    {
    }

    bool intersect(float& resultDelta, const tc::Ray& ray,
                   const size_t primitiveId) const
    {
        const tc::Triangle& triangle = m_triangles[primitiveId];
        return tc::intersect_triangle(resultDelta, ray, triangle);
    }
};
}  // namespace

namespace tc
{

//------------------------------------------------------------------------------
// SimplePolyMesh
//------------------------------------------------------------------------------
SimplePolyMesh::SimplePolyMesh()
{
}

//------------------------------------------------------------------------------
void SimplePolyMesh::addTriangle(const Triangle& newTriangle, const size_t index)
{
    // Add a new triangle
    m_triangles.push_back(newTriangle);
    const Triangle& triangle = m_triangles.back();

    // Add the triangle to the acceleration structure
    m_triangleCache.addEntry(triangle.computeBounds(), index);

    // Add the triangles normal to our normals array
    const Vector3<float> normal = triangle.computeNormal();
    Vector3<float> tangent;
    Vector3<float> bitangent;
    normal.tangentAndBitangent(tangent, bitangent);

    m_surfaceFrames.push_back(SurfaceFrame(tangent, normal, bitangent));
}

//------------------------------------------------------------------------------
void SimplePolyMesh::init(TriangleIterator& triangleIterator)
{
    triangleIterator.begin();
    for (size_t i = 0; triangleIterator.next(); ++i)
    {
        const Triangle tri = *triangleIterator;
        addTriangle(tri, i);
    }
    triangleIterator.end();
    m_triangleCache.sortTree();
}

//------------------------------------------------------------------------------
SimplePolyMesh::TraceResult SimplePolyMesh::geo_trace(SearchCache& searchCache,
                                                      const Ray& ray) const
{
    // Ray Trace Results
    const KDTree_TraceResult result = m_triangleCache.findEntries(
        searchCache, ray, TriangleIntersect(m_triangles));

    return SimplePolyMesh::TraceResult(result.m_distanceAlongRay,
                                       result.m_elementIndex);
}

//------------------------------------------------------------------------------
const SurfaceFrame& SimplePolyMesh::shade_getSurfaceFrame(
    const size_t elementIndex) const
{
    assert(elementIndex < m_surfaceFrames.size());
    const SurfaceFrame& surfaceFrame = m_surfaceFrames[elementIndex];
    return surfaceFrame;
}

//------------------------------------------------------------------------------
// SimpleScene
//------------------------------------------------------------------------------
SimpleScene::SimpleScene(ObjectIterator& objectIterator)
{
    // Build up a list of polygon meshes
    objectIterator.begin();
    while (objectIterator.next())
    {
        objectIterator.recurseIntoChildren(true);
        if (objectIterator.hasTriangles())
        {
            m_simplePolyMeshes.resize(m_simplePolyMeshes.size() + 1);
            m_simplePolyMeshes.back().init(objectIterator.getTriangles());
        }
    }
    objectIterator.end();
}

//------------------------------------------------------------------------------
TraceResult SimpleScene::geo_trace(SearchCache& searchCache,
                                   const Ray& ray) const
{
    TraceResult result(FLT_MAX, GeoID(0, 0));
    float resultDistanceAlongRay = FLT_MAX;
    size_t resultObjectIndex = 0;
    size_t resultElementIndex = 0;

    // Test against all the polygon meshes in the scene.
    // TODO LT: Replace this with accel structure.
    for (size_t i = 0; i != m_simplePolyMeshes.size(); ++i)
    {
        SimplePolyMesh::TraceResult traceResult =
            m_simplePolyMeshes[i].geo_trace(searchCache, ray);
        if (traceResult.m_distanceAlongRay < resultDistanceAlongRay)
        {
            resultDistanceAlongRay = traceResult.m_distanceAlongRay;
            resultObjectIndex = i + 1;
            resultElementIndex = traceResult.m_elementIndex;
        }
    }

    // Intersect with a light sphere around our scene.
    const float globalSphereRadius = 20.0f;
    if (intersect_sphere(resultDistanceAlongRay, ray, globalSphereRadius))
    {
        resultObjectIndex = m_simplePolyMeshes.size() + 1;
        resultElementIndex = 0;
    }

    return TraceResult(resultDistanceAlongRay,
                       GeoID(resultObjectIndex, resultElementIndex));
}

//------------------------------------------------------------------------------
const SurfaceFrame& SimpleScene::shade_getSurfaceFrame(const GeoID& geoID) const
{
    assert(geoID.m_objectIndex <= m_simplePolyMeshes.size());
    return m_simplePolyMeshes[geoID.m_objectIndex - 1]
        .shade_getSurfaceFrame(geoID.m_elementIndex);
}

//------------------------------------------------------------------------------
const Shader& SimpleScene::shade_getSurfaceShader(const GeoID& geoID) const
{
    if (geoID.m_objectIndex == (m_simplePolyMeshes.size() + 1))
    {
        return shadersWhiteLight;
    }
    return shadersDiffuse;
}

}  // namespace tc
