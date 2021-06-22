//------------------------------------------------------------------------------
// Copywrite Luke Titley 2015
//------------------------------------------------------------------------------
#include "trace/test.h"
//------------------------------------------------------------------------------
#include "trace/array.h"
#include "trace/bounds.h"
#include "trace/intersect.h"
#include "trace/kdtree.h"
#include "trace/log.h"
#include "trace/solidangle.h"
#include "trace/tree.h"
#include "trace/vector.h"
//------------------------------------------------------------------------------
namespace tc
{
//------------------------------------------------------------------------------
// Unit Tests
//------------------------------------------------------------------------------
void runTests(const tc::LogContext& logContext)
{
    logContext.log("Running Unit Tests\n");

// Headers we don't test
// * assert : Just a single assertion function
// * docs : Just a documentation file
// * geoid : No functions
// * geoAPI : Purely abstract
// * objectiterator : Purely abstract
// * pixeliterator : Purely abstract
// * shadeAPI : Purely abstract
// * shader : Purely abstract
// * surfaceFrame : No functions

// These are the headers that we unit test, they should be listed
// alphabetically.
#if 0
    argsRunUnitTests(logContext);
#endif
    arrayRunUnitTests(logContext);
    boundsRunUnitTests(logContext);
#if 0
    clampRunUnitTests(logContext);
#endif
    constvectorRunUnitTests(logContext);
    intersectRunUnitTests(logContext);
    kdtreeRunUnitTests(logContext);
#if 0
    linearPixelIteratorRunUnitTests(logContext);
    logRunUnitTests(logContext);
    matrixRunUnitTests(logContext);
    objiteratorRunUnitTests(logContext);
    pngwriterRunUnitTests(logContext);
    radianceRunUnitTests(logContext);
    randomRunUnitTests(logContext);
    rayRunUnitTests(logContext);
    recursivePixelIteratorRunUnitTests(logContext);
    recursiveSequenceIteratorRunUnitTests(logContext);
    rendererRunUnitTests(logContext);
    renderThreadsRunUnitTests(logContext);
    sampledspectrumRunUnitTests(logContext);
    shaderRunUnitTests(logContext);
    shadersDiffuseRunUnitTests(logContext);
    shadeRunUnitTests(logContext);
    shadestackRunUnitTests(logContext);
    simpleSceneRunUnitTests(logContext);
#endif
    solidangleRunUnitTests(logContext);
    treeRunUnitTests(logContext);
#if 0
    sphereRunUnitTests(logContext);
    supersampleiteratorRunUnitTests(logContext);
    surfaceframeRunUnitTests(logContext);
    threadRunUnitTests(logContext);
    timeRunUnitTests(logContext);
    traceResultRunUnitTests(logContext);
    triangleCacheRunUnitTests(logContext);
    triangleIteratorRunUnitTests(logContext);
    triangleRunUnitTests(logContext);
    variantRunUnitTests(logContext);
#endif
    vectorRunUnitTests(logContext);
    return;
}

void test(const tc::LogContext& logContext, bool cond, const char* filename,
          int lineno)
{
    if (!cond)
    {
        logContext.log("Test failed : Line '");
        logContext.log(lineno);
        logContext.log("' File '");
        logContext.log(filename);
        logContext.log("'\n");
    }
}

}  // namespace tc
