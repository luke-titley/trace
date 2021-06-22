//------------------------------------------------------------------------------
// Copywrite Luke Titley 2015
//------------------------------------------------------------------------------
#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "trace/renderer.h"
#include "trace/clamp.h"

#include <QtGui/QKeyEvent>
#include <QtCore/QDirIterator>
#include <QtCore/QDebug>
#include <QtGlobal>
#include <time.h>

//------------------------------------------------------------------------------
// MainWindow
//------------------------------------------------------------------------------
MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    m_renderer(0),
    m_image(0),
    ui(new Ui::MainWindow),
    m_black(100, 100),
    m_timeInterval(0),
    m_blinkInterval(0),
    m_displayCount(1),
    m_currentDisplayCount(0),
    m_shouldBlink(false)
{
    ui->setupUi(this);
    qsrand( (unsigned int)time(NULL) );

    // We have a black
    // image we can use for blinking.
    //
    m_black.fill( Qt::black );

    // Connect a timer to the update
    // method so that the images are updated
    // at a fixed interval.
    //
    QObject::connect(&m_timer, SIGNAL(timeout()), this, SLOT(update()));
}

//------------------------------------------------------------------------------
MainWindow::~MainWindow()
{
    delete ui;
}

//------------------------------------------------------------------------------
void MainWindow::update()
{
    m_timer.stop();
    if(m_renderer== 0)
    {
        return;
    }

    // Get information about how the render is going.
    m_renderer->next();

    // Copy the current image array into a QImage so that we can see the
    // progress
    // We wrap braces around the below code to ensure that the lock is released
    // automatically when we exit the scope.
    if(m_image != 0){
        tc::ArrayReadLock arrayReadLock(*m_renderer);
        if(arrayReadLock.hasNewContent())
        {
            m_pixmap.convertFromImage(*m_image);
            m_pixmap = m_pixmap.scaled(this->size(), Qt::KeepAspectRatio);
        }
    }
    ui->referenceImage->setPixmap(m_pixmap);
    m_timer.start();
}

//------------------------------------------------------------------------------
void MainWindow::startRender(tc::Renderer & renderer,
                             const QImage & image,
                             const int width,
                             const int height)
{
    m_image = & image;
    m_renderer = & renderer;
    m_timeInterval = 2000; // Two seconds
    m_timer.setInterval(m_timeInterval);
    m_timer.start();
    m_pixmap.fill(Qt::black);
    m_pixmap = QPixmap(width,height);
    ui->referenceImage->setPixmap(m_pixmap);
}
