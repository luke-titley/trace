//------------------------------------------------------------------------------
// Copywrite Luke Titley 2015
//------------------------------------------------------------------------------
#ifndef TC_TRIANGLEITERATOR
#define TC_TRIANGLEITERATOR
//------------------------------------------------------------------------------
#include "trace/triangle.h"
//------------------------------------------------------------------------------

namespace tc
{
//------------------------------------------------------------------------------
// Triangleterator
//------------------------------------------------------------------------------
/// \brief This is an interface for looping over a collection of triangles.
///  This abstraction allows for a tc::SimpleScene to be populated from any
/// source of geometry that implements this interface.
///
/// A default implementation is provided which does nothing. This can be used
/// for returning a TriangleIterator for objects that do not contain triangles.
///
/// It decouples the geometry source from the geometry consumer.
/// \code
/// CustomTriangleIterator customTriangleIterator;
/// customTriangleIterator.begin();
/// while(customTriangleIterator.next())
/// {
///     const Triangle triangle = *customTriangleIterator;
///     // Do something with the triangle.
/// }
/// customTriangleIterator.end();
/// \endcode
//------------------------------------------------------------------------------
class TriangleIterator
{
public:
    virtual ~TriangleIterator()
    {
    }

    /// \brief Prep the iterator for iteration.
    virtual void begin()
    {
    }
    /// \brief Notify the iterator that we have finished iterating.
    virtual void end()
    {
    }
    /// \brief Move onto the next triangle.
    virtual bool next()
    {
        return false;
    }
    /// \brief Dereferencing a tc::TriangleIterator gives us a triangle.
    virtual Triangle operator*() const
    {
        return Triangle(Vector3<float>(0.0f), Vector3<float>(0.0f),
                        Vector3<float>(0.0f));
    }
};
}  // namespace tc
#endif  // TC_TRIANGLEITERATOR
