//------------------------------------------------------------------------------
// Copywrite Luke Titley 2015
//------------------------------------------------------------------------------
#include "trace/log.h"
#include "trace/test.h"
#include "trace/array.h"
//------------------------------------------------------------------------------

namespace
{

//------------------------------------------------------------------------------
void constructor(const tc::LogContext& logContext)
{
    /// [test_array constructor]
    const size_t width = 100;
    const size_t height = 100;
    const float default_value = 1.0f;
    const size_t D = 4;
    tc::Array<float, D> a(width, height, default_value);

    for (size_t y = 0; y != height; ++y)
    {
        for (size_t x = 0; x != width; ++x)
        {
            // Channels
            for (size_t c = 0; c != D; ++c)
            {
                float value = a.getValue(x, y, c);
                TC_IS(logContext, value == default_value);
            }
        }
    }
    /// [test_array constructor]
}

//------------------------------------------------------------------------------
void getSetValue(const tc::LogContext& logContext)
{
    /// [test_array getSetValue]

    enum
    {
        R = 0,
        G = 1,
        B = 2,
        A = 3
    };
    size_t width = 100;
    size_t height = 100;
    const size_t D = 4;
    tc::Array<float, D> array(width, height);

    // Set the pixel value
    for (size_t y = 0; y != height; ++y)
    {

        for (size_t x = 0; x != width; ++x)
        {
            array.setValue(x, y, R, 1.0f);
            array.setValue(x, y, G, 0.0f);
            array.setValue(x, y, B, 0.0f);
            array.setValue(x, y, A, 1.0f);
        }
    }

    // Get the pixel value
    for (size_t y = 0; y != height; ++y)
    {

        for (size_t x = 0; x != width; ++x)
        {
            const float r = array.getValue(x, y, R);
            const float g = array.getValue(x, y, G);
            const float b = array.getValue(x, y, B);
            const float a = array.getValue(x, y, A);
            TC_IS(logContext, r == 1.0f);
            TC_IS(logContext, g == 0.0f);
            TC_IS(logContext, b == 0.0f);
            TC_IS(logContext, a == 1.0f);
        }
    }
    /// [test_array getSetValue]
}

//------------------------------------------------------------------------------
void getters(const tc::LogContext& logContext)
{
    /// [test_array getters]
    enum
    {
        R = 0,
        G = 1,
        B = 2,
        A = 3
    };

    const size_t width = 100;
    const size_t height = 100;
    const size_t D = 4;
    const tc::Array<float, D> array(width, height, 1.0f);
    const float* second_row = array.getRow(1);

    // width/height/depth
    TC_IS(logContext, array.getWidth() == width);
    TC_IS(logContext, array.getHeight() == height);
    TC_IS(logContext, array.getSize() == width * height);

    // row
    for (size_t i = 0; i != width * D; i += D)
    {
        const float r = second_row[i + 0];
        const float g = second_row[i + 1];
        const float b = second_row[i + 2];
        const float a = second_row[i + 3];
        TC_IS(logContext, r == 1.0f);
        TC_IS(logContext, g == 1.0f);
        TC_IS(logContext, b == 1.0f);
        TC_IS(logContext, a == 1.0f);
    }
    /// [test_array getters]
}

//------------------------------------------------------------------------------
void fill(const tc::LogContext& logContext)
{
    /// [test_array fill]
    enum
    {
        R = 0,
        G = 1,
        B = 2,
        A = 3
    };

    const size_t width = 100;
    const size_t height = 100;
    const size_t D = 4;
    tc::Array<float, D> array(width, height, 1.0f);

    const float color[4] = {1024.0f, 0.0f, 0.0f, 1.0f};

    array.fill(color);

    for (size_t y = 0; y != height; ++y)
    {
        for (size_t x = 0; x != width; ++x)
        {
            const float r = array.getValue(x, y, R);
            const float g = array.getValue(x, y, G);
            const float b = array.getValue(x, y, B);
            const float a = array.getValue(x, y, A);
            TC_IS(logContext, r == 1024.0f);
            TC_IS(logContext, g == 0.0f);
            TC_IS(logContext, b == 0.0f);
            TC_IS(logContext, a == 1.0f);
        }
    }
    /// [test_array fill]
}
}  // namespace

//------------------------------------------------------------------------------
void tc::arrayRunUnitTests(const tc::LogContext& logContext)
{
    constructor(logContext);
    getSetValue(logContext);
    getters(logContext);
    fill(logContext);
}
