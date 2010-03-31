#include "simplelistmodel.h"

#include <QVariant>
#include <QObject>
#include <QAbstractListModel>

#include "dataobject.h"

const int SimpleListModel::BackgroundRole = Qt::UserRole + 1;

SimpleListModel::SimpleListModel(QObject *parent) :
        QAbstractListModel(parent) {

    DataObject *first = new DataObject(QString("First text"), QColor(150,255,150), QUrl("jdahlbom.jpg"), QString("media/Impact_movie.ogg"), this);
    first->setDescription(QString("This is the description of the first item"));
    DataObject *second = new DataObject(QString("Second text"), QColor(255,50,255), QUrl("jdahlbom.jpg"), QString("media/Illusion_movie.ogg"), this);
    second->setDescription(QString("Second item has a very different description"));
    m_items.append(first);
    m_items.append(second);

    QHash<int, QByteArray> roles;
    roles.insert(Qt::DisplayRole, QByteArray("text"));
    roles.insert(Qt::DecorationRole, QByteArray("image"));
    roles.insert(BackgroundRole, QByteArray("bgColor"));
    setRoleNames(roles);
}

SimpleListModel::~SimpleListModel() {}

int SimpleListModel::rowCount(const QModelIndex &/*parent = QModelIndex()*/) const {
    return m_items.size();
}

QVariant SimpleListModel::data(const QModelIndex &index, int role) const {
    if (!index.isValid())
        return QVariant();
    if (index.row() > (m_items.size()-1) )
        return QVariant();

    DataObject *dobj = m_items.at(index.row());
    switch (role) {
    case Qt::DisplayRole:
        return QVariant::fromValue(dobj->name());
    case Qt::DecorationRole:
        return QVariant::fromValue(dobj->url());
    case BackgroundRole:
        return QVariant::fromValue(dobj->color());
    default:
        return QVariant();
    }
}

void SimpleListModel::requestDetails(int index) {
    if (index < 0 || index > m_items.size()-1)
        return;
    emit detailObjectChanged(m_items.at(index));
}

void SimpleListModel::requestVideoLaunch(int index) {
    if (index < 0 || index > m_items.size()-1)
        return;
    emit videoLaunchRequested(m_items.at(index)->videoSource());
}
