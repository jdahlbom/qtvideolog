#include <QtGui/QApplication>
#include <QGLWidget>
#include <QGraphicsProxyWidget>

#include <QFile>

#include "modifiedqdeclarativeview.h"
#include "videomailwidget.h"
#include "overlaymanager.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    QWidget mainWindow(0);
    mainWindow.setMinimumSize(1200, 800);
    ModifiedQDeclarativeView *view = new ModifiedQDeclarativeView(&mainWindow);

    view->setViewport(new QGLWidget);

    OverlayManager *overlay = new OverlayManager(view);
    VideoMailWidget video(0);

    QObject::connect(overlay, SIGNAL(videoLaunchRequested(QString)),
                     &video, SLOT(setSource(QString)));
    QObject::connect(overlay, SIGNAL(videoPlayRequested()),
                     &video, SLOT(play()));
    QObject::connect(overlay, SIGNAL(videoPauseRequested()),
                     &video, SLOT(pause()));
    QObject::connect(overlay, SIGNAL(videoSeekRequested(double)),
                     &video, SLOT(seek(double)));
    QObject::connect(&video, SIGNAL(sendVideoInfo(int,int)),
                     overlay, SLOT(updateVideoInfo(int,int)));

    QGraphicsProxyWidget *proxy = view->scene()->addWidget(&video);
    //proxy->show();
    proxy->setZValue(1.0f);

    mainWindow.show();


    return a.exec();
}
