//------------------------------------------------------------------------------
// Copywrite Luke Titley 2015
//------------------------------------------------------------------------------
#ifndef TC_RANDOM
#define TC_RANDOM

namespace tc
{
//------------------------------------------------------------------------------
// generateRandomFloat
//------------------------------------------------------------------------------
/// \brief Generates a random floating point value within the given range.
/// \param lowerRange The lower number in the chosen range.
/// \param upperRange The higher number in the chosen range.
/// \return A random floating point value within the given range.
/// \code
/// float x = generateRandomFloat(10.0f, 20.0f); // x is in the range 10 to 20
/// \endcode
//------------------------------------------------------------------------------
float generateRandomFloat(const float lowerRange, const float upperRange);

}  // namespace tc
#endif  // TC_RANDOM
