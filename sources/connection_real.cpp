#include <net/if.h>
#include <sys/ioctl.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <errno.h>

#include "mapviewer.h"
#include "math/astro.coordinates.hpp"


typedef QString(*InsCol)(QString, QString);
InsCol inscol = &InsertColor;
#define StrPars(p, t) StringColorParser::parseString(p, t)
#define Str_Num(p) QString::number(p)

void MapViewer::newServMessagesConnection()
{
    QTcpSocket *clientConnection = _pservMessages->nextPendingConnection();
    clientConnection->connect(clientConnection, SIGNAL(readyRead()), this, SLOT(readMessage()));
    clientConnection->connect(clientConnection, SIGNAL(disconnected()), SLOT(deleteLater()));
}

void MapViewer::newRangefConnection()
{
    QTcpSocket *clientConnection = _pservRangef->nextPendingConnection();
    clientConnection->connect(clientConnection, SIGNAL(readyRead()), this, SLOT(readRangef()));
    clientConnection->connect(clientConnection, SIGNAL(disconnected()), SLOT(deleteLater()));
}

qint64 MapViewer::SendBroadcast(const QByteArray & datagram, quint16 port)
{
    _log->tracer(QString("SendBroadcast"));
    qDebug() << "SendBroadcast";
    return _psockBroad->writeDatagram(datagram.data(), datagram.size(),
                                      QHostAddress((_addr & _mask) | ~_mask), port);
}

quint64 MapViewer::BroadcastDiscoveryInfo(quint32 addr, const QString & name, uchar rssi, quint32 time)
{
    DISCOVERYINFO di;
    di.ip = addr;
    QTextCodec *codec = QTextCodec::codecForName("KOI8-R");
    QByteArray ba(codec->fromUnicode(name));
    qstrncpy(di.name, ba.constData(), qMin(ba.size()+1, (int)sizeof(di.name)));
    di.rssi = rssi;
    di.time = time;



    QByteArray datagram((const char *)&di, sizeof(di));
    return SendBroadcast(datagram, UNITSDISCOVERYBROADCASTPORT);
}

quint64 MapViewer::BroadcastDiscoveryInfo()
{
    _log->tracer(QString("BroadcastDiscoveryInfo"));
    return BroadcastDiscoveryInfo(_addr, _strUnitName);
}

void MapViewer::processPendingRssiDatagrams()
{

}

void MapViewer::processPendingDiscoveryDatagrams()
{
    _log->tracer(QString("processPendingDiscoveryDatagrams"));
    while (_psockDiscover->hasPendingDatagrams())
    {
        QByteArray datagram;

        QHostAddress host;
        quint16 port;

        datagram.resize(_psockDiscover->pendingDatagramSize());
        _psockDiscover->readDatagram(datagram.data(), datagram.size(), &host, &port);

        const DISCOVERYINFO * pdi = (const DISCOVERYINFO *)datagram.constData();

        QTextCodec *codec = QTextCodec::codecForName("KOI8-R");
        QString name(codec->toUnicode(pdi->name));

        MessageHandler* mes = list_connection.getCon(0);
        if(!mes->tablets.isConnected(pdi->ip))
        {
            mes->tablets.addConnection(pdi->ip);
            //create_connection(mes, pdi->ip);
        }
        else
            qDebug() << "Conn yes";
        if(*mes->tablets.getNameConnection(pdi->ip) == "NO_NAME")
        {
            mes->tablets.setName(pdi->ip, codec->toUnicode(pdi->name));
            change_name_con(mes, pdi->ip, codec->toUnicode(pdi->name));
        }
        qDebug() << QString("Received datagram: from %1 name %2 address %3").arg(host.toString()).arg(name).arg(QHostAddress(pdi->ip).toString());
    }//*/
}

