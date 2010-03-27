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

public:
    DataObject(QObject *parent = 0);
    DataObject & operator= (const DataObject &rhs);

    DataObject(const QString &name,
               const QColor &color,
               const QUrl &url,
               QObject *parent = 0);

    void dummyPrint() {
        qDebug("Dummy printing");
    }

    void print();

    void setName(const QString &newName);
    void setColor(const QColor &newColor);
    void setUrl(const QUrl &newUrl);
    void setDescription(const QString &desc);

    QString description() const;
    QUrl url() const;
    QColor color() const;
    QString name() const;
signals:
    void nameChanged();
    void colorChanged();
    void urlChanged();
    void descriptionChanged();
private:
    QString m_name;
    QColor m_color;
    QUrl m_url;
    QString m_description;
}; // class DataObject

#endif // DATAOBJECT_H
