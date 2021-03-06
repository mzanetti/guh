/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 *                                                                         *
 *  This file is part of guh.                                              *
 *                                                                         *
 *  Guh is free software: you can redistribute it and/or modify            *
 *  it under the terms of the GNU General Public License as published by   *
 *  the Free Software Foundation, version 2 of the License.                *
 *                                                                         *
 *  Guh is distributed in the hope that it will be useful,                 *
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of         *
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the          *
 *  GNU General Public License for more details.                           *
 *                                                                         *
 *  You should have received a copy of the GNU General Public License      *
 *  along with guh. If not, see <http://www.gnu.org/licenses/>.            *
 *                                                                         *
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#include "tvdevice.h"

TvDevice::TvDevice(QObject *parent) :
    QObject(parent)
{
    m_manager = new QNetworkAccessManager(this);

    // TODO: make dynamic...displayPin setup
    m_key = "539887";

    m_pairingStatus = false;
    m_reachable = false;

    connect(m_manager, &QNetworkAccessManager::finished, this, &TvDevice::replyFinished);
}

void TvDevice::setLocation(const QUrl &location)
{
    m_location = location;
}

QUrl TvDevice::location() const
{
    return m_location;
}

void TvDevice::setHostAddress(const QHostAddress &hostAddress)
{
    m_hostAddress = hostAddress;
}

QHostAddress TvDevice::hostAddress() const
{
    return m_hostAddress;
}

void TvDevice::setPort(const int &port)
{
    m_port = port;
}

int TvDevice::port() const
{
    return m_port;
}

void TvDevice::setName(const QString &name)
{
    m_name = name;
}

QString TvDevice::name() const
{
    return m_name;
}

void TvDevice::setModelName(const QString &modelName)
{
    m_modelName = modelName;
}

QString TvDevice::modelName() const
{
    return m_modelName;
}

void TvDevice::setManufacturer(const QString &manufacturer)
{
    m_manufacturer = manufacturer;
}

QString TvDevice::manufacturer() const
{
    return m_manufacturer;
}

void TvDevice::setDeviceType(const QString &deviceType)
{
    m_deviceType = deviceType;
}

QString TvDevice::deviceType() const
{
    return m_deviceType;
}

void TvDevice::setUuid(const QString &uuid)
{
    m_uuid = uuid;
}

QString TvDevice::uuid() const
{
    return m_uuid;
}

void TvDevice::setKey(const QString &key)
{
    m_key = key;
}

QString TvDevice::key() const
{
    return m_key;
}

bool TvDevice::reachable() const
{
    return m_reachable;
}

bool TvDevice::paired() const
{
    return m_pairingStatus;
}

bool TvDevice::is3DMode() const
{
    return m_is3DMode;
}

int TvDevice::volumeLevel() const
{
    return m_volumeLevel;
}

bool TvDevice::mute() const
{
    return m_mute;
}

QString TvDevice::channelType() const
{
    return m_channelType;
}

QString TvDevice::channelName() const
{
    return m_channelName;
}

int TvDevice::channelNumber() const
{
    return m_channelNumber;
}

QString TvDevice::programName() const
{
    return m_programName;
}

int TvDevice::inputSourceIndex() const
{
    return m_inputSourceIndex;
}

QString TvDevice::inputSourceLabelName() const
{
    return m_inputSourceLabel;
}

void TvDevice::showPairingKey()
{
    QString urlString = "http://" + m_hostAddress.toString() + ":" + QString::number(m_port) + "/udap/api/pairing";

    QNetworkRequest request;
    request.setUrl(QUrl(urlString));
    request.setHeader(QNetworkRequest::ContentTypeHeader,QVariant("text/xml; charset=utf-8"));
    request.setHeader(QNetworkRequest::UserAgentHeader,QVariant("UDAP/2.0 guh"));

    QByteArray data = "<?xml version=\"1.0\" encoding=\"utf-8\"?><envelope><api type=\"pairing\"> <name>showKey</name></api></envelope>";

    m_showKeyReplay = m_manager->post(request,data);
}

