//------------------------------------------------------------------------------
// Coprwrite Luke Titley 2015
//------------------------------------------------------------------------------
#ifndef TC_OBJITERATOR
#define TC_OBJITERATOR
//------------------------------------------------------------------------------
#include "trace/bounds.h"
#include "trace/objectiterator.h"
#include "trace/triangleIterator.h"
#include "trace/vector.h"
//------------------------------------------------------------------------------
#include <cstdlib>
#include <fstream>
#include <string>
#include <vector>

namespace tc
{
//------------------------------------------------------------------------------
// Obj_TriangleIterator
//------------------------------------------------------------------------------
/// \brief Given a filename, progressively reads an obj from disk.
///
/// Handles parsing the obj file. This makes it possible to easily iterate over
/// the triangles in an obj file, without loading everything into memory.
//------------------------------------------------------------------------------
class Obj_TriangleIterator : public TriangleIterator
{
public:
    Obj_TriangleIterator();
    virtual ~Obj_TriangleIterator();

    /// \brief Specifies the full path of the obj file to iterate over.
    void setFilename(const char* filename);

    /// \brief Set/Reset the iterator.
    /// \usage This must be called before using the iterator in a for loop.
    virtual void begin();
    virtual void end() {};

    /// \brief Increments the iterator to the next triangle in the obj file.
    virtual bool next();

    /// \brief Dereferences the iterator. This returns a triangle.
    virtual Triangle operator*() const;

private:
    std::string m_filename;
    std::ifstream m_infile;

    std::string m_line;
    std::vector<std::string> m_splitline;

    std::vector<Vector3<float> > m_verts;

    Vector3<float> m_a;
    Vector3<float> m_b;
    Vector3<float> m_c;
};

//------------------------------------------------------------------------------
// Obj_ObjectIterator
//------------------------------------------------------------------------------
class Obj_ObjectIterator : public ObjectIterator
{
public:
    Obj_ObjectIterator();

    /// \brief Specifies the full path of the obj file to iterate over.
    void setFilename(const char* filename);

    /// \brief Prep the iterator for iteration.
    virtual void begin();
    /// \brief Notify the iterator that we have finished iterating.
    virtual void end();
    /// \brief Move onto the next triangle.
    virtual bool next();

    /// \brief By default, we do not recurse down into child objects, this must
    /// be called each time we want to step down into the children of a node.
    virtual void recurseIntoChildren(bool yesNo);

    /// \return A TriangleIterator, for looping over all the triangles
    /// in the current object..
    virtual const BoundsF& getBounds() const;

    /// \return true if the current object has triangles. false if not.
    virtual bool hasTriangles() const;

    /// \return Returns a TriangleIterator, for looping over all the triangles
    /// in the current object..
    virtual TriangleIterator& getTriangles();

private:
    BoundsF m_bounds;
    bool m_currentDone;
    bool m_done;
    Obj_TriangleIterator m_triangleIterator;
};

}  // namespace tc
#endif  // TC_OBJITERATOR
