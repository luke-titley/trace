//------------------------------------------------------------------------------
// Copywrite Luke Titley 2015
//------------------------------------------------------------------------------
#ifndef TC_RECURSIVE_PIXELITERATOR
#define TC_RECURSIVE_PIXELITERATOR
//------------------------------------------------------------------------------
#include "trace/pixelIterator.h"
#include "trace/recursiveSequenceIterator.h"
//------------------------------------------------------------------------------
#include <vector>
//------------------------------------------------------------------------------
namespace tc
{

//------------------------------------------------------------------------------
// RecursivePixelIterator
//------------------------------------------------------------------------------
/// Iterates over all the pixels in the image plane recursively.
///
/// \brief Choosing the centre pixel first, then the pixels halfway between the
/// the center and the edge, and then the pixels halfway between those, and so
/// on until all the pixels in the image have been traversed. This iterator is
/// for progressive rendering. Each pixel chosen is helping to refine the final
/// picture. This iterator does return the same pixels more than once. It is up
/// to the user to keep track of which pixels have already been visited and to
/// skip them.
///
/// \code
/// RecursivePixelIterator it;
///
/// it.begin(256, 256, 4, 0); // Thread 0
///
/// while(it.next())
/// {
///     const Vector3<float> pixelLocation = it.getPixelLocation();
/// }
///
/// \endcode
//------------------------------------------------------------------------------
class RecursivePixelIterator : public PixelIterator
{
public:
    /// Initialised the recursive pixel iterator.
    RecursivePixelIterator();

    /// \brief Sets/Resets the recursive pixel iterator. This should be called
    /// prior to using it in a loop.
    /// \param width The resolution of the x axis of the image.
    /// \param height The resolution of the y axis of the image.
    /// \param threadCount The number of threads being used for rendering.
    /// \param threadIndex The index of the current thread being used for
    ///     rendering.
    void begin(const size_t width, const size_t height,
               const size_t threadCount, const size_t threadIndex);

    /// \brief Increments the iterator. Moves this iterator on to the next
    /// pixel. Returns false when finished iterating.
    virtual bool next();

    /// \return The x and y coordinates of the current pixel.
    virtual Vector3<size_t> getPixelLocation() const;

    /// \return The range of pixels that have the same color value as the pixel
    /// returned by tc::RecursivePixelIteraotr::getAffectedPixels.
    virtual PixelIterator::Range getAffectedPixels() const;

private:
    size_t m_width;
    size_t m_x;
    bool m_begun;
    RecursiveSequenceIterator m_y;
};

//------------------------------------------------------------------------------
// RecursivePixelIteratorFactory
//------------------------------------------------------------------------------
/// \brief Allocates and manages RecursivePixelIterator instances.
///
/// Retains ownership of all RecursivePixelIterator instances.
//------------------------------------------------------------------------------
class RecursivePixelIteratorFactory : public PixelIteratorFactory
{
public:
    /// Initialises the RecursivePixelIterator.
    /// \param width The image width
    /// \param height The image height
    /// \param threadCount The number of threads being used for rendering.
    RecursivePixelIteratorFactory(const size_t width, const size_t height,
                                  const size_t threadCount);
    ~RecursivePixelIteratorFactory();

    /// Returns a new RecursivePixelIterator.
    /// \usage This method is thread safe.
    /// \param threadIndex The current thread.
    /// \return A reference to a RecursivePixelIterator that is unique to the
    /// thread specified by 'threadCount'.
    virtual PixelIterator& begin(const size_t threadIndex);

private:
    const size_t m_width;
    const size_t m_height;
    const size_t m_threadCount;
    RecursivePixelIterator* m_recursivePixelIterators;
};

}  // namespace tc
#endif  // TC_RECURSIVE_PIXELITERATOR
