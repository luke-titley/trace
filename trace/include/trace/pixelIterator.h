//------------------------------------------------------------------------------
// Copywrite Luke Titley 2015
//------------------------------------------------------------------------------
#ifndef TC_PIXELITERATOR
#define TC_PIXELITERATOR
//------------------------------------------------------------------------------
#include "trace/vector.h"
//------------------------------------------------------------------------------
namespace tc
{

//------------------------------------------------------------------------------
// PixelIterator
//------------------------------------------------------------------------------
/// \brief An interface which specifies the order of traversal for pixels on the
/// image plane.
///
/// This could be linearly from left to right and top to bottom or recursively.
/// The following methods must be implemented:
/// -next
/// -getPixelLocation
///
/// getAffectedPixels should be implemented for the case when one pixel value
/// might be written to multiple positions. It is only needed for the recursive
/// pixel iterator which shades a block of pixels the same colour, then refines
/// the image, progressively shading each pixel independantly.
//------------------------------------------------------------------------------
class PixelIterator
{
public:
    ///\brief Stores a 2 dimensional range for specifying a region of pixels.
    ///
    /// Used only for tc::PixelIterator::getAffectedPixels.
    class Range
    {
    public:
        const Vector3<size_t> m_lower;
        const Vector3<size_t> m_upper;

        Range(const Vector3<size_t>& lower, const Vector3<size_t>& upper)
            : m_lower(lower), m_upper(upper)
        {
        }
    };

    virtual ~PixelIterator() {};

    /// \brief To be used in a while loop. It must return true until all pixels
    /// on the image plane have been traversed.
    virtual bool next() = 0;

    /// \return the location of the pixel we are currently on.
    virtual Vector3<size_t> getPixelLocation() const = 0;

    /// \brief This is only for recursive pixel iterators that start off filling
    /// the image plane with approximate pixel values and then refining them.
    /// For linear pixel iterators it need not be overridden and this default
    /// implementation can be used.
    virtual Range getAffectedPixels() const
    {
        const Vector3<size_t> pixel = getPixelLocation();
        return PixelIterator::Range(pixel, pixel + Vector3<size_t>(1));
    }
};

//------------------------------------------------------------------------------
// PixelIteratorFactory
//------------------------------------------------------------------------------
/// \brief Builds a new pixel iterator for the thread given by 'threadIndex'.
/// Where the threadIndex ranges from 0 to threadCount, and where threadCount is
/// the number of threads being used for rendering.
//------------------------------------------------------------------------------
class PixelIteratorFactory
{
public:
    virtual ~PixelIteratorFactory() {};
    /// Returns a pixel iterator for the given thread. Pixel iterators can be
    /// re-used.
    /// \param threadIndex A unique index representing a rendering thread.
    /// Ranges from 0 to threadCount, where threadCount is the number of threads
    /// being used for rendering.
    /// \usage Must be implemented and must be thread safe.
    virtual PixelIterator& begin(const size_t threadIndex) = 0;
};

}  // namespace tc
#endif  // TC_PIXELITERATOR
