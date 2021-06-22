//------------------------------------------------------------------------------
// Copywrite Luke Titley 2015
//------------------------------------------------------------------------------
#include "trace/thread.h"
//------------------------------------------------------------------------------
#include "trace/assert.h"
#include <pthread.h>
#include <unistd.h>
#include <vector>

//------------------------------------------------------------------------------
namespace tc
{

//------------------------------------------------------------------------------
size_t getNumProcs()
{
    return sysconf(_SC_NPROCESSORS_CONF);
}

//------------------------------------------------------------------------------
// Thread
//------------------------------------------------------------------------------
struct Thread
{
    const Range m_range;
    ThreadBundle& m_threadBundle;
    size_t m_threadIndex;

    Thread(ThreadBundle& threadBundle, const Range& range,
           const size_t threadIndex)
        : m_range(range),
          m_threadBundle(threadBundle),
          m_threadIndex(threadIndex)
    {
    }
};

typedef std::vector<pthread_t> ThreadIds;

//------------------------------------------------------------------------------
// ThreadBundle_Pimpl
//------------------------------------------------------------------------------
class ThreadBundle_Pimpl
{
public:
    ThreadBundle_Pimpl(const Range& range, const size_t threadCount);
    const pthread_t m_owningThread;
    size_t m_progress;
    const Range m_range;
    const size_t m_threadCount;
    unsigned int m_stop;
    Thread* m_threads;
    ThreadIds m_threadIds;

    static void* run(void*);
};

//------------------------------------------------------------------------------
// ThreadBundle_Pimpl
//------------------------------------------------------------------------------
ThreadBundle_Pimpl::ThreadBundle_Pimpl(const Range& range,
                                       const size_t threadCount)
    : m_owningThread(pthread_self()),
      m_progress(0),
      m_range(range),
      m_threadCount(threadCount),
      m_stop(0),
      m_threads(0)
{
    assert(threadCount != 0);
}

//------------------------------------------------------------------------------
void* ThreadBundle_Pimpl::run(void* self)
{
    Thread* thread = static_cast<Thread*>(self);

    thread->m_threadBundle.run(thread->m_threadIndex, thread->m_range);

    return 0;
}

//------------------------------------------------------------------------------
// ThreadBundle
//------------------------------------------------------------------------------
ThreadBundle::ThreadBundle(const Range& range, const size_t threadCount)
    : m_pimpl(new ThreadBundle_Pimpl(range, threadCount))
{
}

//------------------------------------------------------------------------------
ThreadBundle::~ThreadBundle()
{
    assert(m_pimpl->m_threadIds.empty());
    delete m_pimpl;
}

//------------------------------------------------------------------------------
size_t ThreadBundle::getThreadCount() const
{
    return m_pimpl->m_threadCount;
}

//------------------------------------------------------------------------------
void ThreadBundle::start()
{
    assert(m_pimpl->m_owningThread == pthread_self());
    assert(m_pimpl->m_threads == 0);
    assert(m_pimpl->m_threadCount > 0);

    const size_t threadCount = m_pimpl->m_threadCount;

    // We allocate a new block of memory for the thread array.
    // Then placement new the memory. This allows us to keep all the thread data
    // const.
    Thread* threads =
        static_cast<Thread*>(malloc(sizeof(Thread) * threadCount));

    const size_t lower = m_pimpl->m_range.m_lower;
    const size_t upper = m_pimpl->m_range.m_upper;
    const size_t range = upper - lower;
    const size_t step = range / threadCount;
    const size_t remainder = range % threadCount;

    m_pimpl->m_threadIds.resize(threadCount);

    // Create all the threads except the last thread
    for (size_t i = 0; i != threadCount - 1; ++i)
    {
        Thread* thread = new (threads + i) Thread(
            *this, Range(lower + (i * step), lower + (i * step) + step), i);
        pthread_create(&(m_pimpl->m_threadIds[i]), 0, ThreadBundle_Pimpl::run,
                       static_cast<void*>(thread));
    }

    // Create the last thread
    const size_t lastThreadIndex = threadCount - 1;
    Thread* thread = new (threads + lastThreadIndex) Thread(
        *this, Range(lower + (lastThreadIndex * step),
                     lower + (lastThreadIndex * step) + step + remainder),
        lastThreadIndex);

    pthread_create(&(m_pimpl->m_threadIds[threadCount - 1]), 0,
                   ThreadBundle_Pimpl::run, static_cast<void*>(thread));

    // Create the last thread
    m_pimpl->m_threads = threads;
}

//------------------------------------------------------------------------------
void ThreadBundle::join()
{
    assert(m_pimpl->m_owningThread == pthread_self());
    if (m_pimpl->m_threads == 0)
    {
        return;
    }

    // Wait for the threads to terminate
    for (size_t i = 0; i != m_pimpl->m_threadCount; ++i)
    {
        pthread_join(m_pimpl->m_threadIds[i], 0);
    }

    // Cleanup the thread information
    for (size_t i = 0; i != m_pimpl->m_threadCount; ++i)
    {
        m_pimpl->m_threads[i].~Thread();
    }

    // Free everything
    m_pimpl->m_threadIds.resize(0);
    free(m_pimpl->m_threads);
    m_pimpl->m_threads = 0;
}

//------------------------------------------------------------------------------
void ThreadBundle::stop()
{
    __sync_bool_compare_and_swap(&m_pimpl->m_stop, 0, 1);
}

//------------------------------------------------------------------------------
bool ThreadBundle::shouldStop()
{
    const bool result = __sync_or_and_fetch(&m_pimpl->m_stop, 0) == 1;
    return result;
}

//------------------------------------------------------------------------------
void ThreadBundle::progressIncrement(const size_t increment)
{
    __sync_add_and_fetch(&m_pimpl->m_progress, increment);
}

//------------------------------------------------------------------------------
size_t ThreadBundle::progressRead()
{
    const size_t progress = __sync_add_and_fetch(&m_pimpl->m_progress, 0);
    return progress;
}

//------------------------------------------------------------------------------
// RWLock_Pimpl
//------------------------------------------------------------------------------
class RWLock_Pimpl
{
public:
    pthread_rwlock_t m_rwlock;
    RWLock_Pimpl()
    {
        pthread_rwlock_init(&m_rwlock, NULL);
    }
    ~RWLock_Pimpl()
    {
        pthread_rwlock_destroy(&m_rwlock);
    }
};

//------------------------------------------------------------------------------
// RWLock
//------------------------------------------------------------------------------
RWLock::RWLock() : m_pimpl(new RWLock_Pimpl)
{
}

RWLock::~RWLock()
{
    delete m_pimpl;
    m_pimpl = 0;
}

void RWLock::readLock()
{
    pthread_rwlock_rdlock(&(m_pimpl->m_rwlock));
}

void RWLock::writeLock()
{
    pthread_rwlock_wrlock(&(m_pimpl->m_rwlock));
}

void RWLock::unlock()
{
    pthread_rwlock_unlock(&(m_pimpl->m_rwlock));
}

}  // namespace tc
