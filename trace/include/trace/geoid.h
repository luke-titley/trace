//------------------------------------------------------------------------------
// Copywrite Luke Titley 2015
//------------------------------------------------------------------------------
#ifndef TC_GEOID
#define TC_GEOID
//------------------------------------------------------------------------------
#include <cstdlib>

namespace tc
{

//------------------------------------------------------------------------------
// GeoID
//------------------------------------------------------------------------------
///\brief Represents a unique item of geometry in the scene.
//------------------------------------------------------------------------------
class GeoID
{
public:
    /// Refers to a unique primitive instance, for example a polymesh or a
    /// pointcloud.
    ///
    /// All valid indices start from 1.
    /// An index of 0 means no objectIndex.
    const size_t m_objectIndex;

    /// Refers to a sub primitive of the primitive instance refered to by
    /// m_objectIndex, such as a triangle inside a polymesh, or a point inside a
    /// point cloud.
    ///
    /// Unlike the objectIndex, all valid element indices start from 0.
    const size_t m_elementIndex;

    /// Initializes a GeoID.
    /// \param objectIndex Refers to a unique primitive instance, for example a
    /// polymesh or a pointcloud.
    /// \param elementIndex Refers to a sub primitive of the primitive instance
    /// referred to by m_objectIndex, such as a triangle inside of a polymesh or
    /// a point inside a point cloud.
    GeoID(const size_t objectIndex, const size_t elementIndex)
        : m_objectIndex(objectIndex), m_elementIndex(elementIndex)
    {
    }
};

}  // namespace tc
#endif  // TC_GEOID
