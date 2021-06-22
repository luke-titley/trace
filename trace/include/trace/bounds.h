//------------------------------------------------------------------------------
// Copywrite Luke Titley 2015
//------------------------------------------------------------------------------
#ifndef TC_BOUNDS_H
#define TC_BOUNDS_H
//------------------------------------------------------------------------------
#include "trace/assert.h"
#include "trace/log.h"
#include "trace/ray.h"
#include "trace/test.h"
#include "trace/vector.h"
//------------------------------------------------------------------------------
#include <cfloat>
#include <cstdlib>
#include <sstream>
#include <string>
//------------------------------------------------------------------------------
namespace tc
{

//------------------------------------------------------------------------------
// Pair
//------------------------------------------------------------------------------
/// \brief A pair of values, of any type.
//------------------------------------------------------------------------------
template <typename T>
class Pair
{
public:
    /// \brief The left part of this pair.
    const T m_left;
    /// \brief The right part of this pair.
    const T m_right;

    /// \brief Provides an array interface for the items.
    const T & operator[](const size_t index) const
    {
        assert(index == 0 || index == 1);
        switch(index)
        {
        case 0:
            return m_left;
        case 1:
            return m_right;
        }
        assert(false);// We should never get here
        return m_left;
    }

    /// Initialises a Pair with the provided parts.
    /// \param left: The left part.
    /// \param right: The right part.
    inline Pair(const T& left, const T& right) : m_left(left), m_right(right)
    {
    }
    inline Pair(const Pair& src) : m_left(src.m_left), m_right(src.m_right)
    {
    }
};

//------------------------------------------------------------------------------
// BoundsBuilder
//------------------------------------------------------------------------------
/// \brief This is a utility for building bounding volumes.
///
/// Use the BoundsBuilder for incrementally expanding a bounding volume.
/// Once fully expanded, pass the BoundsBuilder into the constructor of a Bounds
/// instance.
/// \usage The 'Bounds' class is imutable. This is the best way to incrementally
/// build a bounding box.
//------------------------------------------------------------------------------
template <typename T>
class BoundsBuilder
{
    Vector3<T> m_min;
    Vector3<T> m_max;
    bool m_valid;

public:
    inline BoundsBuilder() : m_valid(false)
    {
    }

    /// \brief If the given point is not inside of this bounding box,
    /// expandBounds moves the extents of this bounding box to ensure that the
    /// point is.
    /// \param point:A point which may or may not be inside of the bounding
    /// volume.
    /// \param epsilon: The degree of accuracy with which the point is tested
    /// for being inside of the bounding volume.
    inline void expandBounds(const Vector3<T>& point, T epsilon = 0.001f)
    {
        if (m_valid)
        {
            // Expand bounds Min
            m_min.x = m_min.x < point.x ? m_min.x : point.x - epsilon;
            m_min.y = m_min.y < point.y ? m_min.y : point.y - epsilon;
            m_min.z = m_min.z < point.z ? m_min.z : point.z - epsilon;

            // Expand bounds Max
            m_max.x = m_max.x > point.x ? m_max.x : point.x + epsilon;
            m_max.y = m_max.y > point.y ? m_max.y : point.y + epsilon;
            m_max.z = m_max.z > point.z ? m_max.z : point.z + epsilon;
        }
        else
        {
            m_valid = true;
            m_min = (point - epsilon).overwrite(Vector3<T>::kW, 0.0f);
            m_max = (point + epsilon).overwrite(Vector3<T>::kW, 0.0f);
        }
    }

    /// The minimum extent for the bounding volume being built.
    /// \return a tc::Vector of x,y,z values containing the minimum extent for
    /// the bounding volume being built.
    inline Vector3<T> getMin() const
    {
        return m_min;
    }