void TvDevice::requestPairing()
{
    if(m_key.isNull()){
        emit pairingFinished(false);
    }

    QString urlString = "http://" + m_hostAddress.toString()  + ":" + QString::number(m_port) + "/udap/api/pairing";

    QNetworkRequest request;
    request.setUrl(QUrl(urlString));
    request.setHeader(QNetworkRequest::ContentTypeHeader,QVariant("text/xml; charset=utf-8"));
    request.setHeader(QNetworkRequest::UserAgentHeader,QVariant("UDAP/2.0 guh"));

    QByteArray data = "<?xml version=\"1.0\" encoding=\"utf-8\"?><envelope><api type=\"pairing\"><name>hello</name><value>" + m_key.toUtf8() + "</value><port>8080</port></api></envelope>";

    m_requestPairingReplay = m_manager->post(request,data);
}

void TvDevice::endPairing()
{
    QString urlString = "http://" + m_hostAddress.toString()  + ":" + QString::number(m_port) + "/udap/api/pairing";

    QNetworkRequest request;
    request.setUrl(QUrl(urlString));
    request.setHeader(QNetworkRequest::ContentTypeHeader,QVariant("text/xml; charset=utf-8"));
    request.setHeader(QNetworkRequest::UserAgentHeader,QVariant("UDAP/2.0 guh"));
    request.setRawHeader("Connection", "Close");

    QByteArray data = "<?xml version=\"1.0\" encoding=\"utf-8\"?><envelope><api type=\"pairing\"><name>byebye</name><port>8080</port></api></envelope>";

    m_finishingPairingReplay = m_manager->post(request,data);
}


void TvDevice::sendCommand(TvDevice::RemoteKey key, ActionId actionId)
{
    m_actionId = actionId;

    if(!m_pairingStatus){
        requestPairing();
        return;
    }

    QString urlString = "http://" + m_hostAddress.toString()  + ":" + QString::number(m_port) + "/udap/api/command";

    QByteArray data;
    data.append("<?xml version=\"1.0\" encoding=\"utf-8\"?><envelope><api type=\"command\"><name>HandleKeyInput</name><value>");
    data.append(QString::number(key).toUtf8());
    data.append("</value></api></envelope>");

    QNetworkRequest request;
    request.setUrl(QUrl(urlString));
    request.setHeader(QNetworkRequest::ContentTypeHeader,QVariant("text/xml; charset=utf-8"));
    request.setHeader(QNetworkRequest::UserAgentHeader,QVariant("UDAP/2.0 guh"));

    m_sendCommandReplay = m_manager->post(request,data);
}

void TvDevice::setupEventHandler()
{
    //qDebug() << "set up event handler " << m_hostAddress.toString() << m_port;
    m_eventHandler = new TvEventHandler(this,m_hostAddress,m_port);
    connect(m_eventHandler, &TvEventHandler::eventOccured, this, &TvDevice::eventOccured);
}

void TvDevice::refresh()
{
    if(paired()){
        queryChannelInformation();
        queryVolumeInformation();
    }
}

void TvDevice::queryVolumeInformation()
{
    QString urlString = "http://" + m_hostAddress.toString()  + ":" + QString::number(m_port) + "/udap/api/data?target=volume_info";

    QNetworkRequest request;
    request.setUrl(QUrl(urlString));
    request.setHeader(QNetworkRequest::ContentTypeHeader,QVariant("text/xml"));
    request.setHeader(QNetworkRequest::UserAgentHeader,QVariant("UDAP/2.0"));
    request.setRawHeader("Connection", "Close");

    m_queryVolumeInformationReplay = m_manager->get(request);
}

void TvDevice::queryChannelInformation()
{
    QString urlString = "http://" + m_hostAddress.toString()  + ":" + QString::number(m_port) + "/udap/api/data?target=cur_channel";

    QNetworkRequest deviceRequest;
    deviceRequest.setUrl(QUrl(urlString));
    deviceRequest.setHeader(QNetworkRequest::ContentTypeHeader,QVariant("text/xml"));
    deviceRequest.setHeader(QNetworkRequest::UserAgentHeader,QVariant("UDAP/2.0"));
    deviceRequest.setRawHeader("Connection", "Close");

    m_queryChannelInformationReplay = m_manager->get(deviceRequest);
}

void TvDevice::parseVolumeInformation(const QByteArray &data)
{
    //qDebug() << printXmlData(data);
    QXmlStreamReader xml(data);

    while(!xml.atEnd() && !xml.hasError()){
        xml.readNext();

        if(xml.name() == "mute"){
            m_mute = QVariant(xml.readElementText()).toBool();
        }
        if(xml.name() == "level"){
            m_volumeLevel = QVariant(xml.readElementText()).toInt();
        }
    }
    emit statusChanged();
}

