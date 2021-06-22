//------------------------------------------------------------------------------
// Copywrite Luke Titley 2015
//------------------------------------------------------------------------------
#include "trace/kdtree.h"
//------------------------------------------------------------------------------
#include "trace/constvector.h"
#include "trace/intersect.h"
#include "trace/ray.h"
//------------------------------------------------------------------------------
#include <algorithm>
#include <cmath>
#include <iostream>
#include <numeric>
#include <sstream>
#include <stack>

namespace tc
{

namespace
{

//------------------------------------------------------------------------------
// KDTree_Record
//------------------------------------------------------------------------------
struct KDTree_Record
{
    const size_t m_depth;
    const size_t m_node;
    const size_t m_parentAxis;
    KDTree_Record(const size_t depth, const size_t node,
                  const size_t parentAxis)
        : m_depth(depth), m_node(node), m_parentAxis(parentAxis)
    {
    }
    KDTree_Record(const KDTree_Record& record)
        : m_depth(record.m_depth),
          m_node(record.m_node),
          m_parentAxis(record.m_parentAxis)
    {
    }
};

//------------------------------------------------------------------------------
// KDTree_LocationAxisPair
//------------------------------------------------------------------------------
class KDTree_LocationAxisPair
{
public:
    const float m_location;
    const char m_axis;
    const bool m_shouldSplit;

    KDTree_LocationAxisPair(const float location, const char axis,
                            const bool shouldSplit)
        : m_location(location), m_axis(axis), m_shouldSplit(shouldSplit)
    {
    }

    KDTree_LocationAxisPair(const bool shouldSplit)
        : m_location(0.0f), m_axis(0), m_shouldSplit(shouldSplit)
    {
    }
};

//------------------------------------------------------------------------------
// KDTree_Edge
//------------------------------------------------------------------------------
class KDTree_Edge
{
public:
    enum Type
    {
        kStart = 0,
        kEnd = 1
    };

    Type m_type;
    float m_location;
    size_t m_primitiveIndex;

    KDTree_Edge(Type type, float location, size_t primitiveIndex)
        : m_type(type), m_location(location), m_primitiveIndex(primitiveIndex)
    {
    }

