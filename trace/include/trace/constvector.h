//------------------------------------------------------------------------------
// Copywrite Luke Titley 2015
//------------------------------------------------------------------------------
#ifndef TC_CONSTVECTOR
#define TC_CONSTVECTOR
//------------------------------------------------------------------------------
#include "trace/assert.h"
#include "trace/log.h"
//------------------------------------------------------------------------------
#include <cstdlib>
#include <cstring>
//------------------------------------------------------------------------------

namespace tc
{

//------------------------------------------------------------------------------
// ConstVector
//------------------------------------------------------------------------------
/// \brief A std::vector like container that supports classes that have const
/// fields.
///
/// Can store class types that do not have a default constructor so long as they
/// have a copy constructor.
/// This container does not have a 'resize' method, however it does have
/// 'reserve'.
///
/// It uses 'placement' new to construct element values and malloc and realloc
/// to allocate.
///
/// \usage No support for custom allocators and currently no support for aligned
/// data types.
//------------------------------------------------------------------------------
template <typename T>
class ConstVector
{
private:
    T* m_begin;
    size_t m_size;
    size_t m_reserved;

    void increaseAllocation(size_t size)
    {
        void* ptr = static_cast<void*>(m_begin);
        ptr = realloc(ptr, sizeof(T) * size);
        m_begin = static_cast<T*>(ptr);
        m_reserved = size;
    }

    void grow()
    {
        if (m_size == m_reserved)
        {
            increaseAllocation((m_reserved + 1) + (m_reserved / 2));
        }
        ++m_size;
    }

public:
    typedef T* iterator;
    typedef const T* const_iterator;

    /// \brief Initialises an empty tc::ConstVector.
    ConstVector() : m_begin(0), m_size(0), m_reserved(0)
    {
    }

    /// \brief Initialises a tc::ConstVector, buy copying the contents of
    /// 'constVector'.
    /// \param constVector The source ConstVector to copy from.
    ConstVector(const ConstVector& constVector)
        : m_begin(0), m_size(0), m_reserved(0)
    {
        increaseAllocation(constVector.size());
        m_size = constVector.size();
        for (size_t i = 0; i != m_size; ++i)
        {
            new (&(m_begin[i])) T(constVector.m_begin[i]);
        }
    }

    /// \brief Un-initialises the contents of the ConstVector.
    ~ConstVector()
    {
        if (m_begin != 0)
        {
            for (size_t i = 0; i != m_size; ++i)
            {
                m_begin[i].~T();
            }
            free(m_begin);
        }
        m_begin = 0;
        m_size = 0;
        m_reserved = 0;
    }

    /// \return An object that can be used for forward to back iteration
    /// of the elements in the ConstVector. The object can be used to modify the
    /// elements.
    iterator begin()
    {
        return m_begin;
    }

    /// \return An object that can be used for forward to back iteration
    /// of the elements in the ConstVector. The object can not be used to modify
    /// the elements.
    const_iterator begin() const
    {
        return m_begin;
    }

    /// \brief Empties the contents of the ConstVector. After calling 'clear'
    /// for a ConstVector instance. The instance will have no values.
    void clear()
    {
        for (size_t i = 0; i != m_size; ++i)
        {
            m_begin[i].~T();
        }
        m_size = 0;
    }

    /// \return True of this ConstVector has no values. False if it has one or
    /// more values.
    bool empty() const
    {
        return m_size == 0;
    }

    /// \return A reference to the end of the array. Can be used in the
    /// termination condition for iterating over the elements of a ConstVector
    /// instance. The returned object can also be decremented to obtain a
    /// reference to the last element in the array. Repeated decrements can
    /// return the first element in the array. The returned iterator can be used
    /// to modify the elements of the array.
    ///
    /// \usage Behaviour is undefined if the returned iterator is dereferenced
    /// immediately after a call to 'end'. The iterator must be decremented
    /// before hand. Behaviour is also undefined if the call 'end' is used for a
    /// ConstVector that is empty.
    iterator end()
    {
        return m_begin + m_size;
    }

