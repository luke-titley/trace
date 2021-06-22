//------------------------------------------------------------------------------
// Copywrite Luke Titley 2015
//------------------------------------------------------------------------------
#ifndef TC_THREAD_H
#define TC_THREAD_H

#include <cstdlib>

//------------------------------------------------------------------------------
namespace tc
{
class RWLock_Pimpl;
class ThreadBundle_Pimpl;

//------------------------------------------------------------------------------
// Range
//------------------------------------------------------------------------------
/// \brief Stores a one dimensional number range.
///
/// Consists of two numbers.
/// An upper and a lower bound.
//------------------------------------------------------------------------------
class Range
{
public:
    /// \brief The smallest number permitted in the range.
    const size_t m_lower;
    /// \brief The largest number permitted in the range.
    const size_t m_upper;

    /// \brief Initialises a new Range instance.
    /// \param lower The smallest number permitted in the range.
    /// \param upper The largest number permitted in the range.
    inline Range(const size_t lower, const size_t upper)
        : m_lower(lower), m_upper(upper)
    {
    }

    /// \brief Initializes a new Range instance, by copying from the given
    /// source range.
    /// \param range A source Range instance to copy from.
    inline Range(const Range& range)
        : m_lower(range.m_lower), m_upper(range.m_upper)
    {
    }

    /// \brief A value halfway between the upper and lower limits.
    inline size_t mid() const
    {
        size_t mid = m_lower + ((m_upper - m_lower) / 2);
        return mid;
    }

    /// \brief Splits the range into two Ranges along the midway point and
    /// returns the range below the mid point.
    inline Range splitLower() const
    {
        size_t mid = m_lower + ((m_upper - m_lower) / 2);
        return Range(m_lower, mid);
    }

    /// \brief Splits the range into two Ranges along the midway point and
    /// returns the range above the mid point.
    inline Range splitUpper() const
    {
        size_t mid = m_lower + ((m_upper - m_lower) / 2);
        return Range(mid, m_upper);
    }
};

//------------------------------------------------------------------------------
// getNumProcs
//------------------------------------------------------------------------------
/// \brief Returns the number of hardware processors available on the current
/// machine.
//------------------------------------------------------------------------------
size_t getNumProcs();

//------------------------------------------------------------------------------
// ThreadBundle
//------------------------------------------------------------------------------
/// \brief Spawns and manages a collection of threads.
///
/// This class abstracts away the platform specific details for multithreading.
/// It provides an abstract base class for working with threads.
///
/// Concrete implementations of ThreadBundle must implement
/// tc::ThreadBundle::run. All other functionality is provided by ThreadBundle.
///
/// ThreadBundle instances are given a tc::Range object which specifies an upper
/// and lower bound for indices. The Bundle will automatically split the
/// tc::Range into smaller ranges to be passed to the threads it spawns.
//------------------------------------------------------------------------------
class ThreadBundle
{
public:
    friend class ThreadBundle_Pimpl;

    /// \brief Initializes a ThreadBundle instance.
    /// \param range An upper and lower bound of indices which specify the range
    /// of work.
    /// \param threadCount The number of threads to spawn.
    ThreadBundle(const Range& range, size_t threadCount);
    virtual ~ThreadBundle();

    /// \return The number of threads spawned and managed by this
    /// tc::ThreadBundle.
    size_t getThreadCount() const;

