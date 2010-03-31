#include "dataobject.h"

DataObject::DataObject(QObject *parent) :
        QObject(parent) {}

DataObject::DataObject(const QString &name,
                       const QColor &color,
                       const QUrl &url,
                       const QString &videoSrc,
                       QObject *parent) :
QObject(parent),
m_name(name),
m_color(color),
m_url(url),
m_description(""),
m_videoSource(videoSrc)
{}

DataObject & DataObject::operator= (const DataObject &rhs) {
    setName(rhs.m_name);
    setColor(rhs.m_color);
    setUrl(rhs.m_url);
    setDescription(rhs.m_description);
    setVideoSource(rhs.m_videoSource);
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
    if (m_color == newColor)
        return;
    m_color = newColor;
    emit colorChanged();
}

void DataObject::setUrl(const QUrl &newUrl) {
    if (m_url == newUrl)
        return;
    m_url = newUrl;
    emit urlChanged();
}

void DataObject::setDescription(const QString &desc) {
    if (m_description == desc)
        return;
    m_description = desc;
    emit descriptionChanged();
}

void DataObject::setVideoSource(const QString &videoSrc) {
    if (m_videoSource == videoSrc)
        return;
    m_videoSource = videoSrc;
    emit videoSourceChanged();
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

QString DataObject::videoSource() const {
    return m_videoSource;
}
