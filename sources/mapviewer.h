#ifndef MAPVIEWER_H
#define MAPVIEWER_H

#undef max
#include <QtGui/QMainWindow>
#include <QtNetwork/QTcpSocket>
#include <QtNetwork/QtNetwork>


#include <boost/array.hpp>
#include <boost/asio.hpp>
#include <boost/bind.hpp>

#include "ui_mapviewer.h"
#include "MapObjectContur.h"
#include "messagehandler.h"
#include "messages.h"
#include "LogMessage.h"
#include "qmlabeledit.h"
#include "lListWidgetUserData.h"

#include "port/portImpl.h"
#include "message/msgDataSet.h"

#include "Data/DataPlaneIterator.h"
#include "Data/SpherePathfindAnswerPacket.h"
#include "Data/SpherePathfindRequestPacket.h"
#include "Data/SphereVisMapAnswerPacket.h"
#include "Data/SphereVisMapRequestPacket.h"
#include "Data/SphereCoveringMapRequestPacket.h"
#include "Data/SphereCoveringMapAnswerPacket.h"

#include "Data/SphereCoveringPointRequestPacket.h"
#include "Data/SphereCoveringPointAnswerPacket.h"

#include "Data/SphereReachableBoundRequestPacket.h"
#include "Data/SphereReachableBoundAnswerPacket.h"

#include "layers.h"

#include "fantomtablewidget.h"
#include "deletetablewidget.h"

class TMyReceiver;

extern "C"
{
#include <proj_api.h>
}

QT_BEGIN_NAMESPACE
class QTcpSocket;
class QNetworkSession;
class QUdpSocket;
QT_END_NAMESPACE

class listCon
{
public:
    listCon()
    {
        list_.append(QString("192.168.101.1"));
        list_.append(QString("192.168.102.1"));
        list_.append(QString("192.168.103.1"));
        list_.append(QString("192.168.104.1"));
    }

    QString getValue(int i)
    {
        return list_.at(i);
    }

private:
    QStringList list_;
};

struct EllipseLineText
{
	QGraphicsEllipseItem* ellipse;
	QGraphicsSimpleTextItem* text;
	QGraphicsPixmapItem* pix;
};

class MapViewer : public QMainWindow
{
	Q_OBJECT

public:
	MapViewer(QString _path_app, LogMessage* log_m, QWidget *parent = 0, Qt::WFlags flags = 0);
    ~MapViewer();

	LogMessage* _log;
	Ui::MapViewerClass ui;
	static bool contol_key;
	static bool shift_key;
	QString path_app;

	bool auto_repeat;

	MapObjects map_objects;

	QScrollArea *scroll;

	static QMap<quint16, QPixmap> pixmap_type;
	static QMap<quint16, QString> name_type;

	//static bool useSK42;

	ListConnection tact_zn;

	static QString colorText(QString text, QString color)
	{
		QString res = tr("<font color = %1>%2<\\font><font color = black><\\font>").arg(color).arg(text);
		return res;
	}

	bool initialMap(QString file_name);
	bool ReadFileMap(QString file_name, MapObjects *_map_obj);

	QVector<EllipseLineText*> vectorMarks;
	QVector<EllipseLineText*> vectorPhoto;

	QVector<QAbstractGraphicsShapeItem*> vectorSphereShapes;

    //TMyReceiver* _s_txt_receiver;

    void             GetPacketFromPathfindService(TPort* port, msg::TDataSetPtr ds);
	void             GetPacketFromVisMapService(TPort* port, msg::TDataSetPtr ds);
	void             GetPacketFromCoveringMapService(TPort* port, msg::TDataSetPtr ds);
	void             GetPacketFromCoveringPointService(TPort* port, msg::TDataSetPtr ds);

    void             GetPacketFromReachableBoundService(TPort* port, msg::TDataSetPtr ds);
		

    TPort* PortSendRequestToPathfindService;
	TPort* PortReceiveAnswerFromPathfindService;

	TPort* PortSendRequestToVisMapService;
	TPort* PortReceiveAnswerFromVisMapService;

	TPort* PortSendRequestToCoveringMapService;
	TPort* PortReceiveAnswerFromCoveringMapService;

	TPort* PortSendRequestToCoveringPointService;
	TPort* PortReceiveAnswerFromCoveringPointService;

	TPort* PortSendRequestToReachableBoundService;
    TPort* PortReceiveAnswerFromReachableBoundService;//*/

    ulong _addr;
    ulong _mask;
    qulonglong _mac;
    listCon* listCon_;

    QUdpSocket * _psockBroad;
    QUdpSocket * _psockDiscover;
    QUdpSocket * _psockRssi;

    QString _strUnitName;

    QTcpServer * _pservClients;
    QStringList _lsRouters;

    QTcpServer * _pservSituation;

