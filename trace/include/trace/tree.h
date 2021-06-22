//------------------------------------------------------------------------------
// Copywrite Luke Titley 2015
//------------------------------------------------------------------------------
#ifndef TC_TREE
#define TC_TREE
//------------------------------------------------------------------------------
#include "trace/assert.h"
#include "trace/int.h"
//------------------------------------------------------------------------------
#include <climits>
#include <cstdlib>
#include <cstring>
#include <ostream>
#include <vector>

namespace tc
{
namespace tree
{
//------------------------------------------------------------------------------
// Node
//------------------------------------------------------------------------------
/// \brief A Node in a tc::tree:Tree.
//------------------------------------------------------------------------------
template <typename T>
class Node
{
public:
    enum Type
    {
        kLeaf = 0,
        kBranch = 1
    };

    /// \brief Initialises a tc::tree::Node instance.
    /// \param data: The data to store inside this node.
    inline Node(const T& data);

    /// \return: true if this is a leaf node, false if it is a branch node.
    inline bool isLeaf() const;

    /// \return: true if this is a branch node, false if it is a leaf node.
    inline bool isBranch() const;

    /// \return: The data contained in this node.
    inline const T& getData() const;

    /// \return: The id of the next sibling of this node.
    inline uint32_t getNextSibling() const;

    /// \brief Sets the id of the next sibling of this node.
    /// \param siblingId: The index of this node's neighbour.
    inline void setNextSibling(uint32_t siblingId);

    /// \brief Sets the type of this node to kBranch;
    inline void setToBranch();

private:
    // The first bit determins whether it is a branch or a leaf:
    // [0] = Leaf node.
    // The rest of the bits contain the index of the next sibling node.
    uint32_t m_flags;

    T m_data;
};

//------------------------------------------------------------------------------
template <typename T>
Node<T>::Node(const T& data)
    : m_flags(0), m_data(data)
{
}

//------------------------------------------------------------------------------
template <typename T>
bool Node<T>::isLeaf() const
{
    return !isBranch();
}

//------------------------------------------------------------------------------
template <typename T>
bool Node<T>::isBranch() const
{
    return m_flags & kBranch;
}

//------------------------------------------------------------------------------
template <typename T>
const T& Node<T>::getData() const
{
    return m_data;
}

//------------------------------------------------------------------------------
template <typename T>
uint32_t Node<T>::getNextSibling() const
{
    return m_flags >> 1;
}

//------------------------------------------------------------------------------
template <typename T>
void Node<T>::setNextSibling(uint32_t siblingId)
{
    m_flags = (siblingId << 1) | static_cast<uint32_t>(isBranch());
}

//------------------------------------------------------------------------------
template <typename T>
void Node<T>::setToBranch()
{
    m_flags |= kBranch;
}

//------------------------------------------------------------------------------
// Tree
//------------------------------------------------------------------------------
/// \brief A generic compact tree structure.
///
/// This is intended to be used as a base class for kd and bvh tree
/// implementations.
/// Each node (with no data) is 32 bits in size, and the node tree is one
/// contigious block of these nodes. If the added data per node is another 32
/// bits, then the tree will remain fairly compact.
///
/// There are some limitations on it's use.
///
/// - The first child of a node must be created immediately after it's parent.
///   That is, the tree must be built in depth order.
/// - The internal order of the children is such that the first child added will
///   always be the first child, but all subsequent children will be listed in
///   reverse order. This may feel counter intuative, but by traversing all
///   other children first, it is possible to make the traversal such that
///   all the children are listed in the reverse order to how they were added,
///   which may be slightly more intuative.
template <typename T>
class Tree
{
public:
    typedef typename Node<T>::Type NodeType;

    /// \brief Initializes a new Tree.
    /// \param data: The information stored at the root of the tree.
    Tree(const T& data);

    /// \brief Adds a new node to the tree.
    /// \param parentId: The id of the parent node, for this new node.
    /// \param data: The data to store on this node.
    /// \return: The id of the newly created node.
    uint32_t addNode(uint32_t parentId, const T& data);

