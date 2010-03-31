#ifndef OVERLAYMANAGER_H
#define OVERLAYMANAGER_H

#include <QWidget>
#include <QString>
#include <QPointer>

class ModifiedQDeclarativeView;
class OverlayManagerImpl;
class DataObject;



class OverlayManager : public QObject {
    Q_OBJECT
public:
    OverlayManager(ModifiedQDeclarativeView *view);
    ~OverlayManager();
private:
    OverlayManagerImpl *impl;
    friend class OverlayManagerImpl;
signals:
    void videoLaunchRequested(QString source);
    void videoSeekRequested(double seekPercentage);
    void videoPlayRequested();
    void videoPauseRequested();

public slots:
    void detailsChanged(DataObject *ptr);
    void requestVideoLaunch(const QString &source);
    void requestVideoPause();
    void requestVideoPlay();
    void requestVideoSeek(double seekPercentage);
    void updateVideoInfo(int totalLength, int currentTime);
};


#endif // OVERLAYMANAGER_H
