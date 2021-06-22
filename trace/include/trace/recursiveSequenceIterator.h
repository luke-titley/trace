//------------------------------------------------------------------------------
// Copywrite Luke Titley 2015
//------------------------------------------------------------------------------
#ifndef TC_RECURSIVE_SEQUENCEITERATOR
#define TC_RECURSIVE_SEQUENCEITERATOR
//------------------------------------------------------------------------------
#include "trace/constvector.h"
#include "trace/vector.h"
//------------------------------------------------------------------------------
namespace tc
{

//------------------------------------------------------------------------------
// RecursiveSequenceIterator
//------------------------------------------------------------------------------
/// \brief Recursively subdivides a rectange until it is a grid of pixel sized
/// cells. Pixel coordinates are returned during the subdivision.
//------------------------------------------------------------------------------
class RecursiveSequenceIterator
{
public:
    /// \brief An two dimensional range. Defined by upper and lower bounds,
    /// and an axis.
    class Range
    {
    public:
        const Vector3<size_t> m_lower;
        const Vector3<size_t> m_upper;
        const size_t m_axis;

        Range(const Vector3<size_t>& lower, const Vector3<size_t>& upper,
              const size_t axis)
            : m_lower(lower), m_upper(upper), m_axis(axis)
        {
        }
    };

    /// \brief A two dimensional axis aligned bounding box.
    class Region
    {
    public:
        Vector3<size_t> m_lower;
        Vector3<size_t> m_upper;
        Region() : m_lower(0), m_upper(0)
        {
        }
    };

    typedef ConstVector<Range> RangeStack;

    RangeStack m_stacks[2];
    RangeStack* m_currentStack;
    RangeStack* m_newStack;
    Vector3<size_t> m_i;
    Region m_affectedRegion;

    enum
    {
        X = 0,
        Y = 1
    };

    inline RecursiveSequenceIterator()
        : m_currentStack(&(m_stacks[0])), m_newStack(&(m_stacks[1])), m_i(0)
    {
    }

    void begin(const Vector3<size_t>& lower, const Vector3<size_t>& upper)
    {
        // Reset everything
        m_i = 0;
        m_currentStack->clear();
        m_newStack->clear();

        // Push the first stack entry
        m_currentStack->push_back(Range(lower, upper, X));
    }

    inline bool next()
    {
        // Flip the current stack with the new stack if we are empty
        if (m_currentStack->empty())
        {
            m_currentStack =
                m_currentStack == m_stacks ? &(m_stacks[1]) : m_stacks;
            m_newStack = m_newStack == m_stacks ? &(m_stacks[1]) : m_stacks;
            m_newStack->clear();
        }

        // If we're still empty then we're done recursing
        if (m_currentStack->empty())
        {
            return false;
        }

        // Get the latest value from the top of the stack
        const Range top = m_currentStack->top();

        // Swap the uppers and lowers for output
        const Vector3<size_t> upper = top.m_upper;
        const Vector3<size_t> lower = top.m_lower;
        size_t axis = top.m_axis;

        // Pop the stack
        m_currentStack->pop_back();

        while (true)
        {
            const size_t oppositeAxis = axis == 0 ? 1 : 0;
            if (upper == lower)
            {
                m_i = lower;
            }
            else if ((upper - lower).area() <= 1)
            {
                m_i = lower;
            }
            else if ((upper[axis] - lower[axis]) <= 1)
            {
                axis = oppositeAxis;
                continue;
            }
            else
            {
                Vector3<size_t> lowerHalf = upper;
                Vector3<size_t> upperHalf = lower;
                size_t splitAxis = (lower[axis] + upper[axis]) / 2;
                lowerHalf[axis] = splitAxis;
                upperHalf[axis] = splitAxis;

                m_i = (lower + upper) / Vector3<size_t>(2);
                m_affectedRegion.m_lower = lower;
                m_affectedRegion.m_upper = upper;

                m_newStack->push_back(Range(lower, lowerHalf, oppositeAxis));
                m_newStack->push_back(Range(upperHalf, upper, oppositeAxis));
            }
            break;
        }
        return true;
    }

    Region getAffectedRegion() const
    {
        return m_affectedRegion;
    }

    Vector3<size_t> operator*() const
    {
        return m_i;
    }
};

}  // namespace tc
#endif  // TC_RECURSIVE_SEQUENCEITERATOR
