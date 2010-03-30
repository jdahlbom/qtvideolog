/****************************************************************************
**
** Copyright (C) 2010 Nokia Corporation and/or its subsidiary(-ies).
** All rights reserved.
** Contact: Nokia Corporation (qt-info@nokia.com)
**
** This file is part of the QtDeclarative module of the Qt Toolkit.
**
** $QT_BEGIN_LICENSE:LGPL$
** No Commercial Usage
** This file contains pre-release code and may not be distributed.
** You may use this file in accordance with the terms and conditions
** contained in the Technology Preview License Agreement accompanying
** this package.
**
** GNU Lesser General Public License Usage
** Alternatively, this file may be used under the terms of the GNU Lesser
** General Public License version 2.1 as published by the Free Software
** Foundation and appearing in the file LICENSE.LGPL included in the
** packaging of this file.  Please review the following information to
** ensure the GNU Lesser General Public License version 2.1 requirements
** will be met: http://www.gnu.org/licenses/old-licenses/lgpl-2.1.html.
**
** In addition, as a special exception, Nokia gives you certain additional
** rights.  These rights are described in the Nokia Qt LGPL Exception
** version 1.1, included in the file LGPL_EXCEPTION.txt in this package.
**
** If you have questions regarding the use of this file, please contact
** Nokia at qt-info@nokia.com.
**
**
**
**
**
**
**
**
** $QT_END_LICENSE$
**
****************************************************************************/

/**
  * Modified by Jukka Dahlbom to expose the scene parameter.
  */

#include "modifiedqdeclarativeview.h"

#include <qdeclarative.h>
#include <qdeclarativeitem.h>
#include <qdeclarativeengine.h>
#include <qdeclarativecontext.h>

#include <qgraphicsview.h>


#include <qdebug.h>
#include <qtimer.h>
#include <qevent.h>
#include <qdir.h>
#include <qcoreapplication.h>
#include <qfontdatabase.h>
#include <qicon.h>
#include <qurl.h>
#include <qboxlayout.h>
#include <qbasictimer.h>
#include <QtCore/qabstractanimation.h>

QT_BEGIN_NAMESPACE


class ModifiedQDeclarativeViewDebugServer;

class ModifiedQDeclarativeViewPrivate
{
public:
    ModifiedQDeclarativeViewPrivate(ModifiedQDeclarativeView *view)
        : q(view), root(0), component(0), resizeMode(ModifiedQDeclarativeView::SizeViewToRootObject) {}

    void execute();

    ModifiedQDeclarativeView *q;

    QGraphicsObject *root;
    QDeclarativeItem *qmlRoot;

    QUrl source;

    QDeclarativeEngine engine;
    QDeclarativeComponent *component;
    QBasicTimer resizetimer;

    mutable QSize initialSize;
    ModifiedQDeclarativeView::ResizeMode resizeMode;
    QTime frameTimer;

    void init();

    QGraphicsScene scene;
};

void ModifiedQDeclarativeViewPrivate::execute()
{
    delete root;
    delete component;
    component = new QDeclarativeComponent(&engine, source, q);

    if (!component->isLoading()) {
        q->continueExecute();
    } else {
        QObject::connect(component, SIGNAL(statusChanged(QDeclarativeComponent::Status)), q, SLOT(continueExecute()));
    }
}


