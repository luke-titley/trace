//------------------------------------------------------------------------------
// Copywrite Luke Titley 2015
//------------------------------------------------------------------------------
#ifndef TC_KDTREE
#define TC_KDTREE
//------------------------------------------------------------------------------
#include "trace/assert.h"
#include "trace/bounds.h"
#include "trace/constvector.h"
#include "trace/vector.h"
//------------------------------------------------------------------------------
#include <cstdlib>
#include <vector>

namespace tc
{

class Ray;

typedef size_t KDTree_PrimitiveId;
typedef std::vector<KDTree_PrimitiveId> KDTree_PrimitiveIds;
typedef std::vector<char> KDTree_Nodes;

//------------------------------------------------------------------------------
// KDTree_Entry
//------------------------------------------------------------------------------
/// \cond
class KDTree_Entry
{
public:
    inline KDTree_Entry(const BoundsF& bounds,
                        const KDTree_PrimitiveId primitiveId);
    inline const Vector3<float>& getMax() const;
    inline const Vector3<float>& getMin() const;
    inline KDTree_PrimitiveId getPrimitiveId() const;

private:
    Vector3<float> m_max;
    Vector3<float> m_min;
    KDTree_PrimitiveId m_primitiveId;
};

//------------------------------------------------------------------------------
KDTree_Entry::KDTree_Entry(const BoundsF& bounds,
                           const KDTree_PrimitiveId primitiveId)
    : m_max(bounds.m_max), m_min(bounds.m_min), m_primitiveId(primitiveId)
{
}

//------------------------------------------------------------------------------
const Vector3<float>& KDTree_Entry::getMax() const
{
    return m_max;
}

//------------------------------------------------------------------------------
const Vector3<float>& KDTree_Entry::getMin() const
{
    return m_min;
}

//------------------------------------------------------------------------------
KDTree_PrimitiveId KDTree_Entry::getPrimitiveId() const
{
    return m_primitiveId;
}
/// \endcond

//------------------------------------------------------------------------------
// KDTree_Node
//------------------------------------------------------------------------------
/// \cond
class KDTree_Node
{
    friend class KDTree_Node_Impl;

private:
    enum
    {
        kLeaf = 3,
        kAxisBits = 3
    };

    /// The last two bits are for:
    /// [00] = x axis
    /// [01] = y axis
    /// [10] = z axis
    /// [11] = Leaf node (no axis)
    ///
    /// The rest of the bits store the index of the right child of this node (if
    /// this node is a branch in the tree).
    unsigned int m_flags;

    union
    {
        float m_location;
        unsigned int m_primitiveCount;
    };

public:
    inline KDTree_Node() : m_flags(0), m_primitiveCount(0)
    {
    }
};
/// \endcond

//------------------------------------------------------------------------------
// KDTree_SearchCache_StackFrame
//------------------------------------------------------------------------------
/// \cond
class KDTree_SearchCache_StackFrame
{
    friend class KDTree;

private:
    const size_t m_nodeIndex;
    const BoundsF m_bounds;

    KDTree_SearchCache_StackFrame(const size_t nodeIndex, const BoundsF& bounds)
        : m_nodeIndex(nodeIndex), m_bounds(bounds)
    {
        assert(bounds.m_min != bounds.m_max);
    }
};
/// \endcond

//------------------------------------------------------------------------------
// KDTree_SearchCache
//------------------------------------------------------------------------------
/// \brief A reusable structure that is populated when searching the
/// tc::KDTree for ray intersections.
///
/// The search results are stored in the tc::KDTree::SearchCache and can be
/// returned with tc::KDTree::SearchCache::getPrimitiveIds.
//------------------------------------------------------------------------------
class KDTree_SearchCache
{
    friend class KDTree;

private:
    inline void clear();

private:
    typedef ConstVector<KDTree_SearchCache_StackFrame> Stack;
    Stack m_stack;
};

//------------------------------------------------------------------------------
void KDTree_SearchCache::clear()
{
    m_stack.clear();
}

//------------------------------------------------------------------------------
class KDTree_TraceResult
{
public:
    /// \brief How far along the ray the element was hit.
    const float m_distanceAlongRay;
    /// \brief The element that has been hit.
    const size_t m_elementIndex;

