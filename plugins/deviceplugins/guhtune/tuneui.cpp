#include "tuneui.h"

#include <QDeclarativeView>
#include <QDeclarativeComponent>
#include <QDeclarativeEngine>
#include <QDeclarativeContext>
#include <QDebug>

TuneUi::TuneUi(QObject *parent) :
    QObject(parent)
{
    qmlRegisterUncreatableType<TuneUi>("guhtune", 1, 0, "TuneUi", "can't create one. use \"controller\".");
    m_view = new QDeclarativeView();
    m_view->engine()->addImportPath(QLatin1String("modules"));
    m_view->engine()->rootContext()->setContextProperty("controller", this);
    m_view->setSource(QUrl(QLatin1String("qrc:///guhtune-ui/main.qml")));
    m_view->setResizeMode(QDeclarativeView::SizeRootObjectToView);
    m_view->showFullScreen();

}

TuneUi::~TuneUi()
{
    m_view->deleteLater();
}

