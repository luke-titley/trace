//------------------------------------------------------------------------------
// Copywrite Luke Titley 2015
//------------------------------------------------------------------------------
#ifndef TC_OBJECTITERATOR
#define TC_OBJECTITERATOR
//------------------------------------------------------------------------------
#include "trace/bounds.h"

namespace tc
{
class Triangle;
class TriangleIterator;

//------------------------------------------------------------------------------
// ObjectIterator
//------------------------------------------------------------------------------
/// \brief Iterates over all the 'objects' in a scene. These are the items of
/// geometry. Currently an object is a polymesh, but in the future it could be
/// any type of geometry, for example a subd mesh, or a point cloud.
/// \code
///
/// USDIterator iterator(filename);
/// iterator.begin();
/// while(iterator.next())
/// {
///     if(tc::intersect_bounds(ray, iterator.getBoundsF()))
///     {
///         iterator.recurseIntoChildren(true);
///     }
/// }
/// iterator.end();
/// \endcode
//------------------------------------------------------------------------------
class ObjectIterator
{
public:
    virtual ~ObjectIterator()
    {
    }

    /// \brief Prep the iterator for iteration.
    virtual void begin() = 0;
    /// \brief Notify the iterator that we have finished iterating.
    virtual void end() = 0;
    /// \brief Move onto the next triangle.
    virtual bool next() = 0;

    /// \brief By default, we do not recurse down into child objects, this must
    /// be called each time we want to step down into the children of a node.
    virtual void recurseIntoChildren(bool yesNo) = 0;

    /// \return A TriangleIterator, for looping over all the triangles
    /// in the current object..
    virtual const BoundsF& getBounds() const = 0;

    /// \return true if the current object has triangles. false if not.
    virtual bool hasTriangles() const = 0;

    /// \return Returns a TriangleIterator, for looping over all the triangles
    /// in the current object..
    virtual TriangleIterator& getTriangles() = 0;
};
}  // namespace tc
#endif  // TC_OBJECTITERATOR
