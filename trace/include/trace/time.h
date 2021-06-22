//------------------------------------------------------------------------------
// Copywrite Luke Titley 2015
//------------------------------------------------------------------------------
#ifndef TC_TIME
#define TC_TIME

#include <ctime>

namespace tc
{
//------------------------------------------------------------------------------
// Timer
//------------------------------------------------------------------------------
/// A simple timer, for basic profiling.
//------------------------------------------------------------------------------
class Timer
{
    time_t m_start;

public:
    Timer() : m_start(time(0))
    {
    }

    /// \brief Gives us the elapsed time since the timer was initialised.
    inline size_t elapsedTime() const
    {
        const time_t elapsed = time(0) - m_start;
        return static_cast<size_t>(elapsed);
    }
};
}  // namespace tc
#endif  // TC_TIME
