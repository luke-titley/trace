//------------------------------------------------------------------------------
// Copywrite Luke Titley 2015
//------------------------------------------------------------------------------
#include "trace/log.h"
//------------------------------------------------------------------------------
#include <iostream>

#define LOGGING ENABLED  // Logging can be enabled or disabled here.

namespace tc
{
namespace
{
//------------------------------------------------------------------------------
template <typename T>
void log(const LogContext::Logging enabled, const T value)
{
    if (enabled == LogContext::ENABLED)
    {
        std::cout << value;
    }
}
}  // namespace

//------------------------------------------------------------------------------
// tc::log
//------------------------------------------------------------------------------
LogContext::LogContext() : m_enabled(LOGGING)
{
}

//------------------------------------------------------------------------------
void LogContext::log(const float value) const
{
    tc::log(m_enabled, value);
}

//------------------------------------------------------------------------------
void LogContext::log(const int value) const
{
    tc::log(m_enabled, value);
}

//------------------------------------------------------------------------------
void LogContext::log(const char* value) const
{
    tc::log(m_enabled, value);
}

//------------------------------------------------------------------------------
void LogContext::log(const std::string& value) const
{
    tc::log(m_enabled, value);
}

}  // namespace tc
