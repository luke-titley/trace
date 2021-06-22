//------------------------------------------------------------------------------
// Copywrite Luke Titley 2015
//------------------------------------------------------------------------------
#ifndef TC_LOGCONTEXT
#define TC_LOGCONTEXT
//------------------------------------------------------------------------------
#include <string>
//------------------------------------------------------------------------------
namespace tc
{

//------------------------------------------------------------------------------
// LogContext
//------------------------------------------------------------------------------
/// \brief An interface for writing user output. Using a LogContext avoids
/// writing to stdout directly and allows for writing the log to a graphical
/// user interface.
//------------------------------------------------------------------------------
class LogContext
{
public:
    enum Logging
    {
        DISABLED = 0,
        ENABLED = 1
    };

    LogContext();

    ///\brief Writes a float to the log.
    void log(const float value) const;
    ///\brief Writes an in to the log.
    void log(const int value) const;
    ///\brief Writes a string to the log.
    void log(const char* value) const;
    ///\brief Writes a string to the log.
    void log(const std::string& value) const;

private:
    Logging m_enabled;
};
}  // namespace tc
#endif  // TC_LOGCONTEXT
