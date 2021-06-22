//------------------------------------------------------------------------------
// Copywrite Luke Titley 2015
//------------------------------------------------------------------------------
#include "trace/pngwriter.h"
//------------------------------------------------------------------------------
#include "png.h"
//------------------------------------------------------------------------------
#include <vector>
//------------------------------------------------------------------------------

namespace tc
{

typedef unsigned char byte;

byte floatTo8Bit(float v)
{
    // Clamp v to the 0-1 range
    v = v < 1.0f ? v : 1.0f;
    v = v > 0.0f ? v : 0.0f;
    return static_cast<byte>(v * 255.0f);
}

void savePNG(const Array<float, 4>& array, const char* filename)
{
    png_structp pngWrite =
        png_create_write_struct(PNG_LIBPNG_VER_STRING, png_voidp_NULL,
                                png_error_ptr_NULL, png_error_ptr_NULL);

    png_infop pngInfo = png_create_info_struct(pngWrite);

    FILE* out = fopen(filename, "w");

    png_init_io(pngWrite, out);

    // Default zlib compression
    //
    png_set_compression_level(pngWrite, 0x0006);
    png_set_compression_strategy(pngWrite, Z_DEFAULT_STRATEGY);

    // Setup the png info, width, height etc
    //
    const int BITDEPTH_8 = 8;
    png_set_IHDR(pngWrite, pngInfo, array.getWidth(), array.getHeight(),
                 BITDEPTH_8, PNG_COLOR_TYPE_RGBA, PNG_INTERLACE_NONE,
                 PNG_COMPRESSION_TYPE_BASE, PNG_FILTER_TYPE_BASE);

    // Write the info block out
    //
    png_write_info(pngWrite, pngInfo);

    // A buffer for writing out png data row by row.
    std::vector<byte> row;
    row.resize(array.getWidth() * 4);

    // Write out each row
    //
    for (size_t y = 0; y != array.getHeight(); ++y)
    {
        // Convert the pixels to 8 bit
        for (size_t x = 0; x != array.getWidth(); ++x)
        {
            enum
            {
                R = 0,
                G = 1,
                B = 2,
                A = 3
            };
            row[x * 4 + R] = floatTo8Bit(array.getValue(x, y, R));
            row[x * 4 + G] = floatTo8Bit(array.getValue(x, y, G));
            row[x * 4 + B] = floatTo8Bit(array.getValue(x, y, B));
            row[x * 4 + A] = floatTo8Bit(array.getValue(x, y, A));
        }

        // Write them out by row
        png_write_row(pngWrite, const_cast<byte*>(&row[0]));
    }

    // Finish writing
    // the image out
    //
    png_write_end(pngWrite, pngInfo);

    fclose(out);

    // Cleanup
    png_destroy_write_struct(&pngWrite, &pngInfo);

    return;
}

}  // namespace tc
