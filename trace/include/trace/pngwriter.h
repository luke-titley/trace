//------------------------------------------------------------------------------
// Copywrite Luke Titley 2015
//------------------------------------------------------------------------------
#ifndef TC_PNGWRITER
#define TC_PNGWRITER
//------------------------------------------------------------------------------
#include "trace/array.h"

namespace tc
{

//------------------------------------------------------------------------------
// savePNG
//------------------------------------------------------------------------------
/// \brief Converts the given image from floating point pixel values to integer
/// 8 bit pixel values and writes it to disk as a png file.
/// \param array The four channel (RGBA) 32 bit floating point image.
/// \param filename The path of the file we will write to.
//------------------------------------------------------------------------------
void savePNG(const Array<float, 4>& array, const char* filename);

}  // namespace tc
#endif  // TC_PNGWRITER