    KDTree_TraceResult(const float distanceAlongRay, const size_t elementIndex)
        : m_distanceAlongRay(distanceAlongRay), m_elementIndex(elementIndex)
    {
    }
};

//------------------------------------------------------------------------------
// KDTree_PrimitiveIntersect
//------------------------------------------------------------------------------
class KDTree_PrimitiveIntersect
{
public:
    virtual bool intersect(float& resultDelta, const Ray& ray,
                           const size_t primitiveId) const = 0;
};

//------------------------------------------------------------------------------
// KDTree
//------------------------------------------------------------------------------
/// \brief
/// Implements a KD tree structure. For quick lookup of primtives that intersect
/// a given ray.
///
/// The implementation is based on the one proposed by
/// \cite Matt Pharr and Greg Humphreys in Physically Based Rendering : From
/// Theory To Implementation.
///
/// The tree works only with bounding volumes and so a data type must be
/// specified for identifying primitives. The tree must be sorted after calls to
/// 'addEntry'.
///
/// For each node a split is found by searching along the longest AABB length
/// for a position that has the lowest estimated traversal cost.  There is
/// potential for improvement in build speed at the cost of traversal
/// performance by using binning.
///
/// \usage Sorting is costly.
/// 'findEntriess' is thread safe, 'addEntry' and 'sortTree' are not.
/// Calls to 'findEntries' must provide a 'KDTree_SearchCache', which is a
/// structure that must be instantiated for each thread. The
/// 'KDTree_SearchCache' can be re-used for multiple calls to 'findEntries', but
/// 'KDTree_SearchCache' instances cannot be shared across threads.
/// The 'KDTree_SearchCache' instance contains the search results.
///
/// <b>Example</b>
/// \snippet test_kdtree.cpp test_kdtree two spheres
//------------------------------------------------------------------------------
class KDTree
{
    friend class KDTree_Impl;
    friend class SortStackFrame;

public:
    KDTree();

    /// \name Searching the Tree
    /// \{

    /// \brief Searches for bounding volumes in this tree which intersect the
    /// given ray.
    /// \param searchCache[out]: Populated with the primitive ids of primitives
    /// whose bounding volumes intersect the given ray. Also stores temporary
    /// memory needed when searching the KDTree.
    /// \param ray[in]: The ray that will be tested for intersections.
    /// \param primitiveTest[in]: The actual primitive intersection test. This
    /// contains a triangle intersections method, or a sphere intersection
    /// method for particles.
    /// \usage This method is thread safe but there must be one
    /// tc::KDTree_SearchCache instance per thread accessing the KDTree.
    KDTree_TraceResult findEntries(
        KDTree_SearchCache& searchCache, const Ray& ray,
        const KDTree_PrimitiveIntersect& primtiveTest) const;
    /// \}

    /// \name Building the Tree
    /// \{

    /// \brief Appends a new entry to this tc::KDTree instance. The entry will
    /// not be added to the internal tree structure until tc::KDTree::sortTree
    /// is called.
    /// \param bounds[in]: The bounding box of the primitive to add to the tree.
    /// \param primitiveId[in]: An index for the primitive associated with the
    /// given bounding volume. This is index will be returned in the search
    /// results if ever a ray intersects with the bounding box for this
    /// primitive.
    /// \usage This method is not thread safe.
    void addEntry(const BoundsF& bounds, const KDTree_PrimitiveId primitiveId);

    /// \brief Organises all primitive entries added with tc::KDTree::addEntry
    /// into a tree structure, for efficient ray intersection testing.
    /// \usage This method is not thread safe.
    void sortTree();
    /// \}

    /// \brief Converts this tc::KDTree instance into a human readable string,
    /// for inspection.
    /// \usage This method is thread safe.
    operator const std::string() const;

    /// \brief Produce a string respresentation that can be included in an obj
    /// file. Useful for debugging. We can write the kdtree out as an obj and
    /// then overlay it on top of the an original file.
    /// \return A string respresentation that can be included in an obj file.
    std::string toObj() const;

private:
    typedef std::vector<KDTree_Entry> Entries;

    BoundsBuilderF m_boundsBuilder;
    Entries m_entries;
    KDTree_Nodes m_nodes;
};

//------------------------------------------------------------------------------
// Runs all the unit tests for the 'kdtree' header file.
void kdtreeRunUnitTests(const tc::LogContext& logContext);

}  // namespace tc

#endif  // TC_KDTREE
