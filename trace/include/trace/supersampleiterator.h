//------------------------------------------------------------------------------
// Copywrite Luke Titley 2015
//------------------------------------------------------------------------------
#ifndef TC_SUPERSAMPLEITERATOR
#define TC_SUPERSAMPLEITERATOR
//------------------------------------------------------------------------------
#include "trace/assert.h"
#include "trace/vector.h"
//------------------------------------------------------------------------------
namespace tc
{

namespace shade
{

typedef float (*OffsetFunction)(const float a, const float b);

//------------------------------------------------------------------------------
// SuperSampleIterator
//------------------------------------------------------------------------------
/// \brief Provides a mechanism for iterating through a number of random
/// positions around a pixel.
///
/// As it's name implies, the SuperSampleIterator is intended to be used for
/// super sampling. That is producing a number of positions around a pixel that
/// can be ray traced. The irradiance values resulting from the ray tracing
/// operation can be averaged to produce an estimate for a pixel irradiance
/// value.
///
/// \tparam offsetFunction: A function which produces a random value between an
/// upper and lower bound. Use tc::generateRandomFloat in random.h if you
/// want to use a prebuilt function.
template <OffsetFunction offsetFunction>
class SuperSampleIterator
{
public:
    /// Initializes a SuperSampleIterator
    /// \param pixelPosition: The x and y coordinates of the pixel we wish to
    /// produce random sample positions around.
    /// \param pixelSize: The maximum x and y distance a random sample position
    /// is allowed to be from the pixelPosition.
    /// \param samplesPerPixel: The number of random positions to produce.
    SuperSampleIterator(const Vector3<float>& pixelPosition,
                        const Vector3<float>& pixelSize,
                        const size_t samplesPerPixel);

    /// Moves on to the next random sample position.
    /// \return True if there are more random positions to go through, False if
    /// not.
    bool next();

    /// \return A two dimensional position randomly offset from the
    /// 'pixelPosition' argument provided to
    /// SuperSampleIterator::SuperSampleIterator.
    Vector3<float> getOffset() const;

    /// \return A weighting that can be used to scale the irradiance of the
    /// sample whose position is provided by SuperSampleIterator::getOffset.
    float getWeight() const;

private:
    size_t m_current;
    const Vector3<float> m_pixelPosition;
    size_t m_i;
    const Vector3<float> m_pixelSize;
    const float m_samplesPerPixel;
    const float m_weight;
};

//------------------------------------------------------------------------------
template <OffsetFunction offsetFunction>
SuperSampleIterator<offsetFunction>::SuperSampleIterator(
    const Vector3<float>& pixelPosition, const Vector3<float>& pixelSize,
    const size_t samplesPerPixel)
    : m_current(0),
      m_pixelPosition(pixelPosition),
      m_i(0),
      m_pixelSize(pixelSize),
      m_samplesPerPixel(samplesPerPixel),
      m_weight(1.0f / static_cast<float>(samplesPerPixel))
{
}

//------------------------------------------------------------------------------
template <OffsetFunction offsetFunction>
bool SuperSampleIterator<offsetFunction>::next()
{
    if (m_i == m_samplesPerPixel)
    {
        return false;
    }
    m_current = m_i;
    ++m_i;
    return true;
}

//------------------------------------------------------------------------------
template <OffsetFunction offsetFunction>
Vector3<float> SuperSampleIterator<offsetFunction>::getOffset() const
{
    assert(m_current != m_samplesPerPixel);

    // Generate a point on the image plane that is randomly offset from the
    // given pixel position, but within the distance dictated by pixelSize.
    const float jitter_x =
        offsetFunction(m_pixelPosition.x, m_pixelPosition.x + m_pixelSize.x);
    const float jitter_y =
        offsetFunction(m_pixelPosition.y, m_pixelPosition.y + m_pixelSize.y);

    const Vector3<float> offset(jitter_x, jitter_y);

    return offset;
}

//------------------------------------------------------------------------------
template <OffsetFunction offsetFunction>
float SuperSampleIterator<offsetFunction>::getWeight() const
{
    return m_weight;
}
}  // namespace shade

}  // namespace tc
#endif  // TC_SUPERSAMPLEITERATOR