    /// The maximum extent for the bounding volume being built.
    /// \return a tc::Vector of x,y,z values containing the maximum extent for
    /// the bounding volume being built.
    inline Vector3<T> getMax() const
    {
        return m_max;
    }
};

//------------------------------------------------------------------------------
// Bounds
//------------------------------------------------------------------------------
/// \brief Stores a min and max vector which describe the general spacial extent
/// of a piece of geometry.
//------------------------------------------------------------------------------
template <typename T>
class Bounds
{
public:
    /// The minimum extent. The top, right, back corner of the bounding box.
    const Vector3<T> m_min;
    /// The maximum extent. The bottom, left, front corner of the bounding box.
    const Vector3<T> m_max;

    /// \brief Initialises a tc::Bounds instance, with the extents from the
    /// given tc::BoundsBuilder.
    inline Bounds(const BoundsBuilder<T>& boundsBuilder)
        : m_min(boundsBuilder.getMin()), m_max(boundsBuilder.getMax())
    {
    }

    /// \brief Initialises a tc::bounds instance, with the extents provided as
    /// 'min' and 'max'.
    /// \param min: The smallest point in the bounds.
    /// \param max: The largest point in the bounds.
    inline Bounds(const Vector3<T>& min, const Vector3<T>& max)
        : m_min(min), m_max(max)
    {
    }

    /// \brief Tests whether the given 'bounds' class instance is inside of this
    /// bounding volume.
    /// \param bounds: The boundng volume to test against.
    /// \return true if 'bounds' is all inside this bounding volume, false if it
    /// is not.
    inline bool contains(const Bounds& bounds) const
    {
        return bounds.m_min > m_min && bounds.m_max < m_max;
    }

    /// \brief Tests whether the given 'point' is inside of this boundng volume.
    /// \param point: The point to test against.
    /// \return true if 'point' is inside this bounding volume, false if it is
    /// not.
    inline bool contains(const Vector3<T>& point) const
    {
        return point > m_min && point < m_max;
    }

    /// \brief Tests whether the given 'point' is inside of this boundng volume,
    /// axis, specified by axis.
    /// \param point: The point to test against.
    /// \param axis: The axis to test against.
    /// \return true if 'point' is inside this bounding volume, false if it is
    /// not.
    inline bool contains(T point, size_t axis) const
    {
        return point > m_min[axis] && point < m_max[axis];
    }

    /// \brief Tests whether the given 'point' is inside of this boundng volume,
    /// or if it touches this bounding volume.
    /// \param point: The point to test against.
    /// \return true if 'point' is inside, or touches this bounding volume,
    /// false if not.
    inline bool containsOrTouches(const Vector3<T>& point) const
    {
        return point >= m_min && point <= m_max;
    }

    /// \brief Tests if rhs and *this are equal.
    /// \param rhs: The right hand side 'Bounds' instance.
    /// \param epsilon: The degree of accuracy with which the point is tested
    /// for being inside of the bounding volume.
    /// \return True if the rhs bounding box has the same minimum and maximum
    /// extent. False if not.
    inline bool equals(const Bounds& rhs, const T epsilon = 0.001f) const
    {
        return rhs.m_min.equals(m_min, epsilon) &&
               rhs.m_max.equals(m_max, epsilon);
    }

    /// \return The surface area of the bounding box.
    inline T computeSurfaceArea() const
    {
        return (m_max - m_min).surfaceArea();
    }

    /// \brief Computes the width and weight of the bounding box as a Vector3.
    /// \return The width and weight of the bounding box as a Vector3.
    inline Vector3<T> computeDimensions() const
    {
        const Vector3<T> result = m_max - m_min;
        return result;
    }

    /// \return The boolean intersection of this bounding box with the
    /// one given by rhs.
    inline Bounds intersection(const Bounds& rhs) const
    {
        Vector3<T> min(m_min);
        Vector3<T> max(m_max);
        for (size_t i = 0; i != 3; ++i)
        {
            // The largest min
            if (m_min[i] < rhs.m_min[i])
            {
                min[i] = rhs.m_min[i];
            }
            // The smallest max
            if (m_max[i] > rhs.m_max[i])
            {
                max[i] = rhs.m_max[i];
            }
        }
        return Bounds(min, max);
    }

