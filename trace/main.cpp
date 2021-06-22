//------------------------------------------------------------------------------
// Copywrite Luke Titley 2015
//------------------------------------------------------------------------------
#include "trace/args.h"
#include "trace/array.h"
#include "trace/image.h"
#include "trace/linearPixelIterator.h"
#include "trace/log.h"
//#include "trace/lsditerator.h"
#include "trace/objiterator.h"
#include "trace/pngwriter.h"
#include "trace/recursivePixelIterator.h"
#include "trace/renderer.h"
#include "trace/simpleScene.h"
#include "trace/thread.h"
#include "trace/time.h"
//------------------------------------------------------------------------------
#include <iostream>
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// IOImage
//------------------------------------------------------------------------------
class IOImage : public tc::Image
{
public:
    IOImage(const size_t width, const size_t height):
        m_array(width, height)
    {
        const float black[] = {0.0f, 0.0f, 0.0f, 0.0f};
        m_array.fill(black);
    }

    const tc::Array<float,4>& getArray() const
    {
        return m_array;
    }

    size_t getHeight() const
    {
        return m_array.getHeight();
    }

    size_t getWidth() const
    {
        return m_array.getWidth();
    }

    void setPixel(const size_t x, const size_t y,
                  const tc::Vector3<float>& value)
    {
        for(size_t i=0; i!=4; ++i)
        {
            m_array.setValue(x,y,i,value[i]);
        }
    }

    void getPixel(const size_t x, const size_t y,
                  tc::Vector3<float>& value) const
    {
        for(size_t i=0; i!=4; ++i)
        {
            value[i] = m_array.getValue(x,y,i);
        }
    }

private:
    tc::Array<float,4> m_array;
};

//------------------------------------------------------------------------------
// main
//------------------------------------------------------------------------------
int main(const int argc, const char *argv[])
{
    const tc::Args args(argc, argv);
    const tc::LogContext logContext;

    if(args.runUnitTests)
    {
        // runTests
        tc::runTests(logContext);
    }

    if(args.render)
    {
        IOImage image(args.width, args.height);
    #if 1
        const size_t threadCount =
            args.threadCount == 0 ? tc::getNumProcs() : args.threadCount;
    #else
        const size_t threadCount = 1;
    #endif

#if 0
        // Create an 'lsd' iterator. For piping an lsd file into the scene.
        tc::lsdObjectIterator objectIterator;
        objectIterator.setFilename(args.inputFilename);
#else
        // Create an 'obj' iterator. For piping an obj file into the scene.
        tc::Obj_ObjectIterator objectIterator;
        objectIterator.setFilename(args.inputFilename);
#endif

        tc::Timer timeRender;

        // Create our scene, it will be populated by an object iterator.
        std::cout << "# Building scene" << std::endl;
        tc::SimpleScene simpleScene(objectIterator);

        // Create a renderer instance. This manages the render threads.
        std::cout << "# Rendering" << std::endl;
        tc::Renderer renderer(logContext,
                              image,
                              args.samplesPerPixel,
                              args.qualityLevel,
                              args.maxRayDepth,
                              simpleScene, // Geo
                              simpleScene, // Shade
                              threadCount);


        // Kick off our render and monitor its progress.
        const size_t setupTime = timeRender.elapsedTime();
        size_t lastTime = setupTime;
        while(renderer.next())
        {
            if(args.reportProgress)
            {
                size_t currentTime = timeRender.elapsedTime();
                if((currentTime - lastTime) > args.secondsBetweenProgressReport)
                {
                    std::cout << renderer.computePercentComplete()
                              << std::endl;
                }
                lastTime = currentTime;
            }
        }
        const size_t elapsedTime = timeRender.elapsedTime();
        std::cout << "setup_time= " << setupTime << std::endl;
        std::cout << "render_time= " << elapsedTime-setupTime << std::endl;
        std::cout << "elapsed_time= " << elapsedTime << std::endl;

        // Write to a png file.
        {
            tc::ArrayReadLock arrayLock(renderer);
            tc::savePNG(image.getArray(), args.outputFilename);
        }
    }

    return 0;
}
