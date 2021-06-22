//------------------------------------------------------------------------------
// Copywrite Luke Titley 2015
//------------------------------------------------------------------------------
#ifndef TC_SIMPLESCENE
#define TC_SIMPLESCENE
//------------------------------------------------------------------------------
#include "trace/bounds.h"
#include "trace/constvector.h"
#include "trace/geoAPI.h"
#include "trace/shadeAPI.h"
#include "trace/surfaceframe.h"
#include "trace/traceResult.h"
#include "trace/triangle.h"
#include "trace/triangleCache.h"
#include "trace/triangleIterator.h"
//------------------------------------------------------------------------------
#include <vector>

namespace tc
{
class ObjectIterator;
class Shader;

//------------------------------------------------------------------------------
// SimplePolyMesh
//------------------------------------------------------------------------------
/// \brief Provides a barebones polygon mesh implementation.
/// Contains:
/// -* Acceleration structure (KDTree)
/// -* Array of triangles
/// -* Array of normals, tangents and bi-tangents
//------------------------------------------------------------------------------
class SimplePolyMesh
{
public:
    /// \brief Exists for the same purpose as tc::TraceResult only does not
    /// need a geometryIndex. The tc::SimplePolyMesh::TraceResult contains an
    /// index representing the element in the SimplePolyMesh that has been hit
    /// along with how far along the ray the element was hit.
    class TraceResult
    {
    public:
        /// \brief How far along the ray the element was hit.
        const float m_distanceAlongRay;
        /// \brief The element that has been hit.
        const size_t m_elementIndex;

        TraceResult(const float distanceAlongRay, const size_t elementIndex)
            : m_distanceAlongRay(distanceAlongRay), m_elementIndex(elementIndex)
        {
        }
    };

    SimplePolyMesh();

    /// \brief Initialise the contents of the poly mesh with triangle
    /// information.
    void init(TriangleIterator& triangleIterator);

    /// \brief Perform a ray cast into the poly mesh.
    TraceResult geo_trace(SearchCache& searchCache, const Ray& ray) const;

    /// \return The normal. tangent and bi-tangent vectors for the element
    /// specified by 'elementIndex'.
    const SurfaceFrame& shade_getSurfaceFrame(const size_t elementIndex) const;

private:
    /// \brief Initialise the contents of the poly mesh with triangle
    /// information.
    void addTriangle(const Triangle& triangle, const size_t index);

    typedef ConstVector<SurfaceFrame> SurfaceFrames;

    TriangleCache m_triangleCache;
    Triangles m_triangles;
    SurfaceFrames m_surfaceFrames;
    BoundsBuilderF m_boundsBuilder;
};

//------------------------------------------------------------------------------
// SimpleScene
//------------------------------------------------------------------------------
/// \brief A three dimensional scene containing only triangles defined in world
/// space.
///
/// This is a very simple scene, which contains only triangles and returns the
/// same shader for every item of geometry.
///
/// Implements the GeoAPI and ShadeAPI and provides fast implementations of:
/// - geo_trace
/// - shade_getSurfaceFrame
/// - shade_getSurfaceShader
//------------------------------------------------------------------------------
class SimpleScene : public GeoAPI, public ShadeAPI
{
public:
    /// \brief Initializes a tc::SimpleScene.
    /// \param triangleIterator A concrete tc::TriangleIterator implementation
    /// which provides the triangles to store in this scene.
    SimpleScene(ObjectIterator& objectIterator);

    /// \brief A fast implementation of tc::GeoAPI::geo_trace.
    ///
    /// \param searchCache A KDTree::SearchCache, allows for re-use of dynamic
    /// memory allocated and used during the search process.
    /// \param ray The ray to test for intersections against. If many pieces of
    /// geometry intersect with the scene then the nearest one is stored in
    /// tc::TraceResult.
    /// \return The return value is a TraceResult object which identifies the
    /// object in the scene that has been hit and the sub object inside the
    /// object, that has been hit. For example, polygon 2  + triangle 23. Or
    /// pointcloud + point 64, or NURBS object + patch 5. The TraceResult also
    /// contains 'distanceAlongRay' which is the distance along the given ray
    /// that intersects the geometry object + sub object.
    virtual TraceResult geo_trace(SearchCache& searchCache,
                                  const Ray& ray) const;

    /// \brief A fast implementation of tc::ShadeAPI::shade_getSurfaceFrame.
    ///
    /// \param geoID A reference to the item of geoemetry being rendered. geoID
    /// contains a unique number for the object being rendered (ie polymesh) and
    /// a unique number for the sub-object being rendered (ie triangle).
    /// \return A tc::SurfaceFrame instance for the given item of geoemtry.
    /// The tc::SurfaceFrame contains three axis; normal, tangent and
    /// bi-tangent and is used for creating a hemisphere around a given point.
    virtual const SurfaceFrame& shade_getSurfaceFrame(const GeoID& geoID) const;

    /// \brief A fast implementation of tc::ShadeAPI::shade_getSurfaceShader.
    ///
    /// \param geoID A reference to the item of geoemetry being rendered. geoID
    /// contains a unique number for the object being rendered (ie polymesh) and
    /// a unique number for the sub-object being rendered (ie triangle).
    /// \return A shader object. This is a concrete implementation of the
    /// tc::Shader interface, containing a 'shade' implementation.
    virtual const Shader& shade_getSurfaceShader(const GeoID& geoID) const;

private:
    typedef std::vector<SimplePolyMesh> SimplePolyMeshes;

    SimplePolyMeshes m_simplePolyMeshes;
};

}  // namespace tc
#endif  // TC_SIMPLESCENE
