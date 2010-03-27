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
m_url(url),
m_description("")
{}

DataObject & DataObject::operator= (const DataObject &rhs) {
    setName(rhs.m_name);
    setColor(rhs.m_color);
    setUrl(rhs.m_url);
    setDescription(rhs.m_description);
    return *this;
}

void DataObject::print() {
    qDebug("DataObject:");
    qDebug(m_name.toLatin1().data());
    qDebug(QString("%1,%2,%3").arg(m_color.red()).arg(m_color.green()).arg(m_color.blue()).toLatin1().data());
    qDebug(m_url.path().toLatin1().data());
    qDebug(m_description.toLatin1().data());
}

void DataObject::setName(const QString &newName) {
    m_name = newName;
    emit nameChanged();
}

void DataObject::setColor(const QColor &newColor) {
    m_color = newColor;
    emit colorChanged();
}

void DataObject::setUrl(const QUrl &newUrl) {
    m_url = newUrl;
    emit urlChanged();
}

void DataObject::setDescription(const QString &desc) {
    m_description = desc;
    emit descriptionChanged();
}

QString DataObject::description() const {
    return m_description;
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
