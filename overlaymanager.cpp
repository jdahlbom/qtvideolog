#include "overlaymanager.h"

#include <QDeclarativeEngine>
#include <QDeclarativeContext>
#include <QPointer>
#include <QUrl>
#include <QVariant>

#include <cassert>

#include "modifiedqdeclarativeview.h"

#include "dataobject.h"
#include "simplelistmodel.h"
#include "VideoData.h"


class OverlayManagerImpl {
public:
    OverlayManagerImpl(ModifiedQDeclarativeView *view, OverlayManager *parent);
    void changeDetails(DataObject *other);
    void updateVideoInfo(int totalLength, int currentTime);

    DataObject *details;
    SimpleListModel *fakeModel;
    OverlayManager *self;
    VideoData *videoData;
    QPointer<ModifiedQDeclarativeView> view;
}; // class OverlayManagerImpl



OverlayManagerImpl::OverlayManagerImpl(ModifiedQDeclarativeView *view, OverlayManager *parent) {
    Q_ASSERT(view != NULL);
    self = parent;

    videoData = new VideoData(self);
    fakeModel = new SimpleListModel(self);
    QObject::connect(fakeModel, SIGNAL(detailObjectChanged(DataObject*)),
                     self, SLOT(detailsChanged(DataObject *)));
    QObject::connect(fakeModel, SIGNAL(videoLaunchRequested(QString)),
                     self, SLOT(requestVideoLaunch(const QString &)));

    details = new DataObject(QString(""), QColor("#000000"),
                             QUrl(""), QString(""));

    view->engine()->rootContext()->setContextProperty("fake_model", fakeModel);
    view->engine()->rootContext()->setContextProperty("detailObject", details);
    view->engine()->rootContext()->setContextProperty("overlayManager", self);
    view->engine()->rootContext()->setContextProperty("videoData", videoData);

    view->setSource(QUrl("overlay.qml"));
} // ctor

void OverlayManagerImpl::changeDetails(DataObject *other) {
    *details = *other;
}

void OverlayManager::detailsChanged(DataObject *ptr) {
    assert(ptr != NULL);
    impl->changeDetails(ptr);
}

void OverlayManagerImpl::updateVideoInfo(int totalLength, int currentTime) {
    videoData->setLength(totalLength);
    videoData->setCurrentTime(currentTime);
}

OverlayManager::OverlayManager(ModifiedQDeclarativeView *view) : QObject::QObject(view) {
    impl = new OverlayManagerImpl(view, this);
}
OverlayManager::~OverlayManager() {
    delete(impl);
}

void OverlayManager::requestVideoLaunch(const QString &source) {
    emit videoLaunchRequested(source);
}

void OverlayManager::requestVideoPause() {
    emit videoPauseRequested();
}

void OverlayManager::requestVideoPlay() {
    emit videoPlayRequested();
}

void OverlayManager::requestVideoSeek(double seekTo) {
    emit videoSeekRequested(seekTo);
}

void OverlayManager::updateVideoInfo(int totalLength, int currentTime) {
    impl->updateVideoInfo(totalLength, currentTime);
}
