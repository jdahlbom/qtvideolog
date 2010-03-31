#include "videomailwidget.h"

#include <QtGui>
#include <QFile>

void printState(const Phonon::State &state);

VideoMailWidget::VideoMailWidget(QWidget *parent):
    QWidget(parent)
{
    this->setMinimumWidth(1200);
    this->setMinimumHeight(800);
    mediaObject = new Phonon::MediaObject(this);

    widget = new Phonon::VideoWidget(this);
    widget->setMinimumWidth(this->minimumWidth());
    widget->setMinimumHeight(this->minimumHeight());
    Phonon::createPath(mediaObject, widget);

    widget->setScaleMode(Phonon::VideoWidget::FitInView);

    widget->setAttribute(Qt::WA_OpaquePaintEvent, false);

    audioOutput = new Phonon::AudioOutput(Phonon::VideoCategory, this);
    Phonon::createPath(mediaObject, audioOutput);

    connect(mediaObject, SIGNAL(stateChanged(Phonon::State, Phonon::State)),
	    this, SLOT(stateChanged(Phonon::State, Phonon::State)));
}

void VideoMailWidget::setSource(const QString &srcUrl) {
    if (sourceUrl == srcUrl) {
        return;
    }
    sourceUrl = srcUrl;

    source = new Phonon::MediaSource(sourceUrl);

    switch(source->type()) {
    case Phonon::MediaSource::Invalid:
        qDebug("Invalid or missing file!");
        return;
    case Phonon::MediaSource::LocalFile:
        qDebug() << "Local file [" << sourceUrl << "]";
        break;
    case Phonon::MediaSource::Url:
        qDebug() << "Remote URL [" << sourceUrl << "]";
        break;
    default:
        qDebug("Non-local file");
    }
    mediaObject->setCurrentSource(*source);

    play();
}

void VideoMailWidget::play() {
    if (mediaObject->state() != Phonon::ErrorState) {
        mediaObject->play();
        qDebug() << "Called play";
    } else {
        qDebug() << "Media object in error state";
    }
}

QString stateName(const Phonon::State &state) {
    switch(state) {
    case Phonon::ErrorState:
        return QString("ERROR.");
    case Phonon::LoadingState:
        return QString("Loading...");
    case Phonon::StoppedState:
        return QString("Stopped");
    case Phonon::PlayingState:
        return QString("Playing");
    case Phonon::BufferingState:
        return QString("Buffering");
    case Phonon::PausedState:
        return QString("Paused");
    default:
        return QString("Invalid or unknown state");
    }
}

void VideoMailWidget::stateChanged(Phonon::State old, Phonon::State news) {
    QString msg = QString("State changed from %1 to %2").arg(stateName(old), stateName(news));
    qDebug(msg.toAscii().data());
}



VideoMailWidget::~VideoMailWidget() {}
