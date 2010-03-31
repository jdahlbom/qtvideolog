#ifndef VIDEODATA_H
#define VIDEODATA_H

#include <QObject>

class VideoData : public QObject {
    Q_OBJECT
    Q_PROPERTY(int length READ length WRITE setLength NOTIFY lengthChanged);
    Q_PROPERTY(int currentTime READ currentTime WRITE setCurrentTime NOTIFY currentTimeChanged);
    Q_PROPERTY(QString currentTimeStr READ currentTimeStr NOTIFY currentTimeStrChanged);
    Q_PROPERTY(QString totalTimeStr READ totalTimeStr NOTIFY totalTimeStrChanged);

public:
    VideoData(QObject *parent=0);
    int length() const;
    void setLength(int len);
    QString totalTimeStr() const;
    QString currentTimeStr() const;
    int currentTime() const;
    void setCurrentTime(int time);

signals:
    void lengthChanged();
    void currentTimeChanged();
    void currentTimeStrChanged();
    void totalTimeStrChanged();
private:
    int m_length;
    int m_currentTime;
};

#endif // VIDEODATA_H