    /// \brief Start the worker threads.
    virtual void start();
    /// \brief Wait for the worker threads to complete.
    virtual void join();
    /// \brief Signal the worker threads to stop cleanly.
    virtual void stop();
    /// \brief Can be used to query the progress of the worker threads.
    size_t progressRead();

protected:
    /// \brief Used by worker threads to test if they should shutdown.
    bool shouldStop();
    /// \brief Used by worker threads to increment the record of work done as
    /// they work.
    void progressIncrement(const size_t increment);

private:
    virtual void run(const size_t threadIndex, const Range& range) = 0;
    ThreadBundle_Pimpl* m_pimpl;
};

//------------------------------------------------------------------------------
// ProgressReader
//------------------------------------------------------------------------------
/// \brief A utility for iterating until a ThreadBundle has finished its work.
///
/// Can report the current progress of work as a percentage via
/// tc::ProgressReader::computePercentComplete.
///
/// tc::ProgressReader::next will continue to return true until all the work is
/// done.
//------------------------------------------------------------------------------
class ProgressReader
{
public:
    /// \brief Initialises a new ProgressReader instance.
    /// \param threadBundle The threadBundle whose progress to monitor.
    /// \param upperLimit The number of items to process by the given thread
    /// bundle.
    inline ProgressReader(ThreadBundle& threadBundle, const size_t upperLimit);
    /// \return true unless the image is finished reading, in which case, return
    /// false.
    inline bool next();
    /// \brief Gives us a number out of 100 for how far we are through.
    inline float computePercentComplete() const;

private:
    size_t m_progress;
    ThreadBundle& m_threadBundle;
    const size_t m_upperLimit;
};

//------------------------------------------------------------------------------
inline ProgressReader::ProgressReader(ThreadBundle& threadBundle,
                                      const size_t upperLimit)
    : m_progress(0), m_threadBundle(threadBundle), m_upperLimit(upperLimit)
{
}

//------------------------------------------------------------------------------
inline bool ProgressReader::next()
{
    m_progress = m_threadBundle.progressRead();  // TODO LT: Use condition
                                                 // variable here in the
                                                 // future.
    const bool result = m_progress < m_upperLimit;
    return result;
}

//------------------------------------------------------------------------------
inline float ProgressReader::computePercentComplete() const
{
    const float result = (float(m_progress) / float(m_upperLimit)) * 100.0f;
    return result;
}

//------------------------------------------------------------------------------
// RWLock
//------------------------------------------------------------------------------
/// \brief Implements a Read/Write lock.
/// \code
/// RWLock lock; // A Read/Write lock.
///
/// // Thread 0: Access the lock for writing.
/// lock.writeLock();
///     // Write to some shared memory.
/// lock.unlock();
///
/// // Thread 1: Access the lock for reading.
/// lock.readLock();
///     // Read from some shared memory.
/// lock.unlock();
///
/// \endcode
//------------------------------------------------------------------------------
class RWLock
{
public:
    /// \brief Initializes a RWLock instance.
    RWLock();
    ~RWLock();

    /// \brief Lock the RWLock for reading from.
    void readLock();
    /// \brief Lock the RWLock for writing to.
    void writeLock();
    /// \brief Unlock the RWLock.
    void unlock();

private:
    RWLock_Pimpl* m_pimpl;
};

//------------------------------------------------------------------------------
// RWLock_Read
//------------------------------------------------------------------------------
/// \brief An RAII object for scoping the locking/unlocking of read access to a
/// Read/Write lock.
/// \code
/// RWLock lock; // A Read/Write lock.
///
/// // Thread 0: Access the lock for writing.
/// lock.writeLock();
///     // Write to some shared memory.
/// lock.unlock();
///
/// // Thread 1: Access the lock for reading.
/// {
///     RWLock_Read read(lock); // Lock the Read/Write lock with read access.
///     // Read from some shared memory.
/// }
/// \endcode
//------------------------------------------------------------------------------
class RWLock_Read
{
    RWLock& m_lock;

public:
    RWLock_Read(RWLock& lock) : m_lock(lock)
    {
        m_lock.readLock();
    }

    ~RWLock_Read()
    {
        m_lock.unlock();
    }
};

//------------------------------------------------------------------------------
// RWLock_Write
//------------------------------------------------------------------------------
/// \brief An RAII object for scoping the locking/unlocking of write access to a
/// Read/Write lock.
/// \code
/// RWLock lock; // A Read/Write lock.
///
/// // Thread 0: Access the lock for writing.
/// {
///     RWLock_Write write(lock); // Lock the Read/Write lock with write access.
///     // Write to some shared memory.
/// }
///
/// // Thread 1: Access the lock for reading.
/// {
///     RWLock_Read read(lock); // Lock the Read/Write lock with read access.
///     // Read from some shared memory.
/// }
/// \endcode
//------------------------------------------------------------------------------
class RWLock_Write
{
    RWLock& m_lock;

public:
    RWLock_Write(RWLock& lock) : m_lock(lock)
    {
        m_lock.writeLock();
    }

    ~RWLock_Write()
    {
        m_lock.unlock();
    }
};

}  // namespace tc

#endif  // TC_THREAD_H
