//------------------------------------------------------------------------------
// Copywrite Luke Titley 2015
//------------------------------------------------------------------------------
#include "trace/recursivePixelIterator.h"
//------------------------------------------------------------------------------

namespace tc
{

//------------------------------------------------------------------------------
// RecursivePixelIterator
//------------------------------------------------------------------------------
RecursivePixelIterator::RecursivePixelIterator()
    : m_width(0), m_x(0), m_begun(false)
{
}

//------------------------------------------------------------------------------
void RecursivePixelIterator::begin(const size_t width, const size_t height,
                                   const size_t threadCount,
                                   const size_t threadIndex)
{
    // Split by height
    const size_t splitHeight = height / threadCount;
    size_t blockHeight = splitHeight;
    const bool isLastThread = threadIndex == (threadCount - 1);
    if (isLastThread)
    {
        const size_t surplus = height % threadCount;
        blockHeight += surplus;
    }
    const size_t lower = threadIndex * splitHeight;
    const size_t upper = lower + blockHeight;

    m_width = width;
    m_x = 0;
    m_y.begin(Vector3<size_t>(0, lower), Vector3<size_t>(width, upper));
    m_y.next();
    m_begun = false;
}

//------------------------------------------------------------------------------
bool RecursivePixelIterator::next()
{
    return m_y.next();
}

//------------------------------------------------------------------------------
Vector3<size_t> RecursivePixelIterator::getPixelLocation() const
{
    return *m_y;
}

//------------------------------------------------------------------------------
PixelIterator::Range RecursivePixelIterator::getAffectedPixels() const
{
    const RecursiveSequenceIterator::Region affectedRegion =
        m_y.getAffectedRegion();
    return PixelIterator::Range(affectedRegion.m_lower, affectedRegion.m_upper);
}

//------------------------------------------------------------------------------
// RecursivePixelIteratorFactory
//------------------------------------------------------------------------------
RecursivePixelIteratorFactory::RecursivePixelIteratorFactory(
    const size_t width, const size_t height, const size_t threadCount)
    : m_width(width), m_height(height), m_threadCount(threadCount)
{
    m_recursivePixelIterators = new RecursivePixelIterator[threadCount];
}

//------------------------------------------------------------------------------
RecursivePixelIteratorFactory::~RecursivePixelIteratorFactory()
{
    delete[] m_recursivePixelIterators;
}

//------------------------------------------------------------------------------
PixelIterator& RecursivePixelIteratorFactory::begin(const size_t threadIndex)
{
    RecursivePixelIterator& recursivePixelIterator =
        m_recursivePixelIterators[threadIndex];

    recursivePixelIterator.begin(m_width, m_height, m_threadCount, threadIndex);

    return recursivePixelIterator;
}

}  // namespace tc