/*!
    \class ModifiedQDeclarativeView
  \since 4.7
    \brief The ModifiedQDeclarativeView class provides a widget for displaying a Qt Declarative user interface.

    Any QGraphicsObject or QDeclarativeItem
    created via QML can be placed on a standard QGraphicsScene and viewed with a standard
    QGraphicsView.

    ModifiedQDeclarativeView is a QGraphicsView subclass provided as a convenience for displaying QML
    files, and connecting between QML and C++ Qt objects.

    ModifiedQDeclarativeView performs the following functions:

    \list
    \o Manages QDeclarativeComponent loading and object creation.
    \o Initializes QGraphicsView for optimal performance with QML:
        \list
        \o QGraphicsView::setOptimizationFlags(QGraphicsView::DontSavePainterState);
        \o QGraphicsView::setViewportUpdateMode(QGraphicsView::BoundingRectViewportUpdate);
        \o QGraphicsScene::setItemIndexMethod(QGraphicsScene::NoIndex);
        \endlist
    \o Initializes QGraphicsView for QML key handling:
        \list
        \o QGraphicsView::viewport()->setFocusPolicy(Qt::NoFocus);
        \o QGraphicsScene::setStickyFocus(true);
        \endlist
    \endlist

    Typical usage:
    \code
    ...
    ModifiedQDeclarativeView *view = new ModifiedQDeclarativeView(this);
    vbox->addWidget(view);

    QUrl url(fileName);
    view->setSource(url);
    view->show();
    \endcode

    To receive errors related to loading and executing QML with ModifiedQDeclarativeView,
    you can connect to the statusChanged() signal and monitor for ModifiedQDeclarativeView::Error.
    The errors are available via ModifiedQDeclarativeView::errors().
*/


/*! \fn void ModifiedQDeclarativeView::sceneResized(QSize size)
  This signal is emitted when the view is resized to \a size.
*/

/*! \fn void ModifiedQDeclarativeView::statusChanged(ModifiedQDeclarativeView::Status status)
    This signal is emitted when the component's current \l{ModifiedQDeclarativeView::Status} {status} changes.
*/

/*!
  \fn ModifiedQDeclarativeView::ModifiedQDeclarativeView(QWidget *parent)

  Constructs a ModifiedQDeclarativeView with the given \a parent.
*/
ModifiedQDeclarativeView::ModifiedQDeclarativeView(QWidget *parent)
: QGraphicsView(parent), d(new ModifiedQDeclarativeViewPrivate(this))
{
    setSizePolicy(QSizePolicy::Preferred,QSizePolicy::Preferred);
    d->init();
}

/*!
  \fn ModifiedQDeclarativeView::ModifiedQDeclarativeView(const QUrl &source, QWidget *parent)

  Constructs a ModifiedQDeclarativeView with the given QML \a source and \a parent.
*/
ModifiedQDeclarativeView::ModifiedQDeclarativeView(const QUrl &source, QWidget *parent)
: QGraphicsView(parent), d(new ModifiedQDeclarativeViewPrivate(this))
{
    setSizePolicy(QSizePolicy::Preferred,QSizePolicy::Preferred);
    d->init();
    setSource(source);
}

void ModifiedQDeclarativeViewPrivate::init()
{
#ifdef Q_ENABLE_PERFORMANCE_LOG
    {
        QDeclarativePerfTimer<QDeclarativePerf::FontDatabase> perf;
        QFontDatabase database;
    }
#endif

    q->setScene(&scene);

    q->setOptimizationFlags(QGraphicsView::DontSavePainterState);
    q->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    q->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    q->setFrameStyle(0);

    // These seem to give the best performance
    q->setViewportUpdateMode(QGraphicsView::BoundingRectViewportUpdate);
    scene.setItemIndexMethod(QGraphicsScene::NoIndex);
    q->viewport()->setFocusPolicy(Qt::NoFocus);

    scene.setStickyFocus(true);  //### needed for correct focus handling
}

/*!
  The destructor clears the view's \l {QGraphicsObject} {items} and
  deletes the internal representation.
 */
ModifiedQDeclarativeView::~ModifiedQDeclarativeView()
{
    delete d->root;
}

/*!
    Sets the source to the \a url, loads the QML component and instantiates it.
 */
void ModifiedQDeclarativeView::setSource(const QUrl& url)
{
    if (url != d->source) {
        d->source = url;
        d->execute();
    }
}

/*!
  Returns the source URL, if set.

  \sa setSource()
 */
QUrl ModifiedQDeclarativeView::source() const
{
    return d->source;
}

QGraphicsScene *ModifiedQDeclarativeView::scene() const {
    return &(d->scene);
}

