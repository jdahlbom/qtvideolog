#ifndef DATAOBJECT_H
#define DATAOBJECT_H

#include <QObject>
#include <QString>
#include <QColor>
#include <QUrl>

class DataObject : public QObject {
    Q_OBJECT

    Q_PROPERTY(QString name READ name WRITE setName NOTIFY nameChanged)
    Q_PROPERTY(QColor color READ color WRITE setColor NOTIFY colorChanged)
    Q_PROPERTY(QUrl url READ url WRITE setUrl NOTIFY urlChanged)
    Q_PROPERTY(QString description READ description WRITE setDescription NOTIFY descriptionChanged)
    Q_PROPERTY(QString videoSource READ videoSource WRITE setVideoSource NOTIFY videoSourceChanged)

public:
    DataObject(QObject *parent = 0);
    DataObject & operator= (const DataObject &rhs);

    DataObject(const QString &name,
               const QColor &color,
               const QUrl &imageUrl,
               const QString &videoSrc,
               QObject *parent = 0);

    void dummyPrint() {
        qDebug("Dummy printing");
    }

    void print();

    void setName(const QString &newName);
    void setColor(const QColor &newColor);
    void setUrl(const QUrl &newUrl);
    void setDescription(const QString &desc);
    void setVideoSource(const QString &source);

    QString description() const;
    QUrl url() const;
    QColor color() const;
    QString name() const;
    QString videoSource() const;
signals:
    void nameChanged();
    void colorChanged();
    void urlChanged();
    void descriptionChanged();
    void videoSourceChanged();
private:
    QString m_name;
    QColor m_color;
    QUrl m_url;
    QString m_description;
    QString m_videoSource;
}; // class DataObject

#endif // DATAOBJECT_H
