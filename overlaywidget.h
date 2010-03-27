#ifndef OVERLAYWIDGET_H
#define OVERLAYWIDGET_H

#include <QWidget>
#include <QString>
#include <QPointer>


class OverlayWidgetImpl;
class DataObject;

class OverlayWidget : public QWidget {
    Q_OBJECT
public:
    OverlayWidget(QWidget *parent=0);
    ~OverlayWidget();
private:
    OverlayWidgetImpl *impl;
    void printDO();
    friend class OverlayWidgetImpl;
public slots:
    void detailsChanged(DataObject *ptr);
};


#endif // OVERLAYWIDGET_H