    QUdpSocket * _psockTelemetry;

    int _iClientsListTimer;
    int _iMessagesTimer;
    QTimer* _ptmBcast;

    QTcpServer * _pservMessages;
    QTcpServer * _pservRangef;

    void gotRangef(int unitid, const QString & name, const QByteArray & ba);
    boost::asio::ip::tcp::socket *_socket;
    void smes(char *m, const boost::system::error_code &error);
    void hendMes(const boost::system::error_code &error);
    char* message_send;

protected:
	bool eventFilter(QObject *obj, QEvent *ev);
	void closeEvent(QCloseEvent *e);
	void timerEvent(QTimerEvent *e);
    void keyReleaseEvent ( QKeyEvent * event );
    void keyPressEvent ( QKeyEvent * event );

    ListClients list_connection;

private:
	bool select_position;
	Layers* _layer_k;
	bool gotconn;
	QTcpServer *tcpServer;
	QGraphicsItem * plan;
	void sessionOpened();
	void hideShowMarks(int wi);
	void initTextCommands();
	
	void addZnak(quint64 mac, EmulData _data);

	bool setpos;
	int count_image;

	QGraphicsItemGroup *buildings;
	QGraphicsItemGroup *roads;
	QGraphicsItemGroup *matrix_data;

	FantomTableWidget* addWidget;
	DeleteTableWidget* delWidget;

	QList<quint64> list_emul;

    QVector<QGraphicsRectItem*> rect_vect;

	quint64 selected_mac;
	bool add_point;
	bool add_point_mark;
	ClientConnection *con_sel;
	Path_point* path_sel;
	QTreeWidgetItem* it_sel;
	Point_tab* point_sel;
	QGraphicsEllipseItem* mark_sel;
	QGraphicsItem* selected_obj;
	LineEdit_command* command_edit;
	QMap<QListWidgetItem*, lListWidgetUserData> map_data_listWidget_image;

    //--> new protocol ipv4
protected slots:
    void processPendingDiscoveryDatagrams();
    void processPendingRssiDatagrams();

    void newServCliensConnection();
    void readClients();
    void processPendingTelemetryDatagrams();
    void newServMessagesConnection();

    void newServSitConnection();
    void sendBroadcast();
public slots:

    qint64 SendBroadcast(const QByteArray & datagram, quint16 port);
    quint64 BroadcastDiscoveryInfo();
    quint64 BroadcastDiscoveryInfo(quint32 addr, const QString & name, uchar rssi = 0, quint32 time = QDateTime::currentDateTime().toTime_t());

    void UnicastToAll(const QByteArray & ba, quint16 port); /// don't forget about mss

    void sendMsg(ulong _s, char* mes, int size);
    void readMessage();

    void sendSituation(const QByteArray & ba);
    void readSituation();

    void newRangefConnection();
    void readRangef();
    //<-- new protocol ipv4


	private slots:
		void sendEmulateTable(EmulData _d);
		void clickSelectPosition();
		void addTableAction();
		void delTableAction();
        void select_path();
        void clearShapes();
		void test();
		void addPointTable();
		void endAddPoint();
		void addMark();
		void viewMark(QListWidgetItem* itemWidget);
		void messageToGui(MessageHandler* msgHandler, quint64 mac, QString name, QString text);
		void addMarkFromSignal(QPointF pos, QString name, int size, quint16 _type);
		void disconnected(MessageHandler* msgHandler);
		void newConnection();
		void readyRead(MessageHandler* msgHandler);
		void create_connection(MessageHandler* msgHandler, quint64 mac);
		void change_name_con(MessageHandler* msgHandler, quint64 mac, QString name);
		void viewTable(QListWidgetItem* itemWidget);
		void viewTable_point(QTreeWidgetItem* itemWidget);
		void setPos(quint64, QPointF, QGraphicsEllipseItem*, QByteArray);
		void send_message();
		void send_messageAll();
		void selectedObject(QPointF pos);
		void checkConfirm(quint16 _t);
		void change_state(MessageHandler* msgHandler, quint64 mac, uchar state);
		void refreshMarks();
		void deleteLayerConnect(MessageHandler* mes);
		void sendTo3D(TELEMETRY* pmsg);
		void send_command();
		void send_commandAll();
		void click_command(QListWidgetItem*);
		void readPicture(QPointF, QString, QString);
		void showImage(QListWidgetItem* itemWidget);
		void picture_input(quint64, QImage*, QString, QString, QPointF);

		void drawSuperMap(QString n="Data/CityGradient_mask_547x645.dpl");

		void setTrail(bool checked = false);
		void setPictures(bool checked = false);

		void addPathToTable();
signals:
		void selectObject(QPointF pos);
		void close_window();
		void selectPosition(QPointF _pos);
};

#endif // MAPVIEWER_H
