#include "overlaywidget.h"

#include <QDeclarativeView>
#include <QDeclarativeEngine>
#include <QDeclarativeContext>
#include <QPointer>
#include <QUrl>
#include <QVariant>

#include <cassert>

#include "dataobject.h"
#include "simplelistmodel.h"

class OverlayWidgetImpl {
public:
    OverlayWidgetImpl(OverlayWidget *parent)
    {
        self = parent;

        fakeModel = new SimpleListModel(self);
        QObject::connect(fakeModel, SIGNAL(detailObjectChanged(DataObject*)),
                         self, SLOT(detailsChanged(DataObject *)));

        details = new DataObject(QString("detailit"), QColor("#BBBB44"), QUrl("jdahlbom.jpg"));
        details->setDescription(QString("Kuvaus"));

        view = new QDeclarativeView(parent);

        view->engine()->rootContext()->setContextProperty("fake_model", fakeModel);
        view->engine()->rootContext()->setContextProperty("detailObject", details);

        view->setSource(QUrl("overlay.qml"));
    } // ctor

    void changeDetails(DataObject *other) {
        *details = *other;
    }

    DataObject *details;
    SimpleListModel *fakeModel;
    QPointer<OverlayWidget> self;
    QPointer<QDeclarativeView> view;
}; // class OverlayWidgetImpl

void OverlayWidget::detailsChanged(DataObject *ptr) {
    assert(ptr != NULL);
    impl->changeDetails(ptr);
}

OverlayWidget::OverlayWidget(QWidget *parent) : QWidget::QWidget(parent) {
    impl = new OverlayWidgetImpl(this);
}
OverlayWidget::~OverlayWidget() {
    delete(impl);
}