/*!
  Returns a pointer to the QDeclarativeEngine used for instantiating
  QML Components.
 */
QDeclarativeEngine* ModifiedQDeclarativeView::engine()
{
    return &d->engine;
}

/*!
  This function returns the root of the context hierarchy.  Each QML
  component is instantiated in a QDeclarativeContext.  QDeclarativeContext's are
  essential for passing data to QML components.  In QML, contexts are
  arranged hierarchically and this hierarchy is managed by the
  QDeclarativeEngine.
 */
QDeclarativeContext* ModifiedQDeclarativeView::rootContext()
{
    return d->engine.rootContext();
}


/*!
  \enum ModifiedQDeclarativeView::Status

    Specifies the loading status of the ModifiedQDeclarativeView.

    \value Null This ModifiedQDeclarativeView has no source set.
    \value Ready This ModifiedQDeclarativeView has loaded and created the QML component.
    \value Loading This ModifiedQDeclarativeView is loading network data.
    \value Error An error has occured.  Calling errorDescription() to retrieve a description.
*/

/*!
    \property ModifiedQDeclarativeView::status
    The component's current \l{ModifiedQDeclarativeView::Status} {status}.
*/

ModifiedQDeclarativeView::Status ModifiedQDeclarativeView::status() const
{
    if (!d->component)
        return ModifiedQDeclarativeView::Null;

    return ModifiedQDeclarativeView::Status(d->component->status());
}

/*!
    Return the list of errors that occured during the last compile or create
    operation.  An empty list is returned if isError() is not set.
*/
QList<QDeclarativeError> ModifiedQDeclarativeView::errors() const
{
    if (d->component)
        return d->component->errors();
    return QList<QDeclarativeError>();
}


/*!
    \property ModifiedQDeclarativeView::resizeMode
    \brief whether the view should resize the canvas contents

    If this property is set to SizeViewToRootObject (the default), the view
    resizes with the root item in the QML.

    If this property is set to SizeRootObjectToView, the view will
    automatically resize the root item.

    Regardless of this property, the sizeHint of the view
    is the initial size of the root item. Note though that
    since QML may load dynamically, that size may change.

    \sa initialSize()
*/

void ModifiedQDeclarativeView::setResizeMode(ResizeMode mode)
{
    if (d->resizeMode == mode)
        return;

    d->resizeMode = mode;
    if (d->qmlRoot) {
        if (d->resizeMode == SizeRootObjectToView) {
            d->qmlRoot->setWidth(width());
            d->qmlRoot->setHeight(height());
        } else {
            d->qmlRoot->setWidth(d->initialSize.width());
            d->qmlRoot->setHeight(d->initialSize.height());
        }
    }
}

ModifiedQDeclarativeView::ResizeMode ModifiedQDeclarativeView::resizeMode() const
{
    return d->resizeMode;
}

/*!
  \internal
 */
void ModifiedQDeclarativeView::continueExecute()
{

    disconnect(d->component, SIGNAL(statusChanged(QDeclarativeComponent::Status)), this, SLOT(continueExecute()));

    if (d->component->isError()) {
        QList<QDeclarativeError> errorList = d->component->errors();
        foreach (const QDeclarativeError &error, errorList) {
            qWarning() << error;
        }
        emit statusChanged(status());
        return;
    }

    QObject *obj = d->component->create();

    if(d->component->isError()) {
        QList<QDeclarativeError> errorList = d->component->errors();
        foreach (const QDeclarativeError &error, errorList) {
            qWarning() << error;
        }
        emit statusChanged(status());
        return;
    }

    setRootObject(obj);
    emit statusChanged(status());
}


