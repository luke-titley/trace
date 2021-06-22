//------------------------------------------------------------------------------
// Copywrite Luke Titley 2015
//------------------------------------------------------------------------------
#ifndef TC_ASSERT
#define TC_ASSERT
//------------------------------------------------------------------------------
namespace tc
{

//------------------------------------------------------------------------------
// assert
//------------------------------------------------------------------------------
/// \brief Interrupts the application if the given condition fails.
///
/// \usage Only has an affect if the application has been compiled with '_DEBUG'
/// defined.
/// \param condition: A true or false boolean value. If false then the
/// application will interrupt. If true, then nothing will happen.
//------------------------------------------------------------------------------
#if defined(_DEBUG)
inline void assert(bool condition)
{
    if (!condition)
    {
#if defined(__i386) || defined(__x86_64__)
        __asm__("int $3");  // The intel crash
    }
#else
#error Unsupported CPU architecture.
#endif
}
#else
#define assert(A)
#endif
}  // namespace tc
#endif  // TC_ASSERT
