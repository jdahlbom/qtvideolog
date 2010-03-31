#ifndef SIMPLELISTMODEL_H
#define SIMPLELISTMODEL_H

#include <QAbstractListModel>
#include <QObject>

class DataObject;

class SimpleListModel : public QAbstractListModel {
    Q_OBJECT
public:
    SimpleListModel(QObject *parent=0);
    ~SimpleListModel();

    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const;
    int rowCount(const QModelIndex &parent = QModelIndex()) const;

signals:
    void detailObjectChanged(DataObject *doPtr);
    void videoLaunchRequested(QString videoSource);

public slots:
    void requestDetails(int index);
    void requestVideoLaunch(int index);

private:
    Q_DISABLE_COPY(SimpleListModel);
    QList<DataObject *> m_items;
    static const int BackgroundRole;
};

#endif // SIMPLELISTMODEL_H