void TvDevice::parseChannelInformation(const QByteArray &data)
{
    //qDebug() << printXmlData(data);
    QXmlStreamReader xml(data);

    while(!xml.atEnd() && !xml.hasError()){
        xml.readNext();

        if(xml.name() == "chtype"){
            m_channelType = xml.readElementText();
        }
        if(xml.name() == "major"){
            m_channelNumber = QVariant(xml.readElementText()).toInt();
        }
        if(xml.name() == "chname"){
            m_channelName = xml.readElementText();
        }
        if(xml.name() == "progName"){
            m_programName = xml.readElementText();
        }
        if(xml.name() == "inputSourceIdx"){
            m_inputSourceIndex = QVariant(xml.readElementText()).toInt();
        }
        if(xml.name() == "labelName"){
            m_inputSourceLabel = xml.readElementText();
        }
    }
    emit statusChanged();
}

QString TvDevice::printXmlData(QByteArray data)
{
    QString xmlOut;
    QXmlStreamReader reader(data);
    QXmlStreamWriter writer(&xmlOut);
    writer.setAutoFormatting(true);

    while (!reader.atEnd()) {
        reader.readNext();
        if (!reader.isWhitespace()) {
            writer.writeCurrentToken(reader);
        }
    }
    if(reader.hasError()){
        qDebug() << "ERROR reading XML device information:   " << reader.errorString();
        qDebug() << "--------------------------------------------";
    }
    return xmlOut;
}

void TvDevice::replyFinished(QNetworkReply *reply)
{
    int status = reply->attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt();

    if(status != 200){
        m_reachable = false;
    }else{
        m_reachable = true;
    }

    if(reply == m_showKeyReplay){
        if(status != 200){
            qWarning() << "ERROR: could not request to show pairing key on screen " << status;
        }
        m_showKeyReplay->deleteLater();
    }
    if(reply == m_requestPairingReplay){
        if(status != 200){
            m_pairingStatus = false;
            emit pairingFinished(false);
            qWarning() << "ERROR: could not pair with device" << status;
        }else{
            m_pairingStatus = true;
            qDebug() << "successfully paired with tv " << m_modelName;
            emit pairingFinished(true);
        }
        m_requestPairingReplay->deleteLater();
    }

    if(reply == m_finishingPairingReplay){
        if(status == 200){
            m_pairingStatus = false;
            qDebug() << "successfully unpaired from tv " << m_modelName;
        }
        m_finishingPairingReplay->deleteLater();
    }

    if(reply == m_sendCommandReplay){
        if(status != 200){
            emit sendCommandFinished(false,m_actionId);
            qWarning() << "ERROR: could not send comand" << status;
        }else{
            m_pairingStatus = true;
            qDebug() << "successfully sent command to tv " << m_modelName;
            emit sendCommandFinished(true,m_actionId);
            refresh();
        }
        m_sendCommandReplay->deleteLater();
    }
    if(reply == m_queryVolumeInformationReplay){
        parseVolumeInformation(reply->readAll());
        m_queryVolumeInformationReplay->deleteLater();
    }
    if(reply == m_queryChannelInformationReplay){
        parseChannelInformation(reply->readAll());
        m_queryChannelInformationReplay->deleteLater();
    }

    emit statusChanged();
}

void TvDevice::eventOccured(const QByteArray &data)
{
    // if we got a channel changed event...
    if(data.contains("ChannelChanged")){
        parseChannelInformation(data);
        return;
    }

//    qDebug() << "---------------------------------";
//    qDebug() << printXmlData(data);

    // if the tv suspends, it will send a byebye message, which means
    // the pairing will be closed.
    if(data.contains("api type=\"pairing\"") && data.contains("byebye")){
        qDebug() << "--> tv ended pairing";
        m_pairingStatus = false;
        m_reachable = false;
        emit statusChanged();
        return;
    }

    // check if this is a 3DMode changed event
    QXmlStreamReader xml(data);

    while(!xml.atEnd() && !xml.hasError()){
        xml.readNext();

        if(xml.name() == "name"){
            if(xml.readElementText() == "3DMode"){
                xml.readNext();
                if(xml.name() == "value"){
                    m_is3DMode = QVariant(xml.readElementText()).toBool();
                }
            }
        }
    }

    emit statusChanged();
}