    /// \return A reference to the end of the array. Can be used in the
    /// termination condition for iterating over the elements of a ConstVector
    /// instance. The returned object can also be decremented to obtain a
    /// reference to the last element in the array. Repeated decrements can
    /// return the first element in the array. The returned iterator can not be
    /// used to modify the elements of the array.
    ///
    /// \usage Behaviour is undefined if the returned iterator is dereferenced
    /// immediately after a call to 'end'. The iterator must be decremented
    /// before hand. Behaviour is also undefined if the call 'end' is used for a
    /// ConstVector that is empty.
    const_iterator end() const
    {
        return m_begin + m_size;
    }

    /// \return A refernece to the element specified by the index 'index'. The
    /// returned reference can be modified.
    /// \param index The position of the element we want in the array.
    T& operator[](size_t index)
    {
        return m_begin[index];
    }

    /// \return A refernece to the element specified by the index 'index'. The
    /// returned reference can be modified.
    /// \param index The position of the element we want in the array.
    const T& operator[](size_t index) const
    {
        return m_begin[index];
    }

    /// \brief Appends a new element onto the end of a ConstVector instance.
    void push_back(const T& value)
    {
        const size_t i = m_size;
        grow();
        new ((void*)&(m_begin[i]))
            T(value);  // TODO LT: Currently not handling alignment
    }

    /// \brief Removes an element from the end of a ConstVector instance.
    void pop_back()
    {
        assert(m_size != 0);
        --m_size;
        m_begin[m_size].~T();  // Call the destructor directly
    }

    /// \brief Removes a number of elements from the end of a ConstVector
    /// instance.
    void pop(const size_t amount)
    {
        assert(m_size >= amount);
        for (size_t i = 0; i != amount; ++i)
        {
            m_begin[m_size - amount].~T();  // Call the destructor directly
        }
        m_size -= amount;  // Actually resize
    }

    /// \param An offset from the back of the ConstVector, where 0 is the last
    /// element, 1 is the second from last, 2 is the third from last etc.
    /// \return A refernece to the last element in the ConstVector, or another
    /// element if index is not zero.
    /// \usage The returned reference can not be modified.
    const T& top(const size_t index = 0) const
    {
        assert(index < m_size);
        return m_begin[m_size - (1 + index)];
    }

    /// \param An offset from the back of the ConstVector, where 0 is the last
    /// element, 1 is the second from last, 2 is the third from last etc.
    /// \return A refernece to the last element in the ConstVector, or another
    /// element if index is not zero.
    /// \usage The returned reference can be modified.
    T& top(const size_t index = 0)
    {
        assert(index < m_size);
        return m_begin[m_size - (1 + index)];
    }

    /// \brief Returns the last element in the ConstVector. See also
    /// ConstVector::top. Back is implemented as ConstVector::top(0);
    /// \return The last element in the ConstVector.
    const T& back() const
    {
        return top();
    }

    /// \brief Returns the last element in the ConstVector. See also
    /// ConstVector::top. Back is implemented as ConstVector::top(0);
    /// \return The last element in the ConstVector.
    T& back()
    {
        return top();
    }

    /// \brief Allocates space in a ConstVector instance for at least 'size'
    /// elements.
    /// \param size The minimum number of elements to allocate space for.
    /// \usage Can improve performance when the number of elements needed is
    /// known ahead of time.
    void reserve(size_t size)
    {
        if (size > m_reserved)
        {
            increaseAllocation(size);
        }
    }

    /// \return The number of elements in the ConstVector.
    size_t size() const
    {
        return m_size;
    }

    /// \brief Copies the contents of 'constVector' into this ConstVector
    /// instance.
    void operator=(const ConstVector& constVector)
    {
        clear();
        reserve(constVector.size());
        for (size_t i = 0; i != constVector.size(); ++i)
        {
            push_back(constVector.m_begin[i]);
        }
    }
};

//------------------------------------------------------------------------------
// Runs all the unit tests for the 'constvector' header file.
/// \cond
void constvectorRunUnitTests(const tc::LogContext& logContext);
/// \cond

}  // namespace tc
#endif  // TC_CONSTVECTOR
