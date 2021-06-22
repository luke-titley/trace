//------------------------------------------------------------------------------
// Copywrite Luke Titley 2015
//------------------------------------------------------------------------------
#include <QtGui/QApplication>
#include <QtGui/QImage>
#include <QtCore/QTimer>
#include <QtCore/QtGlobal>
//------------------------------------------------------------------------------
#include <iostream>
//------------------------------------------------------------------------------
#include "mainwindow.h"
//------------------------------------------------------------------------------
#include "trace/array.h"
#include "trace/args.h"
#include "trace/clamp.h"
#include "trace/image.h"
#include "trace/recursivePixelIterator.h"
#include "trace/simpleScene.h"
#include "trace/linearPixelIterator.h"
#include "trace/log.h"
#include "trace/objiterator.h"
#include "trace/log.h"
#include "trace/pngwriter.h"
#include "trace/test.h"
#include "trace/renderer.h"
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// IOImage
//------------------------------------------------------------------------------
class IOImage : public tc::Image
{
public:
    IOImage(const size_t width, const size_t height):
        m_image(width, height, QImage::Format_ARGB32)
    {
        m_image.fill(Qt::black);
    }

    const QImage& getQImage() const
    {
        return m_image;
    }

    size_t getHeight() const
    {
        return m_image.height();
    }

    size_t getWidth() const
    {
        return m_image.width();
    }

    void setPixel(const size_t x, const size_t y,
                  const tc::Vector3<float>& value)
    {
        const tc::Vector3<float> valueClamped =
                tc::clamp(tc::Vector3<float>(0.0f), value,
                          tc::Vector3<float>(1.0f));
        m_image.setPixel(x,y,QColor(valueClamped.r*255, valueClamped.g*255,
                                    valueClamped.b*255, valueClamped.a*255).rgba());
    }

    void getPixel(const size_t x, const size_t y,
                  tc::Vector3<float>& value) const
    {
        QColor color = m_image.pixel(x,y);
        value.r = static_cast<float>(color.red())/255.0f;
        value.g = static_cast<float>(color.green())/255.0f;
        value.b = static_cast<float>(color.blue())/255.0f;
        value.a = static_cast<float>(color.alpha())/255.0f;
    }

private:
    QImage m_image;
};

int main(int argc, char ** argv)
{
    QApplication a(argc, argv);

    tc::LogContext logContext;

    // Load the scene and build the renderer
    const char * argsArray[] = {"qtrace",
                                "--maxRayDepth", "2",
                                "--width", "512",
                                "--height", "256",
                                "--samplesPerPixel", "128",
                                "--qualityLevel", "1",
                                "--inputFilename", "../../trace/in.obj"};

    const tc::Args args(sizeof(argsArray)/sizeof(char*), argsArray);
    IOImage image(args.width, args.height);

#if 1
    const size_t threadCount =
        args.threadCount == 0 ? tc::getNumProcs() : args.threadCount;
#else
    const size_t threadCount = 1;
#endif

    // Create an 'lsd' iterator. For piping an lsd file into the scene.
    tc::Obj_ObjectIterator objObjectIterator;
    objObjectIterator.setFilename(args.inputFilename);

    // Create our scene, it will be populated by an object iterator.
    tc::SimpleScene simpleScene(objObjectIterator);

    // render something
    tc::Renderer renderer(logContext,
                          image,
                          args.samplesPerPixel,
                          args.qualityLevel,
                          args.maxRayDepth,
                          simpleScene,
                          simpleScene,
                          threadCount);

#if 0
    {
        tc::ArrayReadLock arrayLock(renderHelper);
        tc::savePNG(array, args.outputFilename);
    }
#endif

    // Set the window size
    MainWindow mainWindow;
    mainWindow.setFixedSize(args.width, args.height);
    mainWindow.startRender(renderer,
                           image.getQImage(),
                           static_cast<int>(args.width),
                           static_cast<int>(args.height));
    mainWindow.show();
    
    return a.exec();
}
