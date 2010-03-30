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



class OverlayManagerImpl {
public:
    OverlayManagerImpl(ModifiedQDeclarativeView *view, OverlayManager *parent);
    void changeDetails(DataObject *other);

    DataObject *details;
    SimpleListModel *fakeModel;
    QPointer<OverlayManager> self;
    QPointer<ModifiedQDeclarativeView> view;
}; // class OverlayManagerImpl



OverlayManagerImpl::OverlayManagerImpl(ModifiedQDeclarativeView *view, OverlayManager *parent) {
    Q_ASSERT(view != NULL);
    self = parent;

    fakeModel = new SimpleListModel(self);
    QObject::connect(fakeModel, SIGNAL(detailObjectChanged(DataObject*)),
                     self, SLOT(detailsChanged(DataObject *)));

    details = new DataObject(QString("detailit"), QColor("#BBBB44"), QUrl("jdahlbom.jpg"));
    details->setDescription(QString("Kuvaus"));

    view->engine()->rootContext()->setContextProperty("fake_model", fakeModel);
    view->engine()->rootContext()->setContextProperty("detailObject", details);

    view->setSource(QUrl("overlay.qml"));
} // ctor

void OverlayManagerImpl::changeDetails(DataObject *other) {
    *details = *other;
}

void OverlayManager::detailsChanged(DataObject *ptr) {
    assert(ptr != NULL);
    impl->changeDetails(ptr);
}

OverlayManager::OverlayManager(ModifiedQDeclarativeView *view) : QObject::QObject(view) {
    impl = new OverlayManagerImpl(view, this);
}
OverlayManager::~OverlayManager() {
    delete(impl);
}
