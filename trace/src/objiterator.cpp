//------------------------------------------------------------------------------
// Copywrite 2015 Luke Titley
//------------------------------------------------------------------------------
#include "trace/objiterator.h"
//------------------------------------------------------------------------------
#include "pystring.h"
//------------------------------------------------------------------------------

namespace tc
{

//------------------------------------------------------------------------------
// Obj_TriangleIterator
//------------------------------------------------------------------------------
Obj_TriangleIterator::Obj_TriangleIterator()
{
}

//------------------------------------------------------------------------------
Obj_TriangleIterator::~Obj_TriangleIterator()
{
}

//------------------------------------------------------------------------------
void Obj_TriangleIterator::setFilename(const char* filename)
{
    m_filename = filename;
}

//------------------------------------------------------------------------------
void Obj_TriangleIterator::begin()
{
    m_infile.open(m_filename.c_str());

    while (std::getline(m_infile, m_line))
    {
        // Load an m_verts
        pystring::split(m_line, m_splitline);
        if (m_splitline.size() == 4 && m_splitline[0] == "v")
        {
            const float x = atof(m_splitline[1].c_str());
            const float y = atof(m_splitline[2].c_str());
            const float z = atof(m_splitline[3].c_str());

            m_verts.push_back(Vector3<float>(x, y, z));
        }
    }

    m_infile.clear();
    m_infile.seekg(0);
}

//------------------------------------------------------------------------------
bool Obj_TriangleIterator::next()
{
    while (std::getline(m_infile, m_line))
    {
        // Load the m_verts
        pystring::split(m_line, m_splitline);
        if (m_splitline.size() == 4 && m_splitline[0] == "f")
        {
            size_t va = atoi(m_splitline[1].c_str());
            size_t vb = atoi(m_splitline[2].c_str());
            size_t vc = atoi(m_splitline[3].c_str());

            m_a = m_verts[va - 1];
            m_b = m_verts[vb - 1];
            m_c = m_verts[vc - 1];

            return true;
        }
    }
    return false;
}

//------------------------------------------------------------------------------
Triangle Obj_TriangleIterator::operator*() const
{
    return Triangle(m_a, m_b, m_c);
}

//------------------------------------------------------------------------------
// Obj_ObjectIterator
//------------------------------------------------------------------------------
Obj_ObjectIterator::Obj_ObjectIterator()
    : m_bounds(Vector3<float>(0.0f), Vector3<float>(0.0f)),
      m_currentDone(false),
      m_done(false)
{
}

//------------------------------------------------------------------------------
void Obj_ObjectIterator::setFilename(const char* filename)
{
    m_triangleIterator.setFilename(filename);
}

//------------------------------------------------------------------------------
void Obj_ObjectIterator::begin()
{
}

//------------------------------------------------------------------------------
void Obj_ObjectIterator::end()
{
}

//------------------------------------------------------------------------------
bool Obj_ObjectIterator::next()
{
    m_currentDone = m_done;
    m_done = true;
    return !m_currentDone;
}

//------------------------------------------------------------------------------
void Obj_ObjectIterator::recurseIntoChildren(bool yesNo)
{
    // Stub
}

//------------------------------------------------------------------------------
const BoundsF& Obj_ObjectIterator::getBounds() const
{
    return m_bounds;
}

//------------------------------------------------------------------------------
bool Obj_ObjectIterator::hasTriangles() const
{
    return !m_currentDone;
}

//------------------------------------------------------------------------------
TriangleIterator& Obj_ObjectIterator::getTriangles()
{
    return m_triangleIterator;
}

}  // namespace tc
