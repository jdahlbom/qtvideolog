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

    ModifiedQDeclarativeView view;

    view.setViewport(new QGLWidget);

    //OverlayManager *overlay = new OverlayManager(&view);
    VideoMailWidget video(0);

    QFile sourceFile("media/Impact_movie.ogg");
    if (!sourceFile.exists()) {
        qDebug() << "Invalid file, does not exist.";
    } else {
        qDebug() << "File [" << sourceFile.fileName() <<"] exists.";
    }

    video.setSource(sourceFile.fileName());
    video.play();

    QGraphicsProxyWidget *proxy = view.scene()->addWidget(&video);
    proxy->show();
    proxy->setZValue(1.0f);

    view.show();


    return a.exec();
}
