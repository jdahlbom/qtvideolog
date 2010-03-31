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
    void printDO();
    friend class OverlayManagerImpl;
signals:
    void videoLaunchRequested(QString source);

public slots:
    void detailsChanged(DataObject *ptr);
    void requestVideoLaunch(const QString &source);
};


#endif // OVERLAYMANAGER_H
