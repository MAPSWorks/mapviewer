#ifndef MESSAGEHANDLER_H
#define MESSAGEHANDLER_H

#undef max
#undef min

#include <QObject>
#include <QtGui/QtGui>

#include "ClientConnection.h"
#include "LogMessage.h"
#include "MessageBuffer.h"
#include "messages.h"
#include "StringColorParser.h"
#include "fantomtablewidget.h"

QT_BEGIN_NAMESPACE
class QObject;
class QTcpSocket;
QT_END_NAMESPACE

#define XBEETUN_IFNAME      "eth0.2"
#define XBEETUN_ADMIN_PORT  22849 // ushort("AP") Administration Port
#define XBEETUN_RSSI_PORT   21331 // ushort("SS") Signal Strength



#define UNITSDISCOVERYBROADCASTPORT  27434 // Broadcast info connections

#define UNITSMESSAGESPORT           27435  // messages
#define UNITSTELEMETRYPORT          27436  // telemetry
#define UNITSSITUATIONPORT          27437  // situation



#define UNITSDISCOVERYROUTERSTPORT  27444
#define RANGEFPORT                  27440



#pragma pack(1)
typedef struct
{
    quint32 ip;
    char name[32];
    quint32 time;
    uchar rssi;
}
    DISCOVERYINFO;
#pragma pack()


#pragma pack(push, 1)
typedef struct
{
    ulong addr;
    uchar rssi;
}
    RSSIINFO;
#pragma pack(pop)

static int count_picture;


class MessageHandler : public QObject
{
    Q_OBJECT

public:
    MessageHandler(QString _path_app, LogMessage* log_m, QObject * parent, QTcpSocket * s, bool auto_r);
    ~MessageHandler();

	QString path_app;
	quint16 id;
	bool create;
	QGraphicsEllipseItem * point_item;

	ListConnection tablets;
	LogMessage* _log;
	MessageBuffer* _bufferMes;
    void reset()
    {
        size = 0;
    }
	bool comp;
	bool auto_repeat;

    void sendMessage(const char * s, int sz, int rep = 1);
    bool recvMessage(QByteArray & m);

	void hidePoint(quint64 mac)
	{
		if(tablets.getEllipse(mac))
		{
			tablets.getEllipse(mac)->setVisible(false);
			tablets.getClientConnection(mac)->getRectText()->setVisible(false);
		}
		//point_item->setVisible(false);
	}

    QTcpSocket * getSock() {return sock;}

	void signal_pos(quint64 mac, QPointF p, QByteArray name)
	{
		//tablets.setPosition(mac, p);
		emit setPos(mac, p, tablets.getEllipse(mac), name);
	}
	void change_state(quint64 mac, uchar state)
	{
		emit change_state_signal(this, mac, state);
	}
	void change_name(quint64 mac, QString name)
	{
		emit signalChangeName(this, mac, name);
	}

	void repeatMessage(quint16 id);
	void sendMessage_addPoint(quint64 mac, QPointF point, QString name, int id_point, int id_path);
	void sendMessage_addMark(quint64 mac, QPointF point, QString name, Point_tab* point_sel, int id_mark);
	void sendMessage_chat(quint64 mac, char* mess);
	void sendMessage_emulate(quint64 mac, EmulData data);
	void sendMessage_deleteEmul(quint64 mac);
	void messageToGui(quint64 mac, QString text);
	void onCreate(quint64 mac);
	void dalnomerMark(QPointF pos, QString name, int size, quint16 _type);
	void deleteMessageFromBuffer(quint16 _tip);
	void sendPointToComp(quint16 mac, QPointF point, QString name, quint16 id, int params);
	void picture_input(quint64 mac, QImage* picture, QString name, QString filename, QPointF pos = QPointF(0, 0))
	{
		emit picture_signal(mac, picture, name, filename, pos);
	}
	quint16 getIdMessage()
	{
		return tip;
	}

	void sendTo3DComp(TELEMETRY * pmsg)
	{
		emit sendTo3DComp_signal(pmsg);
	}
	void readPicture(QPointF pos, QString filename, QString name, quint64 mac)
	{
		QString name_temp = *tablets.getNameConnection(mac);
		name = name_temp + name;
		emit readPicture_signal(pos, filename, name);
	}

public slots:
	void close_slot();
private slots:
	void disconnected();
	void readyRead();
	void sendPack(QByteArray pack);

signals:
    void disconnected(MessageHandler* msgHandler);
    void readyRead(MessageHandler* msgHandler);
	void onCreate(MessageHandler* msgHandler, quint64 mac);
	void messageToGuiSignal(MessageHandler* msgHandler, quint64 mac, QString name, QString text);
	void dalnomerMarkSignal(QPointF pos, QString name, int size, quint16 _type);

	void setPos(quint64 mac, QPointF point, QGraphicsEllipseItem* ellipse, QByteArray name);
	void change_state_signal(MessageHandler* msgHandler, quint64 mac, uchar state);
	void signalChangeName(MessageHandler* msgHandler, quint64 mac, QString name);
	void confirm_message(quint16 _t);
	void sendTo3DComp_signal(TELEMETRY* pmsg);
	void readPicture_signal(QPointF, QString, QString);
	void picture_signal(quint64, QImage*, QString, QString, QPointF);

protected:
    quint32 size;
    QTcpSocket * sock;
public:
	quint16 tip;
};

void DebugMessage(const QByteArray & ba, MessageHandler *mes);

class ListClients
{
public:
    ListClients(){}
    ~ListClients(){}

	QList<MessageHandler*> list_mes_handlers;

	bool ifConnected(QTcpSocket * soc);
	void add(MessageHandler* mes)
	{
		list_mes_handlers.append(mes);
	}

	void remove(MessageHandler* mes)
	{
		list_mes_handlers.removeOne(mes);
	}

	void remove(int i)
	{
		list_mes_handlers.removeAt(i);
	}
	MessageHandler* getCon(int i)
	{
		typedef QList<MessageHandler*>::Iterator li;
		for(li iter = list_mes_handlers.begin(); iter != list_mes_handlers.end(); iter++)
		{
			MessageHandler* e = *iter;
			if(i == 0)
			{
				if(!e->comp)
					return e;
			}
			else if(i == 1)
			{
				if(e->comp)
					return e;
			}
		}
		return 0;
	}
};

#endif // MESSAGEHANDLER_H
