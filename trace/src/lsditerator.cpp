//------------------------------------------------------------------------------
// Copywrite 2015 Luke Titley
//------------------------------------------------------------------------------
#include "trace/lsditerator.h"
//------------------------------------------------------------------------------
#include "lsd/lsdParser.h"
#include "trace/assert.h"
#include "trace/vector.h"
#include <cstring>

namespace tc
{

//------------------------------------------------------------------------------
// lsdTriangleIterator
//------------------------------------------------------------------------------
lsdTriangleIterator::lsdTriangleIterator()
    : m_currentTriangle(0), m_nextTriangle(0), m_polymesh(0)
{
}

//------------------------------------------------------------------------------
void lsdTriangleIterator::setPolyMesh(const lsd::PolyMesh& polymesh)
{
    m_polymesh = &polymesh;
    m_currentTriangle = 0;
    m_nextTriangle = 0;
    assert(polymesh.m_triangles != 0);
}

//------------------------------------------------------------------------------
void lsdTriangleIterator::begin()
{
}

//------------------------------------------------------------------------------
void lsdTriangleIterator::end()
{
}

//------------------------------------------------------------------------------
bool lsdTriangleIterator::next()
{
    assert(m_polymesh != 0);
    assert(m_polymesh->m_triangles != 0);
    if (m_nextTriangle == m_polymesh->m_triangles->size())
    {
        return false;
    }
    m_currentTriangle = m_nextTriangle;
    ++m_nextTriangle;
    return true;
}

//------------------------------------------------------------------------------
Triangle lsdTriangleIterator::operator*() const
{
    lsd::PolyMesh::Verticies& verticies = *(m_polymesh->m_verticies);
    lsd::PolyMesh::Triangles& triangles = *(m_polymesh->m_triangles);
    lsd::Triangle& lsdTriangle = triangles[m_currentTriangle];
    assert(lsdTriangle.a < verticies.size());
    assert(lsdTriangle.b < verticies.size());
    assert(lsdTriangle.c < verticies.size());
    lsd::Vertex& lsd_a = verticies[lsdTriangle.a];
    lsd::Vertex& lsd_b = verticies[lsdTriangle.b];
    lsd::Vertex& lsd_c = verticies[lsdTriangle.c];
    Vector3<float> a(lsd_a.x, lsd_a.y, lsd_a.z);
    Vector3<float> b(lsd_b.x, lsd_b.y, lsd_b.z);
    Vector3<float> c(lsd_c.x, lsd_c.y, lsd_c.z);
    const Triangle triangle(a, b, c);
    return triangle;
}

//------------------------------------------------------------------------------
// lsdObjectIterator
//------------------------------------------------------------------------------
lsdObjectIterator::lsdObjectIterator()
    : m_placeholderBounds(Vector3<float>(-1.0f), Vector3<float>(1.0f)),
      m_currentIsPolyMesh(false),
      m_stageIterator(m_stage)
{
}

//------------------------------------------------------------------------------
void lsdObjectIterator::setFilename(const char* filename)
{
    m_filename = filename;
}

//------------------------------------------------------------------------------
void lsdObjectIterator::begin()
{
    // Construct a parser object and parse the given file.
    lsd::Parser parser(m_stage, m_filename.c_str());
    parser.parse();
    // Initialise the stage iterator to begin iterating.
    m_stageIterator.begin();
}

//------------------------------------------------------------------------------
void lsdObjectIterator::end()
{
}

//------------------------------------------------------------------------------
bool lsdObjectIterator::next()
{
    m_currentIsPolyMesh = false;
    const bool shouldContinue = m_stageIterator.next();
    if (shouldContinue)
    {
        // Returns a TriangleIterator for looping over the triangles in this
        // polymesh.
        const lsd::Node& node = *m_stageIterator;
        if (node.isPolyMesh())
        {
            m_currentIsPolyMesh = true;
            m_triangleIterator.setPolyMesh(*node.as<lsd::PolyMesh>());
        }
    }
    return shouldContinue;
}

//------------------------------------------------------------------------------
void lsdObjectIterator::recurseIntoChildren(bool yesNo)
{
    m_stageIterator.recurseIntoChildren(yesNo);
}

//------------------------------------------------------------------------------
const BoundsF& lsdObjectIterator::getBounds() const
{
    return m_placeholderBounds;
}

//------------------------------------------------------------------------------
bool lsdObjectIterator::hasTriangles() const
{
    const lsd::Node& node = *m_stageIterator;
    return node.isPolyMesh();
}

//------------------------------------------------------------------------------
TriangleIterator& lsdObjectIterator::getTriangles()
{
    // Returns a TriangleIterator for looping over the triangles in this
    // polymesh.
    if (m_currentIsPolyMesh)
    {
        return m_triangleIterator;
    }
    // This iterator does nothing.
    // All calls to next return 'false'.
    return m_stubTriangleIterator;
}

}  // namespace tc