/*!
  \internal
*/
void ModifiedQDeclarativeView::setRootObject(QObject *obj)
{
    if (QDeclarativeItem *item = qobject_cast<QDeclarativeItem *>(obj)) {
        d->scene.addItem(item);

        d->root = item;
        d->qmlRoot = item;
        connect(item, SIGNAL(widthChanged()), this, SLOT(sizeChanged()));
        connect(item, SIGNAL(heightChanged()), this, SLOT(sizeChanged()));
        if (d->initialSize.height() <= 0 && d->qmlRoot->width() > 0)
            d->initialSize.setWidth(d->qmlRoot->width());
        if (d->initialSize.height() <= 0 && d->qmlRoot->height() > 0)
            d->initialSize.setHeight(d->qmlRoot->height());
        resize(d->initialSize);

        if (d->resizeMode == SizeRootObjectToView) {
            d->qmlRoot->setWidth(width());
            d->qmlRoot->setHeight(height());
        } else {
            QSize sz(d->qmlRoot->width(),d->qmlRoot->height());
            emit sceneResized(sz);
            resize(sz);
        }
        updateGeometry();
    } else if (QGraphicsObject *item = qobject_cast<QGraphicsObject *>(obj)) {
        d->scene.addItem(item);
        qWarning() << "ModifiedQDeclarativeView::resizeMode is not honored for components of type QGraphicsObject";
    } else if (QWidget *wid = qobject_cast<QWidget *>(obj)) {
        window()->setAttribute(Qt::WA_OpaquePaintEvent, false);
        window()->setAttribute(Qt::WA_NoSystemBackground, false);
        if (!layout()) {
            setLayout(new QVBoxLayout);
            layout()->setContentsMargins(0, 0, 0, 0);
        } else if (layout()->count()) {
            // Hide the QGraphicsView in GV mode.
            QLayoutItem *item = layout()->itemAt(0);
            if (item->widget())
                item->widget()->hide();
        }
        layout()->addWidget(wid);
        emit sceneResized(wid->size());
    }
}

/*!
  \internal
 */
void ModifiedQDeclarativeView::sizeChanged()
{
    // delay, so we catch both width and height changing.
    d->resizetimer.start(0,this);
}

/*!
  \internal
  If the \l {QTimerEvent} {timer event} \a e is this
  view's resize timer, sceneResized() is emitted.
 */
void ModifiedQDeclarativeView::timerEvent(QTimerEvent* e)
{
    if (!e || e->timerId() == d->resizetimer.timerId()) {
        if (d->qmlRoot) {
            QSize sz(d->qmlRoot->width(),d->qmlRoot->height());
            emit sceneResized(sz);
            //if (!d->resizable)
                //resize(sz);
        }
        d->resizetimer.stop();
        updateGeometry();
    }
}

/*!
    \internal
    The size hint is the size of the root item.
*/
QSize ModifiedQDeclarativeView::sizeHint() const
{
    if (d->qmlRoot) {
        if (d->initialSize.width() <= 0)
            d->initialSize.setWidth(d->qmlRoot->width());
        if (d->initialSize.height() <= 0)
            d->initialSize.setHeight(d->qmlRoot->height());
    }
    return d->initialSize;
}

/*!
  Returns the view's root \l {QGraphicsObject} {item}.
 */
QGraphicsObject *ModifiedQDeclarativeView::rootObject() const
{
    return d->root;
}

/*!
  \internal
  This function handles the \l {QResizeEvent} {resize event}
  \a e.
 */
void ModifiedQDeclarativeView::resizeEvent(QResizeEvent *e)
{
    if (d->resizeMode == SizeRootObjectToView && d->qmlRoot) {
        d->qmlRoot->setWidth(width());
        d->qmlRoot->setHeight(height());
    }
    if (d->qmlRoot) {
        setSceneRect(QRectF(0, 0, d->qmlRoot->width(), d->qmlRoot->height()));
    } else if (d->root) {
        setSceneRect(d->root->boundingRect());
    } else {
        setSceneRect(rect());
    }
    QGraphicsView::resizeEvent(e);
}

/*!
    \internal
*/
void ModifiedQDeclarativeView::paintEvent(QPaintEvent *event)
{
    QGraphicsView::paintEvent(event);
}

QT_END_NAMESPACE

