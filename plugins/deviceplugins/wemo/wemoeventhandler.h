#ifndef WEMOEVENTHANDLER_H
#define WEMOEVENTHANDLER_H

#include <QObject>
#include <QTcpServer>
#include <QNetworkAccessManager>

class WemoEventHandler : public QTcpServer
{
    Q_OBJECT
public:
    explicit WemoEventHandler(QObject *parent = 0, QHostAddress host = QHostAddress(), int port = 8080);
    void incomingConnection(qintptr socket) override;

private:
    QHostAddress m_host;
    int m_port;
    bool m_disabled;

    bool m_expectingData;

    QNetworkAccessManager *m_manager;

signals:
    void eventOccured(const QByteArray &path);
    void byebyeEvent();

private slots:
    void readClient();
    void discardClient();

public slots:
    void enable();
    void disable();
};

#endif // WEMOEVENTHANDLER_H
