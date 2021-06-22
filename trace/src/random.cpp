//------------------------------------------------------------------------------
// Copywrite Luke Titley 2015
//------------------------------------------------------------------------------
#include "trace/random.h"
//------------------------------------------------------------------------------
#include <cstdlib>

namespace tc
{
float generateRandomFloat(const float lowerRange, const float upperRange)
{
    const float range = upperRange - lowerRange;
    const float randomFloatMax = static_cast<float>(RAND_MAX);
    const float randomFloat = static_cast<float>(rand());
    const float randomFloatInRange =
        lowerRange + ((randomFloat / randomFloatMax) * range);
    return randomFloatInRange;
}
}  // namespace tc
