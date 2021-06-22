//------------------------------------------------------------------------------
// Copywrite Luke Titley 2015
//------------------------------------------------------------------------------
#ifndef TC_TRIANGLECACHE
#define TC_TRIANGLECACHE
//------------------------------------------------------------------------------
#include "trace/kdtree.h"
//------------------------------------------------------------------------------

namespace tc
{

/// \brief For storing bounding volumes and efficiantly searching for ray
/// intersections within those bounding volumes.
class TriangleCache : public KDTree
{
};

/// \brief A block of re-usable memory, useful for repeated ray intersection
/// searches.
class SearchCache : public KDTree_SearchCache
{
};

}  // namespace tc
#endif  // TC_TRIANGLECACHE