void MapViewer::newServCliensConnection()
{
    _log->tracer(QString("newServClientConnection"));
    QTcpSocket *clientConnection = _pservClients->nextPendingConnection();
    clientConnection->connect(clientConnection, SIGNAL(disconnected()), SLOT(deleteLater()));
    ///QTimer::singleShot(5000, clientConnection, SLOT(deleteLater())); /// 5 seconds

    DISCOVERYINFO di;
    di.ip = _addr;
    QTextCodec *codec = QTextCodec::codecForName("KOI8-R");
    QByteArray ba(codec->fromUnicode(_strUnitName));
    qstrncpy(di.name, ba.constData(), qMin(ba.size()+1, (int)sizeof(di.name)));
    di.rssi = 0;
    di.time = 0;



    QByteArray datagram((const char *)&di, sizeof(di));
    datagram.prepend(1);

    clientConnection->write(datagram);

    clientConnection->disconnectFromHost();
}

void MapViewer::readClients()
{
    _log->tracer(QString("Read clients"));
    if (QObject::sender())
    {
        QTcpSocket * psock = qobject_cast<QTcpSocket *>(QObject::sender());
        if (psock)
        {
            if (psock->bytesAvailable() < 1) return;

            uchar c;
            psock->getChar((char *)&c);

            if (psock->bytesAvailable() == (c * sizeof(DISCOVERYINFO)))
            {
                QByteArray ba(psock->readAll());
                if ((ba.size()) == (c * sizeof(DISCOVERYINFO)))
                {
                    DISCOVERYINFO * pdi = (DISCOVERYINFO *)ba.data();

                    for(int i = 0; i < c; i++)
                    {
                        QTextCodec *codec = QTextCodec::codecForName("KOI8-R");
                        QString name(codec->toUnicode(pdi[i].name));

                        MessageHandler* mes = list_connection.getCon(0);
                        if(!mes->tablets.isConnected(pdi[i].ip))
                        {
                            mes->tablets.addConnection(pdi[i].ip);
                        }
                        else
                            qDebug() << "Conn yes";
                        if(*mes->tablets.getNameConnection(pdi[i].ip) == "NO_NAME")
                        {
                            mes->tablets.setName(pdi[i].ip, codec->toUnicode(pdi[i].name));
                            change_name_con(mes, pdi[i].ip, codec->toUnicode(pdi[i].name));
                        }

                        //_pUnitsModel->AddChangeUnit(pdi[i].ip, name, pdi[i].rssi, pdi[i].time);



                        qDebug() << QString("Received from unit info from router: from %1 name %2 address %3 rssi %4 time %5")
                        .arg(psock->peerAddress().toString())
                        .arg(name).arg(QHostAddress(pdi[i].ip).toString())
                        .arg(pdi[i].rssi)
                        .arg(pdi[i].time);
                    }
                }
            }
            else
            {
                psock->ungetChar(c);
            }

        }
    }
}

void MapViewer::UnicastToAll(const QByteArray & ba, quint16 port)
{
    _log->tracer(QString("UnicastToAll"));
    MessageHandler* mes = list_connection.getCon(0);
    QMap<quint64, ClientConnection*>::iterator i = mes->tablets.getMapConnection().begin();
    while (i != mes->tablets.getMapConnection().constEnd())
    {
        QUdpSocket * psock = new QUdpSocket(this);
        psock->bind(QHostAddress(_addr), 0);
        psock->writeDatagram(ba.data(), ba.size(), QHostAddress(i.value()->getIdConnection()), port);
        psock->deleteLater();

        ++i;
    }
}

