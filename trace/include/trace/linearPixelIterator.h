//------------------------------------------------------------------------------
// Copywrite Luke Titley 2015
//------------------------------------------------------------------------------
#ifndef TC_LINEAR_PIXELITERATOR
#define TC_LINEAR_PIXELITERATOR
//------------------------------------------------------------------------------
#include "trace/pixelIterator.h"
//------------------------------------------------------------------------------
#include <vector>
//------------------------------------------------------------------------------
namespace tc
{

//------------------------------------------------------------------------------
// LinearPixelIterator
//------------------------------------------------------------------------------
/// \brief This iterator can be used to loop over each pixel in the grid
/// specified by width and height. Given a threadCount and threadIndex this
/// iterator will loop over a fragment of the grid. This allows us to divide the
/// image plane up into blocks of pixels to render for each thread. The
/// LinearPixelIterator moves from left to right, bottom to top.
///
/// getAffectedPixels is not useful in this case, the range it returns is just
/// around the pixel returned by getPixelLocation.
///
/// Example:
/// \code
///	LinearPixelIterator it;
///
///	For two threads.
///
///	Thread 0, the top half of the grid
///	it.begin(256, 256, 2, 0);
///	while(it.next())
///	{
///		const Vector3<float> i = it.getPixelLocation()
///	}
///
///	Thread 1, the bottom half of the grid
///	it.begin(256, 256, 2, 1);
///	while(it.next())
///	{
///		const Vector3<float> i = it.getPixelLocation()
///	}
/// \endcode
//------------------------------------------------------------------------------
class LinearPixelIterator : public PixelIterator
{
    size_t m_currentI;
    size_t m_i;
    size_t m_upperLimit;
    size_t m_width;
    bool m_hasBegun;

public:
    LinearPixelIterator();

    /// \brief Reset/Set the iterator. Must be called before using the iterator
    /// in a for loop.
    void begin(const size_t width, const size_t height,
               const size_t threadCount, const size_t threadIndex);

    /// \brief Increments the interator.
    virtual bool next();

    /// \return The pixel position as a tc::Vector3<size_t>. The z compnent of
    /// the returned tc::Vector3<size_t> can be ignored.
    virtual Vector3<size_t> getPixelLocation() const;

    /// \return The range of pixels that can have their channels set to the same
    /// value as the pixel returned by
    /// tc::LinearPixelIterator::getPixelLocation. Not useful for the
    /// LinearPixelIterator. Useful for the RecursivePixelIterator.
    virtual PixelIterator::Range getAffectedPixels() const;
};

//------------------------------------------------------------------------------
// LinearPixelIteratorFactory
//------------------------------------------------------------------------------
/// \brief Registerable component for generating LinearPixelIterator instances.
//------------------------------------------------------------------------------
class LinearPixelIteratorFactory : public PixelIteratorFactory
{
    typedef std::vector<LinearPixelIterator> LinearPixelIterators;
    const size_t m_width;
    const size_t m_height;
    const size_t m_threadCount;
    LinearPixelIterators m_linearPixelIterators;

public:
    LinearPixelIteratorFactory(const size_t width, const size_t height,
                               const size_t threadCount);
    ~LinearPixelIteratorFactory();

    /// \param threadIndex A number in the range 0 to NumThreads. Where
    /// NumThreads is the number of threads being used for rendering.
    /// \return A LinearPixelIterator as a PixelIterator. Returns a new
    /// PixelIterator for each thread.
    virtual PixelIterator& begin(const size_t threadIndex);
};

}  // namespace tc
#endif  // TC_LINEAR_PIXELITERATOR
