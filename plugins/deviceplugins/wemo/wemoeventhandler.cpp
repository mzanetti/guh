#include "wemoeventhandler.h"

WemoEventHandler::WemoEventHandler(QObject *parent, QHostAddress host, int port) :
    QTcpServer(parent), m_host(host), m_port(port)
{
    listen(QHostAddress::AnyIPv4,m_port);
    m_disabled = false;
    m_expectingData = false;
    qDebug() << "Wemo event handler enabled " << m_host.toString() << m_port;
}

void WemoEventHandler::incomingConnection(qintptr socket)
{
    if(m_disabled){
        return;
    }

    QTcpSocket* tcpSocket = new QTcpSocket(this);
    connect(tcpSocket, &QTcpSocket::readyRead, this, &WemoEventHandler::readClient);
    connect(tcpSocket, &QTcpSocket::disconnected, this, &WemoEventHandler::discardClient);
    tcpSocket->setSocketDescriptor(socket);
    qDebug() << "incomming connection" << tcpSocket->peerAddress().toString();

}

void WemoEventHandler::readClient()
{
    if(m_disabled){
        return;
    }

    QTcpSocket* socket = (QTcpSocket*)sender();

    // reject everything, except the tv
    if(socket->peerAddress() != m_host){
        qWarning() << "reject connection from " << socket->peerAddress().toString();
        socket->close();
        if (socket->state() == QTcpSocket::UnconnectedState) {
            delete socket;
        }
        return;
    }


    // the tv sends first the header (POST /udap/api/.... HTTP/1.1)
    // in the scond package the tv sends the information (xml format)
    while(!socket->atEnd()){
        QByteArray data = socket->readAll();
        qDebug() << data;
        //        // check if we got information
        //        if(data.startsWith("<?xml") && m_expectingData){
        //            m_expectingData = false;

        //            // Answere with OK
        //            QTextStream textStream(socket);
        //            textStream.setAutoDetectUnicode(true);
        //            textStream << "HTTP/1.0 200 OK\r\n"
        //                          "Content-Type: text/html; charset=\"utf-8\"\r\n"
        //                          "User-Agent: UDAP/2.0 guh\r\n"
        //                       << "Date: " << QDateTime::currentDateTime().toString() << "\n";

        //            emit eventOccured(data);
        //        }

        //        // check if we got header
        //        if (data.startsWith("POST") && !m_expectingData) {
        //            m_expectingData = true;
        ////            QStringList tokens = QString(data).split(QRegExp("[ \r\n][ \r\n]*"));
        ////            qDebug() << "==================================";
        ////            qDebug() << "event occured" << "http://" << m_host.toString() << ":" << m_port << tokens[1];
        //        }
    }
}

void WemoEventHandler::discardClient()
{
    QTcpSocket* socket = (QTcpSocket*)sender();
    socket->deleteLater();
}

void WemoEventHandler::enable()
{
    m_disabled = false;
}

void WemoEventHandler::disable()
{
    m_disabled = true;
}