    /// \brief Breaks the bounding volume into two seperate pieces.
    /// The pieces can be retrieved from the returned tc::Pair<Bounds>.
    /// \param axis: The axis to split.
    /// \param location: The position along the axis to split.
    /// \return A 'BoundsPair' containing the left and right child bounding
    /// volumes.
    inline Pair<Bounds> split(size_t axis, T location) const;

    /// \return A string representation of this bounding volume.
    inline operator std::string() const
    {
        std::stringstream ss;
        ss << "(" << std::string(m_min) << "),";
        ss << "(" << std::string(m_max) << ")";
        return ss.str();
    }

    /// \brief Produce a string respresentation that can be included in an obj
    /// file. Useful for debugging. We can write the bounding box out as an obj
    /// and then overlay it on top of the an original file.
    /// \param index: The index of the OBJ object this function will be
    /// producing. This is to ensure that the vertex indicies are correct.
    /// \return A string respresentation that can be included in an obj file.
    inline std::string toObj(const size_t index) const
    {
        const size_t offset = index * 8;

        std::stringstream ss;
        // Verticies bottom
        ss << "v " << m_min[0] << " " << m_min[1] << " " << m_min[2]
           << std::endl;
        ss << "v " << m_min[0] << " " << m_min[1] << " " << m_max[2]
           << std::endl;
        ss << "v " << m_max[0] << " " << m_min[1] << " " << m_max[2]
           << std::endl;
        ss << "v " << m_max[0] << " " << m_min[1] << " " << m_min[2]
           << std::endl;
        // Verticies top
        ss << "v " << m_min[0] << " " << m_max[1] << " " << m_min[2]
           << std::endl;
        ss << "v " << m_min[0] << " " << m_max[1] << " " << m_max[2]
           << std::endl;
        ss << "v " << m_max[0] << " " << m_max[1] << " " << m_max[2]
           << std::endl;
        ss << "v " << m_max[0] << " " << m_max[1] << " " << m_min[2]
           << std::endl;
        // Lines Bottom
        ss << "l " << offset + 1 << " " << offset + 2 << std::endl;
        ss << "l " << offset + 2 << " " << offset + 3 << std::endl;
        ss << "l " << offset + 3 << " " << offset + 4 << std::endl;
        ss << "l " << offset + 4 << " " << offset + 1 << std::endl;
        // Lines Top
        ss << "l " << offset + 5 << " " << offset + 6 << std::endl;
        ss << "l " << offset + 6 << " " << offset + 7 << std::endl;
        ss << "l " << offset + 7 << " " << offset + 8 << std::endl;
        ss << "l " << offset + 8 << " " << offset + 5 << std::endl;
        // Linking lines
        ss << "l " << offset + 1 << " " << offset + 5 << std::endl;
        ss << "l " << offset + 2 << " " << offset + 6 << std::endl;
        ss << "l " << offset + 3 << " " << offset + 7 << std::endl;
        ss << "l " << offset + 4 << " " << offset + 8 << std::endl;
        return ss.str();
    }
};

//------------------------------------------------------------------------------
template <typename T>
inline Pair<Bounds<T> > Bounds<T>::split(size_t axis, T location) const
{
    assert(axis < 3);  // Ensure the axis is X, Y or Z;
    assert(location >= m_min[axis] &&
           location <= m_max[axis]);  // Only split within range

    Vector3<T> left = m_max;
    Vector3<T> right = m_min;

    left[axis] = location;
    right[axis] = location;

    return Pair<Bounds<T> >(Bounds(m_min, left), Bounds(right, m_max));
}

//------------------------------------------------------------------------------
/// \{
/// \brief tc::Bounds<float> is the most comonly used
typedef Bounds<float> BoundsF;
typedef BoundsBuilder<float> BoundsBuilderF;
/// \}

//------------------------------------------------------------------------------
// Runs all the unit tests for the 'bounds' header file.
void boundsRunUnitTests(const tc::LogContext& logContext);

}  // namespace tc
#endif  // TC_BOUNDS_H