    /// \brief Returns the data for given node.
    /// \param nodeId: The id of the node whose data we want.
    /// \return: A reference to the data for the given node.
    const T& getNodeData(const uint32_t nodeId) const;

    /// \brief Returns the data for given node.
    /// \param nodeId: The id of the node whose data we want.
    /// \return: A reference to the data for the given node.
    T& getNodeData(const uint32_t nodeId);

    /// \brief Writes the contents of this tree to the given output stream.
    ///
    /// This is useful for debugging.
    /// \param ostream: The output stream to write to.
    void print(std::ostream& ostream) const;

private:
    typedef std::vector<Node<T> > Nodes;
    Nodes m_nodes;
};

//------------------------------------------------------------------------------
template <typename T>
Tree<T>::Tree(const T& data)
{
    m_nodes.push_back(Node<T>(data));
}

//------------------------------------------------------------------------------
template <typename T>
uint32_t Tree<T>::addNode(const uint32_t parentId, const T& data)
{
    assert(parentId < m_nodes.size());
    // This assertion is important. It enforces that the first child of a node
    // is always created immediately after it. This helps to keep the tree
    // compact.
    assert(parentId == (m_nodes.size() - 1) || m_nodes[parentId].isBranch());

    const uint32_t nodeId = m_nodes.size();
    m_nodes.push_back(Node<T>(data));

    // If this is not the first child then place a reference to this node from
    // the first child.
    if (parentId == nodeId - 1)
    {
        m_nodes[parentId].setToBranch();
    }
    else
    {
        const uint32_t firstChildId = parentId + 1;
        assert(firstChildId < m_nodes.size());

        Node<T>& node = m_nodes.back();
        Node<T>& firstChild = m_nodes[firstChildId];
        node.setNextSibling(firstChild.getNextSibling());
        firstChild.setNextSibling(nodeId);
    }

    return nodeId;
}

//------------------------------------------------------------------------------
template <typename T>
const T& Tree<T>::getNodeData(const uint32_t nodeId) const
{
    assert(nodeId < m_nodes.size());
    return m_nodes[nodeId].m_data;
}

//------------------------------------------------------------------------------
template <typename T>
T& Tree<T>::getNodeData(const uint32_t nodeId)
{
    assert(nodeId < m_nodes.size());
    return m_nodes[nodeId].m_data;
}

//------------------------------------------------------------------------------
template <typename T>
void Tree<T>::print(std::ostream& ostream) const
{
    std::vector<uint64_t> stack;
    stack.push_back(0);  // Node '0', with no indentation.

    // Loop until the stack is empty
    while (!stack.empty())
    {
        const uint64_t top = stack.back();
        stack.pop_back();

        // Split the stack entry into two values, a node id and an indentation.
        const uint32_t nodeId = UINT_MAX & top;
        const uint32_t indentation = top >> 32;
        const Node<T>& node = m_nodes[nodeId];

        // If we are a branch node, then iterate over all the children adding
        // them to the stack.
        if (node.isBranch())
        {
            // Add all the children.
            uint32_t childNodeId = nodeId + 1;
            while (childNodeId != 0)
            {
                assert(childNodeId < m_nodes.size());
                const Node<T>& childNode = m_nodes[childNodeId];
                uint64_t frame =
                    static_cast<uint64_t>(childNodeId) |
                    ((static_cast<uint64_t>(indentation + 1)) << 32);
                stack.push_back(frame);
                childNodeId = childNode.getNextSibling();
            }
        }

        // Build an indentation string.
        assert(indentation < 64);
        char indent[indentation + 1];
        memset(indent, ' ', indentation);
        indent[indentation] = '\0';

        // Print the node.
        const char* typeAsString = node.isBranch() ? "branch" : "leaf";
        ostream << indent << "Node (type=" << typeAsString
                << ", sibling=" << node.getNextSibling() << ")" << std::endl;
    }
}

}  // namespace tree

//------------------------------------------------------------------------------
// Runs all the: unit tests for the 'tree' header file.
void treeRunUnitTests(const tc::LogContext& logContext);

}  // namespace tc

#endif  // TC_TREE
