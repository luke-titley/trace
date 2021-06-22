//------------------------------------------------------------------------------
// Copywrite Luke Titley 2015
//------------------------------------------------------------------------------
#ifndef TC_ARRAY
#define TC_ARRAY
//------------------------------------------------------------------------------
#include "trace/assert.h"
#include "trace/log.h"
#include "trace/test.h"
//------------------------------------------------------------------------------
#include <cstddef>
#include <algorithm>
//------------------------------------------------------------------------------

namespace tc
{

//------------------------------------------------------------------------------
// Array
//------------------------------------------------------------------------------
/// \brief Contains a grid of values where the width and the height of the grid
/// are specified in the Array constructor.
///
/// The type stored in each cell must be specified by the user as a template
/// parameter, and must be a POD, that is it should not aquire any new resources
/// in its constructor.
/// The depth must also be specified by the user as a template parameter. Array
/// is intended for storing a pixel map where 'depth' specifies the pixel depth.
/// For example RGB is D=3, BW is D=1 and RGBA is D=4, T can be float.
//------------------------------------------------------------------------------
template <typename T, size_t D>
class Array
{
private:
    size_t _width;
    size_t _height;
    T* _pixels;
    void operator=(const Array& src);

public:
    /// \brief Initializes a new Array.
    /// \param width: The number of entries for the x axis of this array.
    /// \param height: The number of entries for the y axis of this array.
    /// \param default_value: The initial value given to every cell in the
    /// array.
    Array(size_t width, size_t height, const T default_value = 0)
        : _width(width), _height(height), _pixels(0)
    {
        size_t size = width * height * D;
        _pixels = new T[size];

        // Set to a default value
        for (size_t i = 0; i != size; ++i)
        {
            new (&_pixels[i]) T(default_value);
        }
    }

    /// \brief Initializes a new array by copying the contents of the given
    /// array.
    /// \param src: The array who's contents will be copied.
    Array(const Array& src)
        : _width(src._width), _height(src._height), _pixels(0)
    {
        size_t size = _width * _height * D;
        _pixels = new T[size];
        std::copy(src._pixels, src._pixels + size, _pixels);
    }

    ~Array()
    {
        delete[] _pixels;
    }

    /// \usage This method is thread safe.
    /// \return The number of channels in this array.
    inline size_t getDepth() const
    {
        return D;
    }

    /// \usage This method is thread safe.
    /// \return The length along the x axis for this array.
    inline size_t getWidth() const
    {
        return _width;
    }

    /// \usage This method is thread safe.
    /// \return The length along the y axis for this array.
    inline size_t getHeight() const
    {
        return _height;
    }

    /// \usage This method is thread safe.
    /// \return The total number of cells in the array.
    inline size_t getSize() const
    {
        return _height * _width;
    }

    /// \brief Sets a cell specified by x, y and channel to the given value.
    /// \param x: The x coordinate of the cell.
    /// \param y: The y coordinate of the cell.
    /// \param channel: The channel ie R, G, B, A that will be set.
    /// \param value: The value to set the cells given channel to.
    inline void setValue(size_t x, size_t y, size_t channel, const T& value)
    {
        assert(x < _width);
        assert(y < _height);
        assert(channel < D);
        new (&_pixels[(y * _width * D) + (x * D) + channel]) T(value);
    }

    /// \param x: The x coordinate of the cell.
    /// \param y: The y coordinate of the cell.
    /// \param channel: The channel ie R, G, B, A that will be got.
    /// \return The value of the given channel for the cell specified by x and
    /// y.
    inline const T& getValue(size_t x, size_t y, size_t channel) const
    {
        assert(x < _width);
        assert(y < _height);
        assert(channel < D);
        return _pixels[(y * _width * D) + (x * D) + channel];
    }

    /// \param x: The x coordinate of the cell.
    /// \param y: The y coordinate of the cell.
    /// \param channel: The channel ie R, G, B, A that will be got.
    /// \return The value of the given channel for the cell specified by x and
    /// y.
    inline T& getValue(size_t x, size_t y, size_t channel)
    {
        assert(x < _width);
        assert(y < _height);
        assert(channel < D);
        return _pixels[(y * _width * D) + (x * D) + channel];
    }

    /// \return A const pointer to the row data, for the row specified by 'row'.
    /// \param row: The index of the row to return.
    inline const T* getRow(size_t row) const
    {
        return &_pixels[row * _width * D];
    }

    /// \brief Sets every cell in the array to the channel values in 'pixel'.
    /// \param pixel: The channel values to assign to every cell in the array.
    void fill(const T pixel[D])
    {
        for (size_t i = 0; i != _width * _height * D; i += D)
        {
            for (size_t j = 0; j != D; ++j)
            {
                _pixels[i + j] = pixel[j];
            }
        }
    }
};

//------------------------------------------------------------------------------
// Runs all the unit tests for the 'array' header file.
void arrayRunUnitTests(const tc::LogContext& logContext);

}  // namespace Array

#endif  // TC_ARRAY
