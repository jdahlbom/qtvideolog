#include "simplelistmodel.h"

#include <QVariant>
#include <QObject>
#include <QAbstractListModel>

#include "dataobject.h"

const int SimpleListModel::BackgroundRole = Qt::UserRole + 1;

SimpleListModel::SimpleListModel(QObject *parent) :
        QAbstractListModel(parent) {

    DataObject *first = new DataObject(QString("Buzz Aldrin step on Moon (video) - Apollo 11"), QColor(150,255,150), QUrl("media/aldrin.jpg"), QString("media/aldrin.ogg"), this);
    first->setDescription(QString("Television clip of Buzz descending the ladder and stepping onto the moon. On arriving at the footpad, he tries to jump up to the ladder again and fails, but succeeds on his second attempt. After remarking to Neil on the beautiful view ('Magnificent desolation') he jumps from the footpad onto the surface."));
    DataObject *second = new DataObject(QString("Impact movie"), QColor(255,50,255), QUrl("media/impact.jpg"), QString("media/Impact_movie.ogg"), this);
    second->setDescription(QString("High-speed digital sequence of a vertical impact by a copper sphere traveling at 4.5 km/sec into porous pumice (density of about 1g/cc). A side-view of a near-vertical impact at 500 frames per second (or 2 milliseconds between each frame) taken with a high-speed video. This is a 60-degree impact (from horizontal) into a highly porous target of fine particles."));
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
