#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTimer>
#include <QVector>
#include <QImage>
#include "trace/array.h"

namespace tc
{
class Image;
class Renderer;
}

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT
    
public:
    typedef QVector<QString> ImageReferences;
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

    void startRender(tc::Renderer& renderer,
                     const QImage & image,
                     const int width,
                     const int height);
protected slots:
    void update();

private:
    tc::Renderer * m_renderer;
    QTimer m_timer;
    const QImage * m_image;
    QString m_currentFilename;
    Ui::MainWindow *ui;
    QPixmap m_pixmap;
    QPixmap m_black;
    unsigned int m_timeInterval;
    unsigned int m_blinkInterval;
    unsigned int m_displayCount;
    unsigned int m_currentDisplayCount;
    bool m_shouldBlink;
    ImageReferences m_images;
};

#endif // MAINWINDOW_H
