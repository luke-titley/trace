//------------------------------------------------------------------------------
// Copywrite Luke Titley 2015
//------------------------------------------------------------------------------
#ifndef TC_LSDITERATOR
#define TC_LSDITERATOR
//------------------------------------------------------------------------------
#include "lsd/lsd.h"
#include "trace/bounds.h"
#include "trace/objectiterator.h"
#include "trace/triangleIterator.h"

namespace tc
{

//------------------------------------------------------------------------------
// lsdTriangleIterator
//------------------------------------------------------------------------------
class lsdTriangleIterator : public TriangleIterator
{
public:
    lsdTriangleIterator();

    void setPolyMesh(const lsd::PolyMesh& polymesh);

    /// \brief Prep the iterator for iteration.
    virtual void begin();
    /// \brief Notify the iterator that we have finished iterating.
    virtual void end();
    /// \brief Move onto the next triangle.
    virtual bool next();
    /// \brief Dereferencing a tc::TriangleIterator gives us a triangle.
    virtual Triangle operator*() const;

private:
    size_t m_currentTriangle;
    size_t m_nextTriangle;
    const lsd::PolyMesh* m_polymesh;
};

//------------------------------------------------------------------------------
// lsdObjectIterator
//------------------------------------------------------------------------------
class lsdObjectIterator : public ObjectIterator
{
public:
    lsdObjectIterator();

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

    /// \return Returns a TriangleIterator, for looping over all the triangles
    /// in the current object..
    virtual const BoundsF& getBounds() const;

    /// \return true if the current object has triangles. false if not.
    virtual bool hasTriangles() const;

    /// \return Returns a TriangleIterator, for looping over all the triangles
    /// in the current object..
    virtual TriangleIterator& getTriangles();

private:
    tc::BoundsF m_placeholderBounds;
    bool m_currentIsPolyMesh;

    std::string m_filename;
    lsd::Stage m_stage;
    lsd::StageIterator m_stageIterator;
    lsdTriangleIterator m_triangleIterator;
    TriangleIterator m_stubTriangleIterator;
};
}  // namespace tc
#endif  // TC_LSDITERATOR
