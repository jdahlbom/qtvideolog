#ifndef VIDEOMAILWIDGET_H
#define VIDEOMAILWIDGET_H

#include <QUrl>
#include <QWidget>
#include <QGLWidget>

#include <phonon/MediaObject>
#include <phonon/MediaSource>
#include <phonon/VideoWidget>
#include <phonon/AudioOutput>

class VideoMailWidget : public QWidget {
    Q_OBJECT

public:
    VideoMailWidget(QWidget *parent);
    ~VideoMailWidget();
    static void printAvailableMimeTypes();

public slots:
    void setSource(const QString &sourceUrl);
    void stateChanged(Phonon::State, Phonon::State);
    void play();

private:
    QString sourceUrl;
    Phonon::MediaObject *mediaObject;
    Phonon::MediaSource *source;
    Phonon::VideoWidget *widget;
    Phonon::AudioOutput *audioOutput;
};

#endif
