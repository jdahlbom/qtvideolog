#include "VideoData.h"

#include <QObject>
#include <QChar>
#include <QString>

VideoData::VideoData(QObject *parent) : QObject(parent), m_length(0)
{}

int VideoData::length() const {
    return m_length;
}

void VideoData::setLength(int len) {
    m_length=len;
    emit lengthChanged();
    emit totalTimeStrChanged();
}

int VideoData::currentTime() const {
    return m_currentTime;
}

void VideoData::setCurrentTime(int time) {
    m_currentTime = time;
    emit currentTimeChanged();
    emit currentTimeStrChanged();
}

QString formatDate(int time) {
    int hours = time / 3600000;
    int minutes = (time / 60000) % 60;
    int seconds = (time / 1000) % 60;

    return QString("%1:%2:%3").arg(hours, 2,10,QChar('0'))
                                .arg(minutes, 2,10,QChar('0'))
                                .arg(seconds, 2, 10, QChar('0'));
}

QString VideoData::totalTimeStr() const {
    return formatDate(m_length);
}

QString VideoData::currentTimeStr() const {
    return formatDate(m_currentTime);
}

