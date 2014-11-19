#include "guhtuneui.h"

#include <QQuickView>
#include <QQmlEngine>
#include <QQmlContext>
#include <QtQml>
#include <QDebug>

GuhTuneUi::GuhTuneUi(QObject *parent) :
    QObject(parent)
{
    qmlRegisterUncreatableType<GuhTuneUi>("guhtune", 1, 0, "GuhTuneUi", "can't create one. use \"controller\".");
    m_view = new QQuickView();
    m_view->engine()->addImportPath(QLatin1String("modules"));
    m_view->engine()->rootContext()->setContextProperty("controller", this);
    m_view->setSource(QUrl(QLatin1String("qrc:///guhtune-ui/main.qml")));
    m_view->setResizeMode(QQuickView::SizeRootObjectToView);
    m_view->showFullScreen();
    //m_view->show();
}

GuhTuneUi::~GuhTuneUi()
{
    m_view->deleteLater();
}

