#ifndef OVERLAYWIDGET_H
#define OVERLAYWIDGET_H

#include <QWidget>
#include <QString>
#include <QPointer>


class OverlayWidgetImpl;

class OverlayWidget : public QWidget {
    Q_OBJECT
public:
    OverlayWidget(QWidget *parent=0);
    ~OverlayWidget();
private:
    OverlayWidgetImpl *impl;
    friend class OverlayWidgetImpl;
};


#endif // OVERLAYWIDGET_H