void MapViewer::processPendingTelemetryDatagrams()
{
    while (_psockTelemetry->hasPendingDatagrams())
    {
        QByteArray datagram;

        QHostAddress host;
        quint16 port;

        datagram.resize(_psockTelemetry->pendingDatagramSize());
        _psockTelemetry->readDatagram(datagram.data(), datagram.size(), &host, &port);
        DEV_TELEMETRY *tel_ = (DEV_TELEMETRY*)datagram.data();
        if(tel_->state)
        {
            uchar mesid = 0;
            algebra::V3D pos, geo;

            PINT pz;

            memcpy((void *)&pz.x, (void *)&tel_->pos.x, sizeof(pos.dec.x));
            memcpy((void *)&pz.y, (void *)&tel_->pos.y, sizeof(pos.dec.y));
            memcpy((void *)&pz.z, (void *)&tel_->pos.z, sizeof(pos.dec.z));

            pos.dec.x = pz.x;
            pos.dec.y = pz.y;
            pos.dec.z = pz.z;

            algebra::Mul(pos, 0.0001); /// в километры, и из зайц
            astro::FromECEFToGeodetic(astro::WGS84(), pos, geo);
            geo.geo.h *= 1000.0; /// метры

            /// заменяем координаты для пересылки АРМу

            tel_->pos.x = geo.geo.b / DEG_TO_RAD;
            tel_->pos.y = geo.geo.l / DEG_TO_RAD;
            tel_->pos.z = geo.geo.h;
        }
        TELEMETRY *tel = new TELEMETRY;
        tel->hdr.id = TELEMETRY_ID;
        tel->hdr.mac = 0;
        tel->hdr.tip = 0;

        QTextCodec *codec = QTextCodec::codecForName("KOI8-R");
        QString name_ = codec->toUnicode(tel_->name);

        qDebug() << "name: " << codec->toUnicode(tel_->name);
        qDebug() << "state: " << tel->state;

        MessageHandler* mes = list_connection.getCon(0);
        tel->hdr.mac = mes->tablets.getIdConnection(name_);
        tel->dirspd = tel_->dirspd;
        tel->id = tel_->id;
        tel->mark = tel->mark;
        memcpy(tel->name, tel_->name, 16);
        tel->pos = tel_->pos;
        tel->state = tel_->state;
        if (tel->state == (uchar)1)
        {

            qDebug() << "coords: " << tel->pos.x << " " << tel->pos.y << " " << tel->pos.z;
            QPointF position(tel->pos.x, tel->pos.y);
            if(!howRecord::useSK42)
                position = recoder(position);
            else
                position = recoderWGSToSK(position);
            QByteArray b = tel->name;
            QGraphicsEllipseItem* q;
            quint64 mac = mes->tablets.getIdConnection(name_);
            if(mac)
                q = mes->tablets.getEllipse(mac);
            else
                q = 0;
            setPos(mac, position, q, b);
        }


        qDebug() << "dir: " << tel->dirspd.x << " speed: " << tel->dirspd.y;
        QString str = "";
        str	+=	inscol("id: [",         COLOR_TEXT_BLACK)  	+	inscol("TELEMETRY_ID",              COLOR_TEXT_PINK);
        str	+=	inscol("] name: [",     COLOR_TEXT_BLACK)	+	inscol(codec->toUnicode(tel->name), COLOR_TEXT_BLUE);
        str	+=	inscol("] state: [",    COLOR_TEXT_BLACK)	+	inscol(Str_Num(tel->state),         COLOR_TEXT_RED);
        str	+=	inscol("] x: [",        COLOR_TEXT_BLACK)  	+	inscol(Str_Num(tel->pos.x),         COLOR_TEXT_GREEN);
        str	+=	inscol("] y: [",        COLOR_TEXT_BLACK)  	+	inscol(Str_Num(tel->pos.y),         COLOR_TEXT_GREEN);
        str	+=	inscol("] z: [",        COLOR_TEXT_BLACK)  	+	inscol(Str_Num(tel->pos.z),         COLOR_TEXT_GREEN);
        str	+=	inscol("] dir: [",      COLOR_TEXT_BLACK)	+	inscol(Str_Num(tel->dirspd.x),      COLOR_TEXT_ORANGE);
        str	+=	inscol("] speed: [",    COLOR_TEXT_BLACK)	+	inscol(Str_Num(tel->dirspd.y),      COLOR_TEXT_ORANGE);
        str	+=	inscol("]",             COLOR_TEXT_BLACK);
        mes->_log->tracer(str);
        sendTo3D(tel);
    }
}

