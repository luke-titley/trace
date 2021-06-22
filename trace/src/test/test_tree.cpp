//------------------------------------------------------------------------------
// Copywrite Luke Titley 2015
//------------------------------------------------------------------------------
#include "trace/log.h"
#include "trace/test.h"
#include "trace/tree.h"
//------------------------------------------------------------------------------
#include <cstring>
#include <sstream>

namespace tc
{
void treeRunUnitTests(const tc::LogContext& logContext)
{
    /// [test_tree general]
    tree::Tree<float> tree(0.0f);
    const uint32_t a = tree.addNode(0, 0.1f);
    const uint32_t b = tree.addNode(a, 0.11f);
    tree.addNode(b, 0.111f);
    tree.addNode(b, 0.112);
    const uint32_t e = tree.addNode(a, 0.13f);
    const uint32_t f = tree.addNode(e, 0.131f);
    tree.addNode(f, 0.1311f);

    std::stringstream stringStream;
    tree.print(stringStream);

    std::string result = stringStream.str();
    std::string correctResult =
        "Node (type=branch, sibling=0)\n"
        " Node (type=branch, sibling=0)\n"
        "  Node (type=branch, sibling=0)\n"
        "   Node (type=branch, sibling=0)\n"
        "    Node (type=leaf, sibling=0)\n"
        "  Node (type=branch, sibling=5)\n"
        "   Node (type=leaf, sibling=0)\n"
        "   Node (type=leaf, sibling=4)\n";

    TC_IS(logContext, result == correctResult);
    /// [test_tree general]
}

}  // namespace tc
