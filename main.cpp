#include <QtGui/QApplication>
#include "overlaywidget.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    OverlayWidget overlay;
    overlay.show();
    return a.exec();
}