void MapViewer::sendMsg(ulong _s, char* mes, int size)
{
    _log->tracer("send message");
    try
    {
        boost::asio::io_service srv;
        boost::asio::ip::tcp::socket sock(srv);
        boost::system::error_code ec;
        sock.open(boost::asio::ip::tcp::v4(), ec);
        std::cout << ec.message() << std::endl;
        boost::asio::ip::tcp::endpoint ep;
        ep.address(boost::asio::ip::address_v4(_addr));
        sock.bind(ep, ec);
        std::cout << ec.message() << std::endl;
        std::cout << sock.local_endpoint() << std::endl;
        boost::asio::ip::tcp::endpoint dest(boost::asio::ip::address_v4(_s), UNITSMESSAGESPORT);
        sock.connect(dest, ec);
        if(!ec)
            sock.send(boost::asio::buffer(mes, size), 0, ec);
        std::cout << ec.message() << std::endl;
    }
    catch(std::exception& e)
    {
        std::cout << e.what() << std::endl;
    }
}

void MapViewer::sendBroadcast()
{
    BroadcastDiscoveryInfo();
    _ptmBcast->start(10000 + (qrand() % 50000));
}

void MapViewer::readMessage()
{
    qDebug() << "readMessage";
    _log->tracer(QString("read message"));
    if (QObject::sender())
    {
        QTcpSocket * psock = qobject_cast<QTcpSocket *>(QObject::sender());
        if (psock)
        {
            if (psock->bytesAvailable() < 1) return;
            {
                QByteArray ba(psock->readAll());
                {

//                    QString name(psock->peerAddress().toString());
                    quint64 mac = psock->peerAddress().toIPv4Address();
                    MessageHandler* m = list_connection.getCon(0);
                    QString *name = m->tablets.getNameConnection(mac);
                    qDebug() << "Message from :" << name;
                    _log->tracer(QString("Message from:").append(name));
                    QTextCodec *codec = QTextCodec::codecForName("KOI8-R");
                    QString text = codec->toUnicode(ba);
                    messageToGui(list_connection.getCon(0), 0, *name, text);
                }
            }
        }
    }
}

void MapViewer::sendSituation(const QByteArray & ba)
{

}

void MapViewer::newServSitConnection()
{
    _log->tracer(QString("newServSitConnection"));
    QTcpSocket *clientConnection = _pservSituation->nextPendingConnection();
    clientConnection->connect(clientConnection, SIGNAL(disconnected()), this, SLOT(readSituation()));
}

void MapViewer::readSituation()
{
    _log->tracer(QString("Read situation"));
    if (QObject::sender())
    {
        QTcpSocket * psock = qobject_cast<QTcpSocket *>(QObject::sender());

        if (psock)
        {
            /*if (_pUnitsModel->ips().contains(psock->peerAddress().toIPv4Address()))
            {
                emit gotSituation(_pUnitsModel->ips().indexOf(psock->peerAddress().toIPv4Address()), psock->readAll());
            }*/

            psock->deleteLater();
        }
    }
}

void MapViewer::readRangef()
{
    qDebug() << "readRangef";
    _log->tracer(QString("Read rangef"));
    if (QObject::sender())
    {
        QTcpSocket * psock = qobject_cast<QTcpSocket *>(QObject::sender());
        if (psock)
        {
            if (psock->bytesAvailable() < 1) return;

            if (psock->bytesAvailable() == sizeof(DEV_COMMAND1))
            {
                QByteArray ba(psock->readAll());
                if ((ba.size()) == sizeof(DEV_COMMAND1))
                {
                    QString s = "";
                    gotRangef(0, s, ba);
                }
            }//*/
        }
    }
}

void MapViewer::gotRangef(int unitid, const QString & name, const QByteArray & ba)
{
    if (ba.size() == sizeof(DEV_COMMAND1))
        {
            qDebug() << "GOT RANGE";

            DEV_COMMAND1 * pm = (DEV_COMMAND1 *)ba.data();

            if ((pm->params[0] == 4))
            {
                QTextCodec *codec = QTextCodec::codecForName("KOI8-R");
                QString name(codec->toUnicode(pm->text, qstrlen(pm->text)));
                pm->params[1] = (pm->params[1] % 5);
                QPointF pos_m(pm->cord.x, pm->cord.y);

                addMarkFromSignal(pos_m, name, sizeof(pm->text), pm->params[2]);
            }
        }
}
