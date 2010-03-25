#include "dataobject.h"

DataObject::DataObject(QObject *parent) :
        QObject(parent) {}

DataObject::DataObject(const QString &name,
                       const QColor &color,
                       const QUrl &url,
                       QObject *parent) :
QObject(parent),
m_name(name),
m_color(color),
m_url(url)
{
}

QUrl DataObject::url() const {
    return m_url;
}

QColor DataObject::color() const {
    return m_color;
}

QString DataObject::name() const {
    return m_name;
}