    bool operator<(const KDTree_Edge& rhs) const
    {
        if (m_location == rhs.m_location)
        {
            return m_type < rhs.m_type;
        }
        return m_location < rhs.m_location;
    }
};

typedef std::vector<KDTree_Edge> KDTree_Edges;

}  // namespace

//------------------------------------------------------------------------------
// KDTree_Node_Impl
//------------------------------------------------------------------------------
class KDTree_Node_Impl
{
public:
    inline static bool isLeaf(const KDTree_Node& node);
    inline static bool isBranch(const KDTree_Node& node);
    inline static size_t getAxis(const KDTree_Node& node);
    inline static float getLocation(const KDTree_Node& node);
    inline static unsigned int getPrimitiveCount(const KDTree_Node& node);
    inline static unsigned int getRight(const KDTree_Node& node);
    inline static void setAxis(KDTree_Node& node, const size_t axis);
    inline static void setLocation(KDTree_Node& node, const float location);
    inline static void setPrimitiveCount(KDTree_Node& node,
                                         unsigned int primitiveCount);
    inline static void setRight(KDTree_Node& node, unsigned int right);
    inline static void setIsLeaf(KDTree_Node& node);
    inline static const KDTree_Node& lookupNode(const KDTree_Nodes& nodes,
                                                const size_t nodeIndex);
    inline static KDTree_Node& lookupNode(KDTree_Nodes& nodes,
                                          const size_t nodeIndex);
    inline static size_t addBranchNode(KDTree_Nodes& nodes,
                                       const float location, const size_t axis);
    inline static size_t addLeafNode(KDTree_Nodes& nodes,
                                     const KDTree_PrimitiveIds& primitives);
    inline static void setRight(KDTree_Nodes& nodes, const size_t nodeIndex,
                                const size_t right);
    inline static size_t getLeft(const KDTree_Nodes& nodes,
                                 const size_t nodeIndex);
    inline static size_t getRight(const KDTree_Nodes& nodes,
                                  const size_t nodeIndex);
    inline static const size_t* getPrimitives(const KDTree_Nodes& nodes,
                                              const size_t nodeIndex);
};

//------------------------------------------------------------------------------
bool KDTree_Node_Impl::isLeaf(const KDTree_Node& node)
{
    return (node.m_flags & KDTree_Node::kAxisBits) == KDTree_Node::kLeaf;
}

//------------------------------------------------------------------------------
bool KDTree_Node_Impl::isBranch(const KDTree_Node& node)
{
    return !KDTree_Node_Impl::isLeaf(node);
}

//------------------------------------------------------------------------------
size_t KDTree_Node_Impl::getAxis(const KDTree_Node& node)
{
    assert(KDTree_Node_Impl::isBranch(node));
    return node.m_flags & KDTree_Node::kAxisBits;
}

//------------------------------------------------------------------------------
float KDTree_Node_Impl::getLocation(const KDTree_Node& node)
{
    assert(KDTree_Node_Impl::isBranch(node));
    return node.m_location;
}

//------------------------------------------------------------------------------
unsigned int KDTree_Node_Impl::getPrimitiveCount(const KDTree_Node& node)
{
    assert(KDTree_Node_Impl::isLeaf(node));
    return node.m_primitiveCount;
}

//------------------------------------------------------------------------------
unsigned int KDTree_Node_Impl::getRight(const KDTree_Node& node)
{
    assert(KDTree_Node_Impl::isBranch(node));
    return node.m_flags >> 2;
}

//------------------------------------------------------------------------------
void KDTree_Node_Impl::setAxis(KDTree_Node& node, const size_t axis)
{
    assert(KDTree_Node_Impl::isBranch(node));
    assert(axis <= 2);
    node.m_flags |= axis;
}

//------------------------------------------------------------------------------
void KDTree_Node_Impl::setLocation(KDTree_Node& node, const float location)
{
    assert(KDTree_Node_Impl::isBranch(node));
    node.m_location = location;
}

//------------------------------------------------------------------------------
void KDTree_Node_Impl::setPrimitiveCount(KDTree_Node& node,
                                         const unsigned int primitiveCount)
{
    assert(KDTree_Node_Impl::isLeaf(node));
    node.m_primitiveCount = primitiveCount;
}

//------------------------------------------------------------------------------
void KDTree_Node_Impl::setRight(KDTree_Node& node, unsigned int right)
{
    assert(KDTree_Node_Impl::isBranch(node));
    node.m_flags |= right << 2;
}

//------------------------------------------------------------------------------
void KDTree_Node_Impl::setIsLeaf(KDTree_Node& node)
{
    assert((node.m_flags & KDTree_Node::kAxisBits) == 0);
    node.m_flags |= KDTree_Node::kLeaf;
}

//------------------------------------------------------------------------------
const KDTree_Node& KDTree_Node_Impl::lookupNode(const KDTree_Nodes& nodes,
                                                const size_t nodeIndex)
{
    assert(nodeIndex < nodes.size());
    const KDTree_Node& node =
        *reinterpret_cast<const KDTree_Node*>(&nodes[nodeIndex]);
    return node;
}

//------------------------------------------------------------------------------
KDTree_Node& KDTree_Node_Impl::lookupNode(KDTree_Nodes& nodes,
                                          const size_t nodeIndex)
{
    assert(nodeIndex < nodes.size());
    KDTree_Node& node = *reinterpret_cast<KDTree_Node*>(&nodes[nodeIndex]);
    return node;
}

//------------------------------------------------------------------------------
size_t KDTree_Node_Impl::addBranchNode(KDTree_Nodes& nodes,
                                       const float location, const size_t axis)
{
    const size_t nodeIndex = nodes.size();
    nodes.resize(nodes.size() + sizeof(KDTree_Node));
    KDTree_Node& node = *reinterpret_cast<KDTree_Node*>(&nodes[nodeIndex]);
    node = KDTree_Node();
    KDTree_Node_Impl::setAxis(node, axis);
    KDTree_Node_Impl::setLocation(node, location);
    return nodeIndex;
}

//------------------------------------------------------------------------------
size_t KDTree_Node_Impl::addLeafNode(KDTree_Nodes& nodes,
                                     const KDTree_PrimitiveIds& primitives)
{
    const size_t nodeIndex = nodes.size();
    const size_t nodeSize = sizeof(KDTree_Node);
    const size_t primitivesSize = sizeof(size_t) * primitives.size();
    nodes.resize(nodes.size() + nodeSize + primitivesSize);
    KDTree_Node& node = *reinterpret_cast<KDTree_Node*>(&nodes[nodeIndex]);
    node = KDTree_Node();
    KDTree_Node_Impl::setIsLeaf(node);

    // Copy the primitive information into the primitives array
    if (primitivesSize != 0)
    {
        size_t* primitiveIndices = (size_t*)(&nodes[nodeIndex + nodeSize]);
        KDTree_Node_Impl::setPrimitiveCount(node, primitives.size());
        std::copy(primitives.begin(), primitives.end(), primitiveIndices);
    }
    return nodeIndex;
}

//------------------------------------------------------------------------------
void KDTree_Node_Impl::setRight(KDTree_Nodes& nodes, const size_t nodeIndex,
                                const size_t right)
{
    assert(KDTree_Node_Impl::isBranch(lookupNode(nodes, nodeIndex)));
    assert(right != 0);
    assert(right < (size_t)(2 << 30));  // We can only store indices smaller
                                        // than 2^30.
    KDTree_Node& node = lookupNode(nodes, nodeIndex);
    setRight(node, right);
}

//------------------------------------------------------------------------------
size_t KDTree_Node_Impl::getLeft(const KDTree_Nodes& nodes,
                                 const size_t nodeIndex)
{
    assert(KDTree_Node_Impl::isBranch(lookupNode(nodes, nodeIndex)));
    return nodeIndex + sizeof(KDTree_Node);
}

//------------------------------------------------------------------------------
size_t KDTree_Node_Impl::getRight(const KDTree_Nodes& nodes,
                                  const size_t nodeIndex)
{
    assert(KDTree_Node_Impl::isBranch(lookupNode(nodes, nodeIndex)));
    return KDTree_Node_Impl::getRight(lookupNode(nodes, nodeIndex));
}

//------------------------------------------------------------------------------
const size_t* KDTree_Node_Impl::getPrimitives(const KDTree_Nodes& nodes,
                                              const size_t nodeIndex)
{
    assert(KDTree_Node_Impl::getPrimitiveCount(
               KDTree_Node_Impl::lookupNode(nodes, nodeIndex)) != 0);

    const size_t nodeSize = sizeof(KDTree_Node);
    const size_t* primitiveIndices =
        (const size_t*)(&nodes[nodeIndex + nodeSize]);
    return primitiveIndices;
}

class SortStackFrame;

//------------------------------------------------------------------------------
// KDTree_Impl
//------------------------------------------------------------------------------
class KDTree_Impl
{
public:
    static KDTree_LocationAxisPair findLocationAndAxis(
        const KDTree::Entries& entries, const KDTree_PrimitiveIds& primitives,
        KDTree_Edges& edges, const BoundsF& bounds,
        const size_t intersectionCost = 80, const size_t traversalCost = 1);

