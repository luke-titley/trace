//------------------------------------------------------------------------------
// Copywrite Luke Titley 2015
//------------------------------------------------------------------------------
#ifndef TC_TEST
#define TC_TEST
//------------------------------------------------------------------------------
namespace tc
{
class LogContext;
}

//------------------------------------------------------------------------------
/// \brief Tests a condition and if the condition fails, reports an error to the
/// given logger.
///
/// Using a macro allows us to include the filename and line number of the
/// condition for if it fails. For use in unit testing only.
//------------------------------------------------------------------------------
#define TC_IS(LOG, COND) tc::test(LOG, COND, __FILE__, __LINE__)

namespace tc
{
class LogContext;

//------------------------------------------------------------------------------
/// \brief Reports if a test has passed or failed.
///
/// This shouldn't be used directly. Use TC_IS(logContext, <<Condition>>)
/// instead.
/// \param logContext The logger to write to.
/// \param cond The condition to test.
/// \param filename A string of the full file path, for the source file
/// containing the condition.
/// \param lineno The line number in the source file that the condition is being
/// tested on.
//------------------------------------------------------------------------------
void test(const tc::LogContext& logContext, bool cond, const char* filename,
          int lineno);

//------------------------------------------------------------------------------
/// \brief Runs all the registered tests.
//------------------------------------------------------------------------------
void runTests(const tc::LogContext& logContext);

}  // namespace tc
#endif  // TC_TEST
