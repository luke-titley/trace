//------------------------------------------------------------------------------
// Copywrite Luke Titley 2015
//------------------------------------------------------------------------------
#ifndef TC_IMAGE
#define TC_IMAGE
//------------------------------------------------------------------------------
#include <cstdlib>
//------------------------------------------------------------------------------
#include "trace/vector.h"

namespace tc
{
//------------------------------------------------------------------------------
// Image
//------------------------------------------------------------------------------
class Image
{
public:
    virtual size_t getHeight() const = 0;
    virtual size_t getWidth() const = 0;
    virtual void setPixel(const size_t x, const size_t y,
                          const Vector3<float> & pixel) = 0;
    virtual void getPixel(const size_t x, const size_t y,
                          Vector3<float> & pixel) const = 0;
};

}  // namespace tc
#endif
