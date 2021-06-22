//------------------------------------------------------------------------------
// Copywrite Luke Titley 2015
//------------------------------------------------------------------------------
#include "trace/linearPixelIterator.h"
//------------------------------------------------------------------------------

namespace tc
{

//------------------------------------------------------------------------------
// LinearPixelIterator
//------------------------------------------------------------------------------
LinearPixelIterator::LinearPixelIterator()
    : m_currentI(0), m_i(0), m_upperLimit(0), m_width(0), m_hasBegun(false)
{
}

//------------------------------------------------------------------------------
void LinearPixelIterator::begin(const size_t width, const size_t height,
                                const size_t threadCount,
                                const size_t threadIndex)
{
    const size_t area = width * height;

    // Work out how many pixels to allocate to this pixel iterator
    size_t size = area / threadCount;
    bool isLastThread = threadIndex == (threadCount - 1);
    if (isLastThread)
    {
        const size_t surplus = area % threadCount;
        size += surplus;
    }
    m_i = threadIndex * size;
    m_upperLimit = m_i + size;
    m_width = width;
    m_hasBegun = false;
}

//------------------------------------------------------------------------------
bool LinearPixelIterator::next()
{
    m_currentI = m_i;
    if (m_currentI == m_upperLimit)
    {
        return false;
    }
    ++m_i;

    return true;
}

//------------------------------------------------------------------------------
Vector3<size_t> LinearPixelIterator::getPixelLocation() const
{
    size_t y = m_currentI / m_width;
    size_t x = m_currentI % m_width;

    return Vector3<size_t>(x, y, 0);
}

//------------------------------------------------------------------------------
PixelIterator::Range LinearPixelIterator::getAffectedPixels() const
{
    const Vector3<size_t> pixel = getPixelLocation();
    return PixelIterator::Range(pixel, pixel + Vector3<size_t>(1));
}

//------------------------------------------------------------------------------
// LinearPixelIteratorFactory
//------------------------------------------------------------------------------
LinearPixelIteratorFactory::LinearPixelIteratorFactory(const size_t width,
                                                       const size_t height,
                                                       const size_t threadCount)
    : m_width(width),
      m_height(height),
      m_threadCount(threadCount),
      m_linearPixelIterators(threadCount)
{
}

//------------------------------------------------------------------------------
LinearPixelIteratorFactory::~LinearPixelIteratorFactory()
{
}

//------------------------------------------------------------------------------
PixelIterator& LinearPixelIteratorFactory::begin(const size_t threadIndex)
{
    LinearPixelIterator& linearPixelIterator =
        m_linearPixelIterators[threadIndex];

    linearPixelIterator.begin(m_width, m_height, m_threadCount, threadIndex);

    return linearPixelIterator;
}

}  // namespace tc