    static void indent(std::stringstream& sstream, size_t depth);

    static void sortTree(const SortStackFrame& initial_frame);
};

//------------------------------------------------------------------------------
KDTree_LocationAxisPair KDTree_Impl::findLocationAndAxis(
    const KDTree::Entries& entries, const KDTree_PrimitiveIds& primitives,
    KDTree_Edges& edges, const BoundsF& bounds, const size_t intersectionCost,
    const size_t traversalCost)
{
    assert(!primitives.empty());
    edges.reserve(primitives.size() * 2);

    const float outerSurfaceArea = bounds.computeSurfaceArea();

    const float unsplitCost =
        static_cast<float>(intersectionCost * primitives.size());

    float bestCost = FLT_MAX;
    float bestLocation = 0.0f;
    char bestAxis = 0;
#if 0
    for (char axis = 0; axis != 3; ++axis)
    {
#else
    // Choose an axis
    char axis = 0;
    const Vector3<float> dimensions = bounds.computeDimensions();
    if (dimensions.x < dimensions.y && dimensions.z < dimensions.y)
    {
        axis = 1;
    }
    else if (dimensions.x < dimensions.z && dimensions.y < dimensions.z)
    {
        axis = 2;
    }
    {
#endif
    edges.clear();

    // Create our edges array
    //
    for (size_t i = 0; i != primitives.size(); ++i)
    {
        size_t primitiveIndex = primitives[i];
        const KDTree_Entry& entry = entries[primitiveIndex];
        const float start = entry.getMin()[axis];
        const float end = entry.getMax()[axis];
        edges.push_back(
            KDTree_Edge(KDTree_Edge::kStart, start, primitiveIndex));
        edges.push_back(KDTree_Edge(KDTree_Edge::kEnd, end, primitiveIndex));
    }

    // Sort it
    //
    std::sort(edges.begin(), edges.end());

    // Search for the best splitting location
    size_t primitivesBelowSplit = 0;  // TODO LT: Strictly speaking this
                                      // is not correct, but this is the
    // implementation from 'Photorealistic Rendering Techniques'.

    size_t primitivesAboveSplit = primitives.size();
    for (size_t i = 0; i != edges.size(); ++i)
    {
        const KDTree_Edge& edge = edges[i];
        if (bounds.contains(edge.m_location, (size_t)axis))
        {
            if (edges[i].m_type == KDTree_Edge::kEnd)
            {
                --primitivesAboveSplit;
                ++primitivesBelowSplit;
            }
            Pair<BoundsF> boundsPair = bounds.split(axis, edge.m_location);
            const float leftSurfaceArea =
                boundsPair.m_left.computeSurfaceArea();
            const float rightSurfaceArea =
                boundsPair.m_right.computeSurfaceArea();
            const float leftProbability = leftSurfaceArea / outerSurfaceArea;
            const float rightProbability = rightSurfaceArea / outerSurfaceArea;
            const float leftCost =
                leftProbability *
                static_cast<float>(primitivesBelowSplit * intersectionCost);
            const float rightCost =
                rightProbability *
                static_cast<float>(primitivesAboveSplit * intersectionCost);

#if 1
            const float bonus =
                (primitivesAboveSplit == 0 || primitivesBelowSplit == 0) ? 0.5f
                                                                         : 0.0f;
#else
                const float bonus = 0.0f;
#endif
            const float cost =
                traversalCost + (1.0f - bonus) * (leftCost + rightCost);

            if (cost < bestCost)
            {
                bestCost = cost;
                bestLocation = edge.m_location;
                bestAxis = axis;
            }
        }
    }
}

if (bestCost < unsplitCost)
{
    return KDTree_LocationAxisPair(bestLocation, bestAxis, true);
}

return KDTree_LocationAxisPair(false);
}

//------------------------------------------------------------------------------
void KDTree_Impl::indent(std::stringstream& sstream, size_t depth)
{
    for (; depth != 0; --depth)
    {
        sstream << " ";
    }
}

//------------------------------------------------------------------------------
// SortStackFrame
//------------------------------------------------------------------------------
class SortStackFrame
{
public:
    KDTree::Entries& m_entries;
    KDTree_Nodes& m_nodes;
    KDTree_PrimitiveIds m_primitives;
    KDTree_Edges& m_edges;
    const BoundsF m_bounds;
    const size_t m_depth;
    const size_t m_maxDepth;
    const size_t m_parentNodeIndex;

    enum Position
    {
        kRoot = 0,
        kLeft = 1,
        kRight = 2
    };
    const Position m_position;

    SortStackFrame(KDTree::Entries& entries, KDTree_Nodes& nodes,
                   KDTree_PrimitiveIds primitives, KDTree_Edges& edges,
                   BoundsF bounds, size_t depth, size_t maxDepth,
                   size_t parentNodeIndex, Position position)
        : m_entries(entries),
          m_nodes(nodes),
          m_primitives(primitives),
          m_edges(edges),
          m_bounds(bounds),
          m_depth(depth),
          m_maxDepth(maxDepth),
          m_parentNodeIndex(parentNodeIndex),
          m_position(position)
    {
    }
};

typedef ConstVector<SortStackFrame> SortStack;

//------------------------------------------------------------------------------
void KDTree_Impl::sortTree(const SortStackFrame& initial_frame)
{
    SortStack sortStack;

    sortStack.push_back(initial_frame);

    while (!sortStack.empty())
    {
        SortStackFrame& top = sortStack.top();
        const size_t top_position = top.m_position;
        KDTree_Nodes& top_nodes = top.m_nodes;
        const size_t top_parentNodeIndex = top.m_parentNodeIndex;

        size_t nodeIndex = 0;

        // Create a leaf node
        const size_t count = top.m_primitives.size();
        if (count <= 2 || top.m_depth == top.m_maxDepth)
        {
            nodeIndex =
                KDTree_Node_Impl::addLeafNode(top.m_nodes, top.m_primitives);
            sortStack.pop_back();
        }

        // Create a branch node
        else
        {
            // Find the best place to split the tree
            const KDTree_LocationAxisPair locationAxisPair =
                findLocationAndAxis(top.m_entries, top.m_primitives,
                                    top.m_edges, top.m_bounds);

            if (!locationAxisPair.m_shouldSplit)
            {
                nodeIndex = KDTree_Node_Impl::addLeafNode(top.m_nodes,
                                                          top.m_primitives);
                sortStack.pop_back();
            }
            else
            {
                const float location = locationAxisPair.m_location;
                const size_t axis =
                    static_cast<size_t>(locationAxisPair.m_axis);
                const Pair<BoundsF> boundsPair =
                    top.m_bounds.split(axis, location);

                // Find the new location where we need to split the array
                nodeIndex = KDTree_Node_Impl::addBranchNode(top.m_nodes,
                                                            location, axis);

                // Create the child nodes
                KDTree_PrimitiveIds left;
                KDTree_PrimitiveIds right;
                left.reserve(top.m_primitives.size());   // TODO LT: Re-use this
                right.reserve(top.m_primitives.size());  // TODO LT: Re-use this
                for (size_t i = 0; i != top.m_primitives.size(); ++i)
                {
                    const size_t primitiveIndex = top.m_primitives[i];
                    KDTree_Entry& entry = top.m_entries[primitiveIndex];

                    // Straddles
                    if (location > entry.getMin()[axis] &&
                        location < entry.getMax()[axis])
                    {
                        left.push_back(primitiveIndex);
                        right.push_back(primitiveIndex);
                    }
                    // Left
                    else if (entry.getMax()[axis] <= location)
                    {
                        left.push_back(primitiveIndex);
                    }
                    // Right
                    else if (entry.getMin()[axis] >= location)
                    {
                        right.push_back(primitiveIndex);
                    }
                }

                SortStackFrame rightFrame(top.m_entries, top.m_nodes, right,
                                          top.m_edges, boundsPair.m_right,
                                          top.m_depth + 1, top.m_maxDepth,
                                          nodeIndex, SortStackFrame::kRight);

                SortStackFrame leftFrame(top.m_entries, top.m_nodes, left,
                                         top.m_edges, boundsPair.m_left,
                                         top.m_depth + 1, top.m_maxDepth,
                                         nodeIndex, SortStackFrame::kLeft);

                sortStack.pop_back();
                sortStack.push_back(rightFrame);  // Right frame first.
                sortStack.push_back(leftFrame);   // Left frame last, so that it
                                                  // is computed first.
            }
        }
        if (top_position == SortStackFrame::kRight)
        {
            KDTree_Node_Impl::setRight(top_nodes, top_parentNodeIndex,
                                       nodeIndex);
        }
    }
}

//------------------------------------------------------------------------------
// KDTree
//------------------------------------------------------------------------------
KDTree::KDTree()
{
}

//------------------------------------------------------------------------------
KDTree::operator const std::string() const
{
    if (m_nodes.empty())
    {
        return std::string();
    }

    std::stringstream sstream;
    size_t pointIndex = 0;

    typedef std::stack<KDTree_Record> NodeStack;
    NodeStack nodeStack;
    nodeStack.push(KDTree_Record(0, 0, 0));
    while (!nodeStack.empty())
    {
        KDTree_Record record = nodeStack.top();
        const size_t depth = record.m_depth;
        const KDTree_Node& node =
            KDTree_Node_Impl::lookupNode(m_nodes, record.m_node);
        nodeStack.pop();

        if (KDTree_Node_Impl::isBranch(node))
        {
            KDTree_Impl::indent(sstream, depth);
            sstream << "Axis: " << KDTree_Node_Impl::getAxis(node)
                    << " Location: " << KDTree_Node_Impl::getLocation(node)
                    << std::endl;

            // Add right
            nodeStack.push(KDTree_Record(
                depth + 1, KDTree_Node_Impl::getRight(m_nodes, record.m_node),
                KDTree_Node_Impl::getAxis(node)));
            // Add left
            nodeStack.push(KDTree_Record(
                depth + 1, KDTree_Node_Impl::getLeft(m_nodes, record.m_node),
                KDTree_Node_Impl::getAxis(node)));
        }
        else
        {
            KDTree_Impl::indent(sstream, depth);
            sstream << "Leaf" << std::endl;

            if (KDTree_Node_Impl::getPrimitiveCount(node) != 0)
            {
                const size_t* primitives =
                    KDTree_Node_Impl::getPrimitives(m_nodes, record.m_node);
                for (size_t i = 0;
                     i != KDTree_Node_Impl::getPrimitiveCount(node); ++i)
                {
                    const KDTree_Entry& entry = m_entries[primitives[i]];
                    KDTree_Impl::indent(sstream, depth + 1);
                    sstream << "PrimitiveId: " << entry.getPrimitiveId() << " "
                            << "Location: "
                            << entry.getMin()[record.m_parentAxis] << std::endl;
                    ++pointIndex;
                }
            }
        }
    }

    return sstream.str();
}

//------------------------------------------------------------------------------
std::string KDTree::toObj() const
{
    if (m_nodes.empty())
    {
        return std::string();
    }

    size_t objectIndex = 0;
    std::stringstream sstream;

    typedef std::stack<KDTree_Record> NodeStack;
    typedef ConstVector<BoundsF> BoundsStack;
    NodeStack nodeStack;
    BoundsStack boundsStack;
    nodeStack.push(KDTree_Record(0, 0, 0));
    boundsStack.push_back(BoundsF(m_boundsBuilder));

    while (!nodeStack.empty())
    {
        KDTree_Record record = nodeStack.top();
        const size_t depth = record.m_depth;
        const KDTree_Node& node =
            KDTree_Node_Impl::lookupNode(m_nodes, record.m_node);
        nodeStack.pop();

        if (KDTree_Node_Impl::isBranch(node))
        {
            const size_t axis = KDTree_Node_Impl::getAxis(node);
            const float location = KDTree_Node_Impl::getLocation(node);
            const Pair<BoundsF> boundsPair =
                boundsStack.back().split(axis, location);

            boundsStack.pop_back();

            // Add right
            nodeStack.push(KDTree_Record(
                depth + 1, KDTree_Node_Impl::getRight(m_nodes, record.m_node),
                KDTree_Node_Impl::getAxis(node)));
            boundsStack.push_back(boundsPair.m_right);
            // Add left
            nodeStack.push(KDTree_Record(
                depth + 1, KDTree_Node_Impl::getLeft(m_nodes, record.m_node),
                KDTree_Node_Impl::getAxis(node)));
            boundsStack.push_back(boundsPair.m_left);
        }
        else
        {
            const size_t primitiveCount =
                KDTree_Node_Impl::getPrimitiveCount(node);
            const BoundsF& bounds = boundsStack.back();

            sstream << "o Leaf_" << objectIndex << "_primitives";
            if (primitiveCount != 0)
            {
                const size_t* primitives =
                    KDTree_Node_Impl::getPrimitives(m_nodes, record.m_node);
                for (size_t i = 0; i != primitiveCount; ++i)
                {
                    const KDTree_Entry& entry = m_entries[primitives[i]];
                    sstream << "_" << entry.getPrimitiveId();
                }
            }
            sstream << std::endl;
            sstream << bounds.toObj(objectIndex);
            ++objectIndex;

            boundsStack.pop_back();
        }
    }

    return sstream.str();
}

//------------------------------------------------------------------------------
void KDTree::addEntry(const BoundsF& bounds,
                      const KDTree_PrimitiveId primitiveId)
{
    m_boundsBuilder.expandBounds(bounds.m_min);
    m_boundsBuilder.expandBounds(bounds.m_max);

    const KDTree_Entry entry(bounds, primitiveId);
    m_entries.push_back(entry);
}

//------------------------------------------------------------------------------
KDTree_TraceResult KDTree::findEntries(
    KDTree_SearchCache& searchCache, const Ray& ray,
    const KDTree_PrimitiveIntersect& primtiveTest) const
{
    if (m_nodes.empty())
    {
        return KDTree_TraceResult(FLT_MAX, 0);
    }

    enum Favour
    {
        kLeft = 0,
        kRight = 1
    };

    // Work out which part of each node to use for.
    Favour favour[3] = {kLeft, kLeft, kLeft};
    for (size_t axis = 0; axis != 3; ++axis)
    {
        favour[axis] = ray.m_direction[axis] >= 0.0f ? kLeft : kRight;
    }

    float bestDistanceAlongRay = FLT_MAX;
    size_t bestPrimitiveIndex = 0;

    searchCache.clear();

    BoundsF rootBounds = BoundsF(m_boundsBuilder);

    if(intersect_bounds(ray,rootBounds))
    {
        searchCache.m_stack.push_back(KDTree_SearchCache_StackFrame(0, rootBounds));
        while (!searchCache.m_stack.empty())
        {
            KDTree_SearchCache_StackFrame stackFrame =
                    searchCache.m_stack.top();
            searchCache.m_stack.pop_back();

            const KDTree_Node& node =
                KDTree_Node_Impl::lookupNode(m_nodes, stackFrame.m_nodeIndex);

            if (KDTree_Node_Impl::isBranch(node))
            {
                const size_t axis = KDTree_Node_Impl::getAxis(node);
                const float location = KDTree_Node_Impl::getLocation(node);
                const Pair<BoundsF> boundsPair =
                    stackFrame.m_bounds.split(axis, location);

                // Child indicies
                const size_t childIndicies[2] =
                {
                        KDTree_Node_Impl::getLeft(m_nodes,
                                                  stackFrame.m_nodeIndex),
                        KDTree_Node_Impl::getRight(m_nodes,
                                                   stackFrame.m_nodeIndex)
                };
                // Decide which child gets processed first
                const size_t first =
                        favour[axis] == kLeft ||
                        boundsPair.m_left.contains(ray.m_position) ? 0 : 1;
                const size_t second = (~first) & 1;

                // Push the second to be processed first, only add to the stack
                // if the ray intersects the bounds for the child node.
                if(intersect_bounds(ray, boundsPair[second]))
                {
                    searchCache.m_stack.push_back(
                                KDTree_SearchCache_StackFrame(childIndicies[second],
                                                              boundsPair[second]));
                }
                // Push the first to be processed last, only add to the stack
                // if the ray intersects the bounds for the child node.
                if(intersect_bounds(ray, boundsPair[first]))
                {
                    searchCache.m_stack.push_back(
                                KDTree_SearchCache_StackFrame(childIndicies[first],
                                                              boundsPair[first]));
                }
            }
            else
            {
                const size_t primitiveCount =
                    KDTree_Node_Impl::getPrimitiveCount(node);
                if (primitiveCount != 0)
                {
                    const size_t nodeIndex = stackFrame.m_nodeIndex;
                    const size_t* primitiveIndices =
                        KDTree_Node_Impl::getPrimitives(m_nodes, nodeIndex);

                    // Add all the entries in this node
                    for (size_t i = 0; i != primitiveCount; ++i)
                    {
                        const KDTree_Entry& entry =
                            m_entries[primitiveIndices[i]];
                        BoundsF entryBounds(entry.getMin(), entry.getMax());
                        BoundsF entryBoundsIntersection =
                            entryBounds.intersection(stackFrame.m_bounds);
                        if (intersect_bounds(ray, entryBoundsIntersection))
                        {
                            float distanceAlongRay = bestDistanceAlongRay;
                            if (primtiveTest.intersect(distanceAlongRay, ray,
                                                       entry.getPrimitiveId()))
                            {
                                const Vector3<float> intersectionPoint =
                                    ray.computePointOnRay(distanceAlongRay);

                                // It's possible for the ray to intersect the
                                // the triangle at a position that is outside of
                                // the bounds of this part of the triangle,
                                // because
                                // triangles can be shared across bounds.
                                // So we have to reject intersections that are
                                // outside of the bounds of this triangle's
                                // kdtree box.
                                if (entryBoundsIntersection.containsOrTouches(
                                        intersectionPoint))
                                {
                                    bestDistanceAlongRay = distanceAlongRay;
                                    bestPrimitiveIndex = entry.getPrimitiveId();
                                }
                            }
                        }
                    }

                    if (bestDistanceAlongRay != FLT_MAX)
                    {
                        return KDTree_TraceResult(bestDistanceAlongRay,
                                                  bestPrimitiveIndex);
                    }
                }
            }
        }
    }
    return KDTree_TraceResult(FLT_MAX, 0);
}

//------------------------------------------------------------------------------
void KDTree::sortTree()
{
    if (!m_entries.empty())
    {
        const BoundsF bounds(m_boundsBuilder);
        KDTree_Edges edges;
        KDTree_PrimitiveIds primitives;
        primitives.resize(m_entries.size());
        for (size_t i = 0; i != primitives.size(); ++i)
        {
            primitives[i] = i;
        }
        const double primitivesSize = static_cast<double>(primitives.size());

// Taken from PBRT, but adjusted for our test data.
        size_t maxDepth = 25.0f + (1.3f * log(primitivesSize));
        KDTree_Impl::sortTree(SortStackFrame(m_entries, m_nodes, primitives,
                                             edges, bounds, 0, maxDepth, 0,
                                             SortStackFrame::kRoot));
    }
}

}  // namespace tc
