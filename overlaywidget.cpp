#include "overlaywidget.h"

#include <QDeclarativeView>
#include <QDeclarativeEngine>
#include <QDeclarativeContext>
#include <QPointer>
#include <QUrl>
#include <QVariant>

#include "dataobject.h"
#include "simplelistmodel.h"

class OverlayWidgetImpl {
public:
    OverlayWidgetImpl(OverlayWidget *parent) {
        self = parent;

        fakeModel = new SimpleListModel(self);

        view = new QDeclarativeView(parent);

        view->engine()->rootContext()->setContextProperty("fake_model", fakeModel);
        view->setSource(QUrl("overlay.qml"));
    } // ctor


    SimpleListModel *fakeModel;
    QPointer<OverlayWidget> self;
    QPointer<QDeclarativeView> view;
}; // class OverlayWidgetImpl

OverlayWidget::OverlayWidget(QWidget *parent) : QWidget::QWidget(parent) {
    impl = new OverlayWidgetImpl(this);
}
OverlayWidget::~OverlayWidget() {
    delete(impl);
}

