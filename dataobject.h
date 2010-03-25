#ifndef DATAOBJECT_H
#define DATAOBJECT_H

#include <QObject>
#include <QString>
#include <QColor>
#include <QUrl>

class DataObject : public QObject {
    Q_OBJECT

    Q_PROPERTY(QString name READ name)
    Q_PROPERTY(QColor color READ color)
    Q_PROPERTY(QUrl url READ url)

public:
    DataObject(QObject *parent = 0);

    DataObject(const QString &name,
               const QColor &color,
               const QUrl &url,
               QObject *parent = 0);

    QUrl url() const;
    QColor color() const;
    QString name() const;
private:
    QString m_name;
    QColor m_color;
    QUrl m_url;
}; // class DataObject


#endif // DATAOBJECT_H
