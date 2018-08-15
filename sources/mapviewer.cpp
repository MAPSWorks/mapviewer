#include "mapviewer.h"
#include "StringColorParser.h"
#include <QtGui/qgraphicsitem.h>

#include <QtDebug>
#include <QtNetwork/QTcpSocket>
#include <stdlib.h>

#include "Data/DataPlaneIterator.h"

#include "sphere_messages.h"

#include <net/if.h>
#include <sys/ioctl.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <errno.h>

bool MapViewer::contol_key = false;
bool MapViewer::shift_key = false;
QMap<quint16, QString> MapViewer::name_type = QMap<quint16, QString>();
QMap<quint16, QPixmap> MapViewer::pixmap_type = QMap<quint16, QPixmap>();
bool howRecord::useSK42 = true;

MapViewer::MapViewer(QString _path_app, LogMessage* log_m, QWidget *parent, Qt::WFlags flags)
	: QMainWindow(parent, flags)
{
	select_position = false;
	path_app = _path_app;
	auto_repeat = false;
	_log = log_m;
	ui.setupUi(this);
	ui.widget->setStyleSheet( QString::fromUtf8( "QWidget {\n"
		"background-color: rgba( 221, 223, 227, 170 );\n"
		"}\n" ) );
	ui.widget_3->setStyleSheet( QString::fromUtf8( "QWidget {\n"
		"background-color: rgba( 221, 223, 227, 170 );\n"
		"}\n" ) );

	ui.centralWidget->installEventFilter(this);
	ui.treeWidget->installEventFilter(this);

	ui.widget->move(5, 5);
	ui.listWidget_images->hide();
	
	command_edit = new LineEdit_command(ui.widget);
	command_edit->setObjectName(QString::fromUtf8("command_edit"));
	command_edit->setGeometry(QRect(10, 730, 251, 20));
	command_edit->setMaxLength(128);
	command_edit->setWidgetForSearch(ui.listWidget_command);


	scroll = new QScrollArea(ui.centralWidget);
	scroll->setWidget(ui.widget);
	scroll->setStyleSheet( QString::fromUtf8( "QWidget {\n"
		"background-color: rgba( 221, 223, 227, 170 );\n"
		"}\n" ) );
	scroll->show();

	QIcon icon_addCon = QIcon(":/MapViewer/Resources/addCon.png");
	QAction* addTable = ui.mainToolBar->addAction(icon_addCon, QString::fromLocal8Bit("Добавить планшет"));
	connect(addTable, SIGNAL(triggered()), this, SLOT(addTableAction()));
	QIcon icon_delCon = QIcon(":/MapViewer/Resources/delCon.png");
	QAction* delTable = ui.mainToolBar->addAction(icon_delCon, QString::fromLocal8Bit("Удалить планшет"));
	connect(delTable, SIGNAL(triggered()), this, SLOT(delTableAction()));

	buildings = new QGraphicsItemGroup(0, ui.graphicsView->scene());
	roads = new QGraphicsItemGroup(0, ui.graphicsView->scene());
	matrix_data = new QGraphicsItemGroup(0, ui.graphicsView->scene());
    {
	QPixmap _t = QPixmap(":/MapViewer/Resources/AK-74.png", "PNG");
	if(!_t.isNull())
		pixmap_type.insert(1, _t);
	name_type.insert(1, QString::fromLocal8Bit("Автоматчик"));

	_t = QPixmap(":/MapViewer/Resources/sniper1.png", "PNG");
	if(!_t.isNull())
		pixmap_type.insert(2, _t);
	name_type.insert(2, QString::fromLocal8Bit("Снайпер"));

	_t = QPixmap(":/MapViewer/Resources/PKM.png", "PNG");
	if(!_t.isNull())
		pixmap_type.insert(3, _t);
	name_type.insert(3, QString::fromLocal8Bit("Пулеметчик ПКМ"));

	_t = QPixmap(":/MapViewer/Resources/gp-30.png", "PNG");
	if(!_t.isNull())
		pixmap_type.insert(4, _t);
	name_type.insert(4, QString::fromLocal8Bit("Автоматчик с подствольным гранатометом"));

	_t = QPixmap(":/MapViewer/Resources/AGS-17.png", "PNG");
	if(!_t.isNull())
		pixmap_type.insert(5, _t);
	name_type.insert(5, QString::fromLocal8Bit("30-мм автоматический гранатомет АГС-17"));

	_t = QPixmap(":/MapViewer/Resources/NSV.png", "PNG");
	if(!_t.isNull())
		pixmap_type.insert(6, _t);
	name_type.insert(6, QString::fromLocal8Bit("НСВ 'Утес'"));

	_t = QPixmap(":/MapViewer/Resources/RPG7V.png", "PNG");
	if(!_t.isNull())
		pixmap_type.insert(7, _t);
	name_type.insert(7, QString::fromLocal8Bit("Гранатометчик с РПГ-7"));

	_t = QPixmap(":/MapViewer/Resources/PTRK9R148.png", "PNG");
	if(!_t.isNull())
		pixmap_type.insert(8, _t);
	name_type.insert(8, QString::fromLocal8Bit("ПТРК 9П148 АТ-4 'Конкурс'"));

	_t = QPixmap(":/MapViewer/Resources/Rapira.png", "PNG");
	if(!_t.isNull())
		pixmap_type.insert(9, _t);
	name_type.insert(9, QString::fromLocal8Bit("100-мм МТ-12 'Рапира'"));

	_t = QPixmap(":/MapViewer/Resources/MSTA-S.png", "PNG");
	if(!_t.isNull())
		pixmap_type.insert(10, _t);
	name_type.insert(10, QString::fromLocal8Bit("152-мм самоходная гаубица МСТА-С"));

	_t = QPixmap(":/MapViewer/Resources/2c3.png", "PNG");
	if(!_t.isNull())
		pixmap_type.insert(11, _t);
	name_type.insert(11, QString::fromLocal8Bit("152-мм самоходная гаубица 2С3 'Акация'"));

	_t = QPixmap(":/MapViewer/Resources/2c1.png", "PNG");
	if(!_t.isNull())
		pixmap_type.insert(12, _t);
	name_type.insert(12, QString::fromLocal8Bit("152-мм самоходная гаубица 2С1 'Гвоздика'"));

	_t = QPixmap(":/MapViewer/Resources/Grad.png", "PNG");
	if(!_t.isNull())
		pixmap_type.insert(13, _t);
	name_type.insert(13, QString::fromLocal8Bit("122-мм РСЗО БМ-21 'Град'"));

	_t = QPixmap(":/MapViewer/Resources/Podnos.png", "PNG");
	if(!_t.isNull())
		pixmap_type.insert(14, _t);
	name_type.insert(14, QString::fromLocal8Bit("120-мм миномет ПМ-120 'Поднос'"));

	_t = QPixmap(":/MapViewer/Resources/T80.png", "PNG");
	if(!_t.isNull())
		pixmap_type.insert(15, _t);
	name_type.insert(15, QString::fromLocal8Bit("Автоматчик"));

	_t = QPixmap(":/MapViewer/Resources/T80.png", "PNG");
	if(!_t.isNull())
		pixmap_type.insert(16, _t);
	name_type.insert(16, QString::fromLocal8Bit("Танк Т-80"));

	_t = QPixmap(":/MapViewer/Resources/BMP3.png", "PNG");
	if(!_t.isNull())
		pixmap_type.insert(17, _t);
	name_type.insert(17, QString::fromLocal8Bit("Танк Т-90"));

	_t = QPixmap(":/MapViewer/Resources/MTLB.png", "PNG");
	if(!_t.isNull())
		pixmap_type.insert(18, _t);
	name_type.insert(18, QString::fromLocal8Bit("МТЛБ"));
    }

	sessionOpened();

	connect(tcpServer, SIGNAL(newConnection()), this, SLOT(newConnection()));
    connect(ui.listWidget, SIGNAL(itemClicked(QListWidgetItem*)), this, SLOT(viewTable(QListWidgetItem*)));
	connect(ui.listWidget_mark, SIGNAL(itemClicked(QListWidgetItem*)), this, SLOT(viewMark(QListWidgetItem*)));
	connect(ui.addPointTable, SIGNAL(clicked()), this, SLOT(addPointTable()));
	connect(ui.endAddPoint, SIGNAL(clicked()), this, SLOT(endAddPoint()));
	connect(ui.addMark, SIGNAL(clicked()), this, SLOT(addMark()));
	connect(ui.send_text, SIGNAL(clicked()), this, SLOT(send_message()));
	connect(ui.send_allUsers, SIGNAL(clicked()), this, SLOT(send_messageAll()));
	connect(this, SIGNAL(selectObject(QPointF)), this, SLOT(selectedObject(QPointF)));
	connect(ui.refresh, SIGNAL(clicked()), this, SLOT(refreshMarks()));
	connect(ui.button_sendCommand, SIGNAL(clicked()), this, SLOT(send_command()));
	connect(ui.button_sendCommandAll, SIGNAL(clicked()), this, SLOT(send_commandAll()));
	connect(ui.listWidget_command, SIGNAL(itemClicked(QListWidgetItem*)), this, SLOT(click_command(QListWidgetItem*)));
    connect(ui.listWidget_images, SIGNAL(itemClicked(QListWidgetItem*)), this, SLOT(showImage(QListWidgetItem*)));
	connect(ui.actionTrailItems, SIGNAL(triggered(bool)), this, SLOT(setTrail(bool)));
	connect(ui.actionPictures, SIGNAL(triggered(bool)), this, SLOT(setPictures(bool)));
    connect(ui.addPath_button, SIGNAL(clicked()), this, SLOT(addPathToTable()));
    connect(ui.treeWidget, SIGNAL(itemSelectionChanged ()), this, SLOT(select_path()));
	ui.pushButton->setEnabled(false);
    ui.pushButton->hide();

	setpos = false;
	add_point = false;
	add_point_mark = false;
	selected_obj = 0;
	selected_mac = -1;
	initTextCommands();
	count_image = 0;
	path_sel = 0;
	it_sel = 0;
    tact_zn = ListConnection();
	
    try
	{
		sphere::Initialize();
	

        PortSendRequestToPathfindService     = sphere::CreatePort("Pathfind_ClientPortToService");
        sphere::EV_MessageReceiver f1 = boost::bind(&MapViewer::GetPacketFromPathfindService, boost::ref(*this), _1, _2);
        PortReceiveAnswerFromPathfindService = sphere::CreatePort("Pathfind_ClientPortFromService", f1);

        PortSendRequestToVisMapService       = sphere::CreatePort("VisMap_ClientPortToService");
        sphere::EV_MessageReceiver f2 = boost::bind(&MapViewer::GetPacketFromVisMapService, boost::ref(*this), _1, _2);
        PortReceiveAnswerFromVisMapService   = sphere::CreatePort("VisMap_ClientPortFromService", f2);

        PortSendRequestToCoveringMapService     = sphere::CreatePort("CoveringMap_ClientPortToService");
        sphere::EV_MessageReceiver f3 = boost::bind(&MapViewer::GetPacketFromCoveringMapService, boost::ref(*this), _1, _2);
        PortReceiveAnswerFromCoveringMapService = sphere::CreatePort("CoveringMap_ClientPortFromService", f3);

        PortSendRequestToCoveringPointService     = sphere::CreatePort("CoveringPoint_ClientPortToService");
        sphere::EV_MessageReceiver f4 = boost::bind(&MapViewer::GetPacketFromCoveringMapService, boost::ref(*this), _1, _2);
        PortReceiveAnswerFromCoveringPointService = sphere::CreatePort("CoveringPoint_ClientPortFromService", f4);

        PortSendRequestToReachableBoundService     = sphere::CreatePort("ReachableBound_ClientPortToService");
        sphere::EV_MessageReceiver f5 = boost::bind(&MapViewer::GetPacketFromReachableBoundService, boost::ref(*this), _1, _2);
        PortReceiveAnswerFromReachableBoundService = sphere::CreatePort("ReachableBound_ClientPortFromService", f5);
	}
	catch (std::exception& e)
	{
        _log->tracer(QString::fromLocal8Bit(e.what()));
        qDebug() << QString::fromLocal8Bit(e.what());
    }//*/

    //startTimer(10000);
}
void MapViewer::test()
{
	QImage *im = new QImage;
    im->load(":/MapViewer/Resources/AK-74.png", "PNG");
    QPixmap pict = QPixmap::fromImage(*im);
	if(pict.isNull())
		qDebug() << "error";
	qDebug() << pict.size();
	QGraphicsPixmapItem *item = new QGraphicsPixmapItem(pict, 0, ui.graphicsView->scene());
	item->setScale(0.10);
	item->setPos(6177596.03, 37414370.63);
	ui.graphicsView->centerOn(item);
	qDebug() << item->pixmap().size();
	qDebug() << item->boundingRect();
    qDebug() << item->pos();
}

void MapViewer::addTableAction()
{
	addWidget = new FantomTableWidget(this);
	connect(addWidget, SIGNAL(clickSelectPosition()), this, SLOT(clickSelectPosition()));
	connect(this, SIGNAL(selectPosition(QPointF)), addWidget, SLOT(setPosition(QPointF)));
	connect(addWidget, SIGNAL(addTS(EmulData)), this, SLOT(sendEmulateTable(EmulData)));
}

void MapViewer::sendEmulateTable(EmulData _d)
{
	bool ex = true;
	quint64 mac;
	if(_d.type_obj == 0)
		_d.type_obj = 1;
	MessageHandler* mes = list_connection.getCon(0);
	if(mes)
	{
		do
		{
			mac = qrand();
			QMap<quint64, ClientConnection*> map_con = mes->tablets.getMapConnection();
			foreach(ClientConnection* cl_con, map_con)
			{
				if(cl_con->getIdConnection() == mac)
					ex = false;
			}
		} while(!ex);

		mes->sendMessage_emulate(mac, _d);
		list_emul.append(mac);
	}
	else
	{
		do
		{
            mac = qrand();
			foreach(quint64 _mac, list_emul)
			{
				if(_mac == mac)
					ex = false;
			}
		} while(!ex);

		list_emul.append(mac);
	}
	addZnak(mac, _d);
}

void MapViewer::addZnak(quint64 mac, EmulData _data)
{
	if(!tact_zn.isConnected(mac))
	{
		tact_zn.addConnection(mac, _data.name, _data.position);
		tact_zn.setEllipse(mac, ui.graphicsView->scene());
		tact_zn.getEllipse(mac)->hide();
		tact_zn.getEllipse(mac)->setEnabled(false);

		QPixmap pict = pixmap_type.value(_data.type_obj);
		if(!pict.isNull())
		{
			ClientConnection* cl_con = tact_zn.getClientConnection(mac);
			cl_con->icon_pix = new QGraphicsPixmapItem(pict, 0, ui.graphicsView->scene());
			cl_con->icon_pix->setScale(0.01);
			cl_con->icon_pix->setEnabled(ClientConnection::doPictures);
			cl_con->icon_pix->setVisible(ClientConnection::doPictures);
			if(!howRecord::useSK42)
				cl_con->icon_pix->setPos(recoder(_data.position));
			else
                cl_con->icon_pix->setPos(recoderWGSToSK(_data.position));
		}

	}
}

void MapViewer::clickSelectPosition()
{
	select_position = true;
}

void MapViewer::delTableAction()
{
	QList<ClientConnection*> _temp;
	foreach(quint64 _t, list_emul)
	{
		ClientConnection* _add_item = list_connection.getCon(0)->tablets.getClientConnection(_t);
		if(_add_item != 0)
			_temp.append(_add_item);
	}
	delWidget = new DeleteTableWidget(_temp);
	connect(delWidget, SIGNAL(deleteItems(QList<quint64>)), this, SLOT());
}

void MapViewer::addPathToTable()
{
	add_point_mark = false;
	ui.addMark->setEnabled(false);
	if(con_sel)
	{
		QStringList str;
		str.append("Path");
		int _id = ++con_sel->id_path;
		str.append(QString::number(_id));
        QTreeWidgetItem *item = new QTreeWidgetItem(str);
		ui.treeWidget->addTopLevelItem(item);
		Path_point *p = new Path_point();
		p->id = _id;
        con_sel->paths.insert(_id, p);
    }
}

void MapViewer::select_path()
{
	if(ui.treeWidget->selectedItems().size() > 0)
	if(!ui.treeWidget->selectedItems()[0]->parent())
	{
		add_point_mark = false;
		ui.addMark->setEnabled(false);
		it_sel = ui.treeWidget->selectedItems()[0];
		path_sel = con_sel->paths.value(ui.treeWidget->indexOfTopLevelItem(it_sel)+1);
		if(!path_sel)
			qDebug() << "Error!";
		else
			ui.addPointTable->setEnabled(true);
	}
	else
	{
		QTreeWidgetItem* item = ui.treeWidget->selectedItems()[0];
		it_sel = item->parent();
		path_sel = con_sel->paths.value(ui.treeWidget->indexOfTopLevelItem(it_sel)+1);
		if(!path_sel)
			qDebug() << "Error!";
		else
            viewTable_point(item);
	}
}

void MapViewer::setTrail(bool checked)
{
	ClientConnection::doTrailItem/*trailItem*/ = checked;

	if (!checked)
		ClientConnection::claerTrailVec();
}

void MapViewer::setPictures(bool checked)
{
	ClientConnection::doPictures = checked;
	MessageHandler* mes = list_connection.getCon(0);
	if(mes)
	{
		QMap<quint64, ClientConnection*> map_con = mes->tablets.getMapConnection();
		foreach(ClientConnection* cl_con, map_con)
		{
			cl_con->statePictures(checked);
		}
	}
}

void MapViewer::initTextCommands()
{
    QFile m_file(path_app + QString("commands2.txt"));
	if (!m_file.open(QIODevice::ReadOnly))
    {
		return;
	}

    _log->tracer(QString("file open"));

	while(!m_file.atEnd())
	{
		char* temp = (char*)malloc(128);//[128];
        int size_text;
		size_text = m_file.readLine(temp, 128);
        QByteArray ba2;
        ba2.resize(size_text);
        qstrncpy(ba2.data(), temp, size_text);
        QString str = QString::fromLocal8Bit(ba2.data());
		ui.listWidget_command->addItem(str.simplified());
		free(temp);
	}
	return;
}

void MapViewer::send_command()
{
	QString text(command_edit->text());
	QTextCodec *codec = QTextCodec::codecForName("KOI8-R");
    int size_ = text.size() + 1;
    char* str = (char*)malloc(size_);
	QByteArray ba2(codec->fromUnicode(text));
    qstrncpy(str, ba2.data(), size_);
	if(selected_mac != (quint64)-1)
	{
		MessageHandler* mes = list_connection.getCon(0);
		if(mes)
		{
			quint64 mac = selected_mac;
            sendMsg(mac, str, size_-1);
			QString name = *(mes->tablets.getNameConnection(mac));
			
			ui.lineEdit->clear();
			text = "\"" + text + "\"";
            QString t = QString(InsertColor(QString("Send to "), COLOR_TEXT_BLACK))
                    + QString(InsertColor(name, COLOR_TEXT_BLUE))
                    + QString(InsertColor(QString(": "), COLOR_TEXT_BLACK))
                    + QString(InsertColor(text, COLOR_TEXT_DARK_GREEN))
                    + QString("\n");
            QTextCursor cursor = ui.text_log->textCursor();
			StringColorParser::parseString(t, &cursor);
			ui.text_log->append("\n");
			_log->tracer(t);
		}
	}
	else
	{
        QString str_err(InsertColor(QString("Error: selected device!"), COLOR_TEXT_RED));
        QTextCursor cursor = ui.text_log->textCursor();
		StringColorParser::parseString(str_err, &cursor);
		ui.text_log->append("\n");
	}
	free(str);
}

void MapViewer::send_commandAll()
{
	QString text(command_edit->text());
	QTextCodec *codec = QTextCodec::codecForName("KOI8-R");
    int size_ = text.size()+1;
    char* str = (char*)malloc(size_);
	QByteArray ba2(codec->fromUnicode(text));
    qstrncpy(str, ba2.data(), size_);
	MessageHandler* mes = list_connection.getCon(0);
	if(mes)
	{
		ListConnection _list = mes->tablets;
		QMap<quint64, ClientConnection*> con = _list.getMapConnection();
		foreach(ClientConnection* obj, con)
		{
			quint64 mac = obj->getIdConnection();
            sendMsg(mac, str, size_-1);
		}
		text = "\"" + text + "\"";
        QString t = QString(InsertColor(QString("Send all: "), COLOR_TEXT_BLACK)) + QString(InsertColor(text, COLOR_TEXT_DARK_GREEN));
        QTextCursor cursor = ui.text_log->textCursor();
		StringColorParser::parseString(t, &cursor);
        ui.text_log->append("\n");
		_log->tracer(t);
	}
	free(str);
}

void MapViewer::click_command(QListWidgetItem* wi)
{
	command_edit->setText(wi->text().simplified());
}

void MapViewer::timerEvent(QTimerEvent *event)
{
    //sphere::ProcessMessages();
    if (event->timerId() == _iClientsListTimer)
    {
        for (int i = 0; i < _lsRouters.size(); i++)
        {
            QTcpSocket * sock = new QTcpSocket(this);

            connect(sock, SIGNAL(readyRead()), this, SLOT(readClients()));
            sock->connect(sock, SIGNAL(disconnected()), SLOT(deleteLater()));
            QTimer::singleShot(5000, sock, SLOT(deleteLater())); /// 5 seconds

            sock->connectToHost(_lsRouters.at(i), UNITSDISCOVERYROUTERSTPORT);
        }
    }
}

MapViewer::~MapViewer()
{
    //sphere::Deinitialize();
}

void MapViewer::closeEvent(QCloseEvent *e)
{
	if (_log)
	{
		_log->saveHTML();
		_log->close();
	}
    emit close_window();
	QMainWindow::eventFilter(this, e);
}

void MapViewer::refreshMarks()
{
	typedef QList<MessageHandler*>::Iterator li;
	QGraphicsScene* scene = ui.graphicsView->scene();
	for(li iter = list_connection.list_mes_handlers.begin(); iter != list_connection.list_mes_handlers.end(); iter++)
	{
		MessageHandler* mes = *iter;
		if(mes)
		{
			int count_c = mes->tablets.getCountConnection();
			for(int i = 0; i < count_c; i++)
			{
				QMap<quint64, ClientConnection*> map_con = mes->tablets.getMapConnection();
				typedef QMap<quint64, ClientConnection*>::Iterator m_i;
				for(m_i iterM = map_con.begin(); iterM != map_con.end(); iterM++)
				{
					ClientConnection* cl_c = *iterM;
					typedef QVector<Point_tab*>::Iterator v_iP;
					for(QMap<int, Path_point*>::Iterator it1 = cl_c->paths.begin(); it1 != cl_c->paths.end(); it1++)
					{
						Path_point* ob = *it1;
						for(v_iP iterV_iP = ob->points.begin(); iterV_iP != ob->points.end(); iterV_iP++)
						{
						Point_tab* point_tab = *iterV_iP;
						typedef QVector<Mark_tab*>::Iterator v_iM;
						for(v_iM iterV_iM = point_tab->marks.begin(); iterV_iM != point_tab->marks.end(); iterV_iM++)
						{
							Mark_tab* mark_tab = *iterV_iM;
							scene->removeItem(mark_tab->this_mark);
							scene->removeItem(mark_tab->line_mark);
						}
						point_tab->marks.clear();
						scene->removeItem(point_tab->this_point);
						if(point_tab->line_point)
							scene->removeItem(point_tab->line_point);
						}
                    }
					cl_c->paths.clear();
					cl_c->mark_static_id = -1;
					cl_c->static_id = 0;
					cl_c->id_path = 0;
				}
			}
		}
	}
	foreach(EllipseLineText *obj, vectorMarks)
	{
		scene->removeItem(obj->ellipse);
		scene->removeItem(obj->text);
	}
	foreach(EllipseLineText *obj, vectorPhoto)
	{
		scene->removeItem(obj->ellipse);
		scene->removeItem(obj->text);
	}
	ui.listWidget_images->clear();
	ui.listWidget_images->hide();
    ui.text_log->clear();
	ui.treeWidget->clear();
	ui.listWidget_mark->clear();
	path_sel = 0;
	point_sel = 0;
	mark_sel = 0;
	MessageHandler* mes = list_connection.getCon(1);
	if(mes)
	{
		quint16 _n = 777;
		mes->sendMessage((const char*)&_n, sizeof(_n), 0);
	}
}

void MapViewer::deleteLayerConnect(MessageHandler* mes)
{
	QGraphicsScene* scene = ui.graphicsView->scene();
	if(mes)
	{
		int count_c = mes->tablets.getCountConnection();
		for(int i = 0; i < count_c; i++)
		{
			QMap<quint64, ClientConnection*> map_con = mes->tablets.getMapConnection();
			typedef QMap<quint64, ClientConnection*>::Iterator m_i;
			for(m_i iterM = map_con.begin(); iterM != map_con.end(); iterM++)
			{
				ClientConnection* cl_c = *iterM;
				typedef QVector<Point_tab*>::Iterator v_iP;
				for(QMap<int, Path_point*>::Iterator it1 = cl_c->paths.begin(); it1 != cl_c->paths.end(); it1++)
				{
					Path_point* ob = *it1;
					for(v_iP iterV_iP = ob->points.begin(); iterV_iP != ob->points.end(); iterV_iP++)
					{
						Point_tab* point_tab = *iterV_iP;
						typedef QVector<Mark_tab*>::Iterator v_iM;
						for(v_iM iterV_iM = point_tab->marks.begin(); iterV_iM != point_tab->marks.end(); iterV_iM++)
						{
							Mark_tab* mark_tab = *iterV_iM;
							scene->removeItem(mark_tab->this_mark);
							scene->removeItem(mark_tab->line_mark);
						}
						point_tab->marks.clear();
						scene->removeItem(point_tab->this_point);
						if(point_tab->line_point)
							scene->removeItem(point_tab->line_point);
					}
				}
				cl_c->points.clear();
				cl_c->paths.clear();
				cl_c->mark_static_id = -1;
				cl_c->static_id = 0;
				if(cl_c->getEllipse())
				{
					scene->removeItem(cl_c->getEllipse());
					scene->removeItem(cl_c->getRectText());
				}
			}
			map_con.clear();
		}
	}
	foreach(EllipseLineText *obj, vectorMarks)
	{
		scene->removeItem(obj->ellipse);
		scene->removeItem(obj->text);
	}
	foreach(EllipseLineText *obj, vectorPhoto)
	{
		scene->removeItem(obj->ellipse);
		scene->removeItem(obj->text);
	}
	ui.listWidget_images->clear();
	ui.listWidget_images->hide();
    ui.text_log->clear();
	ui.treeWidget->clear();
	ui.listWidget_mark->clear();
	path_sel = 0;
	point_sel = 0;
	mark_sel = 0;
}

void MapViewer::endAddPoint()
{
	add_point = false;
	ui.addPointTable->setEnabled(true);
	ui.endAddPoint->setEnabled(false);
	ui.addMark->setEnabled(false);
}

void MapViewer::addMark()
{
	ui.addPointTable->setEnabled(false);
	ui.addMark->setEnabled(false);
	add_point = false;
	add_point_mark = true;
}

void MapViewer::viewMark(QListWidgetItem* itemWidget)
{
	add_point_mark = false;
	add_point = false;
	int wi = ui.listWidget_mark->row(itemWidget);
	ui.graphicsView->centerOn(point_sel->marks[wi]->this_mark);
}

void MapViewer::sessionOpened()
{   
    _lsRouters.append(QString("192.168.101.1"));
    _lsRouters.append(QString("192.168.102.1"));
    _lsRouters.append(QString("192.168.103.1"));
    //_lsRouters.append(QString("192.168.104.1"));
    tcpServer = new QTcpServer(this);
    if (!tcpServer->listen(QHostAddress::Any, 7777)) {
        QMessageBox::critical(this, tr("PB Server"),
                              tr("Unable to start the server: %1.")
                              .arg(tcpServer->errorString()));
        return;
    }

    QString ipAddress;
    if (ipAddress.isEmpty())
        ipAddress = QHostAddress(QHostAddress::LocalHost).toString();
	QString t = tr("The server is running on\n\nIP: %1\nport: %2\n\n")
		.arg(ipAddress).arg(tcpServer->serverPort());

    ui.text_log->append(t);
    _log->tracer(t);//*/

    //--> new protocol ipv4
    struct ifreq ifr;
    memset(&ifr, 0, sizeof(ifr));
    strcpy(ifr.ifr_name, XBEETUN_IFNAME);
    int s = socket(AF_INET, SOCK_DGRAM, 0);
    if(s>0)
    {
        if( ioctl(s, SIOCGIFADDR, &ifr) >= 0 )
            _addr = ntohl(((struct sockaddr_in*)&ifr.ifr_addr)->sin_addr.s_addr);

        if( ioctl(s, SIOCGIFNETMASK, &ifr ) >= 0 )
            _mask = ntohl(((struct sockaddr_in*)&ifr.ifr_netmask)->sin_addr.s_addr);

        if( ioctl(s, SIOCGIFHWADDR, &ifr) >= 0 )
        {
            _mac = (_mac << 8) | (unsigned char)ifr.ifr_hwaddr.sa_data[0];
            _mac = (_mac << 8) | (unsigned char)ifr.ifr_hwaddr.sa_data[1];
            _mac = (_mac << 8) | (unsigned char)ifr.ifr_hwaddr.sa_data[2];
            _mac = (_mac << 8) | (unsigned char)ifr.ifr_hwaddr.sa_data[3];
            _mac = (_mac << 8) | (unsigned char)ifr.ifr_hwaddr.sa_data[4];
            _mac = (_mac << 8) | (unsigned char)ifr.ifr_hwaddr.sa_data[5];
        }
        ::close(s);
    }

    _strUnitName = QString("ARM");
    _ptmBcast = new QTimer(this);
    connect(_ptmBcast, SIGNAL(timeout()), this, SLOT(sendBroadcast()));
    _ptmBcast->start(10000);

    _pservClients = new QTcpServer(this);
    connect(_pservClients, SIGNAL(newConnection()), this, SLOT(newServCliensConnection()));
    _pservClients->listen(QHostAddress(_addr), UNITSDISCOVERYROUTERSTPORT);

    _pservSituation = new QTcpServer(this);
    connect(_pservSituation, SIGNAL(newConnection()), this, SLOT(newServSitConnection()));
    _pservSituation->listen(QHostAddress(_addr), UNITSSITUATIONPORT);

    _iClientsListTimer = startTimer(10000);
    _iMessagesTimer = startTimer(200);

    _psockBroad = new QUdpSocket(this);
    _psockBroad->bind(QHostAddress(_addr), 0);

    _psockDiscover = new QUdpSocket(this);
    _psockDiscover->bind(QHostAddress((_addr & _mask) | ~_mask), UNITSDISCOVERYBROADCASTPORT,
                         QUdpSocket::ShareAddress | QUdpSocket::ReuseAddressHint);
    connect(_psockDiscover, SIGNAL(readyRead()), this, SLOT(processPendingDiscoveryDatagrams()));

    _psockTelemetry = new QUdpSocket(this);
    _psockTelemetry->bind(QHostAddress(_addr), UNITSTELEMETRYPORT,
                          QUdpSocket::ShareAddress | QUdpSocket::ReuseAddressHint);
    connect(_psockTelemetry, SIGNAL(readyRead()), this, SLOT(processPendingTelemetryDatagrams()));

    QTcpSocket* sock_ = new QTcpSocket(this);
     MessageHandler *msgHandler = new MessageHandler(path_app, _log, this, sock_, auto_repeat);
     msgHandler->point_item = new QGraphicsEllipseItem(-4, -4, 8, 8, 0, ui.graphicsView->scene());
     list_connection.add(msgHandler);

    _pservMessages = new QTcpServer(this);
    connect(_pservMessages, SIGNAL(newConnection()), this, SLOT(newServMessagesConnection()));
    _pservMessages->listen(QHostAddress(_addr), UNITSMESSAGESPORT);

    _pservRangef = new QTcpServer(this);
    connect(_pservRangef, SIGNAL(newConnection()), this, SLOT(newRangefConnection()));
    _pservRangef->listen(QHostAddress(_addr), RANGEFPORT);
    //<-- new protocol ipv4
}

void MapViewer::addPointTable()
{
	add_point = true;
	add_point_mark = false;
	ui.endAddPoint->setEnabled(true);
	ui.addPointTable->setEnabled(false);
	ui.addMark->setEnabled(false);
}

void MapViewer::send_message()
{
    QString text(ui.lineEdit->text());
	QTextCodec *codec = QTextCodec::codecForName("KOI8-R");
    int size_ = text.size()+1;
    char* str = (char*)malloc(size_);
	QByteArray ba2(codec->fromUnicode(text));
    qstrncpy(str, ba2.data(), size_);
    if(selected_mac != (quint64)-1)
	{
		MessageHandler* mes = list_connection.getCon(0);
		if(mes)
		{
            quint64 mac = selected_mac;
            sendMsg(mac, str, size_-1);
			QString *name = mes->tablets.getNameConnection(mac);
			ui.lineEdit->clear();
			text = "\"" + text + "\"";
            QString t = QString(InsertColor(QString("Send to "), COLOR_TEXT_BLACK))
                    + QString(InsertColor(*name, COLOR_TEXT_BLUE))
                    + QString(InsertColor(QString(": "), COLOR_TEXT_BLACK))
                    + QString(InsertColor(text, COLOR_TEXT_DARK_GREEN));
            QTextCursor cursor = ui.text_log->textCursor();
			StringColorParser::parseString(t, &cursor);
            ui.text_log->append("\n");
			_log->tracer(t);
		}
	}
	else
	{
		QString str_err(InsertColor(QString("Error: selected device!"), COLOR_TEXT_RED));
        QTextCursor cursor = ui.text_log->textCursor();
		StringColorParser::parseString(str_err, &cursor);
		ui.text_log->append("\n");
    }
    free(str);
}

void MapViewer::send_messageAll()
{
	QString text(ui.lineEdit->text());
	QTextCodec *codec = QTextCodec::codecForName("KOI8-R");
    int size_ = text.size()+1;
    char* str = (char*)malloc(size_);
	QByteArray ba2(codec->fromUnicode(text));
    qstrncpy(str, ba2.data(), size_);
	MessageHandler* mes = list_connection.getCon(0);
	if(mes)
	{
		ListConnection _list = mes->tablets;
		QMap<quint64, ClientConnection*> con = _list.getMapConnection();
		foreach(ClientConnection* obj, con)
		{
			quint64 mac = obj->getIdConnection();
            sendMsg(mac, str, size_-1);
		}
		ui.lineEdit->clear();
		text = "\"" + text + "\"";
        QString t = QString(InsertColor(QString("Send to all: "), COLOR_TEXT_BLACK)) + QString(InsertColor(text, COLOR_TEXT_DARK_GREEN));
        QTextCursor cursor = ui.text_log->textCursor();
		StringColorParser::parseString(t, &cursor);
        ui.text_log->append("\n");
		_log->tracer(t);
	}
	free(str);
}

void MapViewer::viewTable(QListWidgetItem* itemWidget)
{
	add_point_mark = false;
	add_point = false;

	int wi = ui.listWidget->row(itemWidget);
	MessageHandler* mes = list_connection.getCon(0);
	if(mes)
	{
		con_sel = mes->tablets.getClientConnectionFromWidgetId(wi);
		if(con_sel && con_sel->widgetId >= 0)
		{
			selected_mac = con_sel->getIdConnection();
			ui.addPath_button->setEnabled(true);
			ui.addPointTable->setEnabled(false);
			ui.addMark->setEnabled(false);
			ui.endAddPoint->setEnabled(false);
            ui.graphicsView->centerOn(con_sel->getEllipse());
			ui.listWidget_mark->clear();
            ui.treeWidget->clear();
			for(QMap<int, Path_point*>::Iterator it1 = con_sel->paths.begin(); it1 != con_sel->paths.end(); it1++)
			{
				Path_point* ob = *it1;
				QTreeWidgetItem* item = new QTreeWidgetItem(ob->toListString());
				ui.treeWidget->addTopLevelItem(item);
				for(QVector<Point_tab*>::Iterator it2 = ob->points.begin(); it2 != ob->points.end(); it2++)
				{
					Point_tab* ob2 = *it2; 
                    new QTreeWidgetItem(item, QStringList(ob2->ToString()));
				}
			}
			hideShowMarks(wi);
		}
		else
			ui.listWidget->setCurrentRow(0);
	}
}
void MapViewer::hideShowMarks(int wi)
{
	MessageHandler* mes = list_connection.getCon(0);
	if(mes)
	{
		typedef QVector<Point_tab*>::Iterator pti;
		typedef QVector<Mark_tab*>::Iterator mti;
		for(int i = 0; i < mes->tablets.getCountConnection(); i++)
		{
			if(i != wi)
			{
				ClientConnection* iter_con = mes->tablets.getClientConnectionFromWidgetId(i);
				if(iter_con)
				{
					for(QMap<int, Path_point*>::Iterator it1 = iter_con->paths.begin(); it1 != iter_con->paths.end(); it1++)
					{
						Path_point* ob1 = *it1;
						for(pti iter = ob1->points.begin(); iter != ob1->points.end(); iter++)
						{
							Point_tab *sel_p = *iter;
							sel_p->this_point->setEnabled(false);
							sel_p->this_point->setVisible(false);
							if(sel_p->line_point)
							{
								sel_p->line_point->setEnabled(false);
								sel_p->line_point->setVisible(false);
							}
							if(sel_p->marks.count() > 0)
							{
								for(mti iter_m = sel_p->marks.begin(); iter_m != sel_p->marks.end(); iter_m++)
								{
									Mark_tab *sel_m = *iter_m;
									sel_m->this_mark->setEnabled(false);
									sel_m->this_mark->setVisible(false);
									sel_m->line_mark->setEnabled(false);
									sel_m->line_mark->setVisible(false);
								}
							}
						}
					}
				}
			}
		}

		for(QMap<int, Path_point*>::Iterator it1 = con_sel->paths.begin(); it1 != con_sel->paths.end(); it1++)
		{
			Path_point* ob1 = *it1;
			for(pti iter = ob1->points.begin(); iter != ob1->points.end(); iter++)
			{
				Point_tab *sel_p = *iter;
				sel_p->this_point->setEnabled(true);
				sel_p->this_point->setVisible(true);
				if(sel_p->line_point)
				{
					sel_p->line_point->setEnabled(true);
					sel_p->line_point->setVisible(true);
				}
				if(sel_p->marks.count() > 0)
				{
					for(mti iter_m = sel_p->marks.begin(); iter_m != sel_p->marks.end(); iter_m++)
					{
						Mark_tab *sel_m = *iter_m;
						sel_m->this_mark->setEnabled(true);
						sel_m->this_mark->setVisible(true);
						sel_m->line_mark->setEnabled(true);
						sel_m->line_mark->setVisible(true);
					}
				}
			}
		}
	}
}
void MapViewer::viewTable_point(QTreeWidgetItem* itemWidget)
{
	add_point_mark = false;
	add_point = false;
	ui.addPointTable->setEnabled(false);
	ui.addMark->setEnabled(true);
	ui.endAddPoint->setEnabled(false);

	int wi = itemWidget->parent()->indexOfChild(itemWidget)/*ui.listWidget_Points->row(itemWidget)*/;
	MessageHandler* mes = list_connection.getCon(0);
	if(mes)
	{
        point_sel = path_sel->points[wi];
		ui.graphicsView->centerOn(point_sel->this_point);
		ui.listWidget_mark->clear();
        //int it = 0;
		typedef QVector<Mark_tab*>::Iterator VI;
		for(VI iter = point_sel->marks.begin(); iter != point_sel->marks.end(); iter++)
		{
			Mark_tab* e = *iter;
			QString n_p = e->ToString();
			ui.listWidget_mark->addItem(n_p);
		}
	}
}

void MapViewer::addMarkFromSignal(QPointF pos, QString name, int size, quint16 _type)
{
    size++;
	QString str = InsertColor(QString("Reseived signal of mark: "), COLOR_TEXT_BLACK) + InsertColor(tr("%1 :").arg(QuoteDouble(name)), COLOR_TEXT_BLUE) + InsertColor(tr("x = %1; y = %2;").arg(QuoteSquare(QString::number(pos.x()))).arg(QuoteSquare(QString::number(pos.y()))), COLOR_TEXT_ORANGE);
	_log->tracer(str);
	QGraphicsEllipseItem* item = new QGraphicsEllipseItem(-1.5, -1.5, 3, 3, 0, ui.graphicsView->scene());
    QPointF posit = recoder(pos);
    if(!howRecord::useSK42)
		item->setPos(recoder(pos));
	else
		item->setPos(recoderWGSToSK(pos));
	item->setBrush(QBrush(Qt::gray, Qt::SolidPattern));
	QGraphicsSimpleTextItem* item_text = new QGraphicsSimpleTextItem(name, item, ui.graphicsView->scene());
	QFont serifFont("Times", 2, QFont::Bold);
    item_text->setFont(serifFont);
	EllipseLineText *val = new EllipseLineText;
	val->ellipse = item;
	val->text = item_text;
    val->pix = 0;
	vectorMarks.push_back(val);
	MessageHandler* m_c = list_connection.getCon(1);
	if(m_c)
    {
		if(!howRecord::useSK42)
			m_c->sendPointToComp(selected_mac, recoder(pos), name, 0, 4);
		else
			m_c->sendPointToComp(selected_mac, recoderWGSToSK(pos), name, 0, 4);
    }
	item_text->setPos(3, 3);

}
void MapViewer::newConnection()
{   
    qDebug() << "Got new connection";
	_log->tracer(QString("Got new connection"));
    QTcpSocket *clientConnection = tcpServer->nextPendingConnection();
		
	MessageHandler *msgHandler = new MessageHandler(path_app, _log, this, clientConnection, auto_repeat);
	msgHandler->point_item = new QGraphicsEllipseItem(-4, -4, 8, 8, 0, ui.graphicsView->scene());
	list_connection.add(msgHandler);
	connect(msgHandler, SIGNAL(onCreate(MessageHandler*, quint64)), this, SLOT(create_connection(MessageHandler*,quint64)));
	connect(msgHandler, SIGNAL(disconnected(MessageHandler*)), SLOT(disconnected(MessageHandler*)));
	connect(msgHandler, SIGNAL(readyRead(MessageHandler*)), this, SLOT(readyRead(MessageHandler*)));
	connect(msgHandler, SIGNAL(setPos(quint64, QPointF, QGraphicsEllipseItem*, QByteArray)), this, SLOT(setPos(quint64, QPointF, QGraphicsEllipseItem*, QByteArray)));
	connect(msgHandler, SIGNAL(signalChangeName(MessageHandler*, quint64, QString)), this, SLOT(change_name_con(MessageHandler*, quint64, QString)));
	connect(msgHandler, SIGNAL(messageToGuiSignal(MessageHandler*, quint64, QString, QString)), this, SLOT(messageToGui(MessageHandler*, quint64, QString, QString)));
	connect(msgHandler, SIGNAL(dalnomerMarkSignal(QPointF, QString, int, quint16)), this, SLOT(addMarkFromSignal(QPointF, QString, int, quint16)));
	connect(this, SIGNAL(close_window()), msgHandler, SLOT(close_slot()));
	connect(msgHandler, SIGNAL(confirm_message(quint16)), this, SLOT(checkConfirm(quint16)));
	connect(msgHandler, SIGNAL(change_state_signal(MessageHandler*, quint64, uchar)), this, SLOT(change_state(MessageHandler*, quint64, uchar)));
	connect(msgHandler, SIGNAL(sendTo3DComp_signal(TELEMETRY*)), this, SLOT(sendTo3D(TELEMETRY*)));
	connect(msgHandler, SIGNAL(readPicture_signal(QPointF, QString, QString)), this, SLOT(readPicture(QPointF, QString, QString)));
	connect(msgHandler, SIGNAL(picture_signal(quint64, QImage*, QString, QString, QPointF)), this, SLOT(picture_input(quint64, QImage*, QString, QString, QPointF)));

	qDebug() << "Accept it";
	_log->tracer(QString("Accept it"));
}

void MapViewer::readPicture(QPointF pos, QString filename, QString name)
{
	_log->tracer(filename);
	QPixmap _image(filename, "JPG");
	QGraphicsEllipseItem* _ellipse = new QGraphicsEllipseItem(-1.5, -1.5, 3, 3, 0, ui.graphicsView->scene());
	QBrush brush(QColor(255, 0, 255), Qt::SolidPattern);
	_ellipse->setBrush(brush);
	_ellipse->setData(0, filename);
	QPointF _pos;
	if(!howRecord::useSK42)
		_pos = recoder(pos);
	else
		_pos = recoderWGSToSK(pos);
	_ellipse->setPos(_pos);
	QGraphicsSimpleTextItem* item_text = new QGraphicsSimpleTextItem(name, _ellipse, ui.graphicsView->scene());
	QFont serifFont("Times", 2, QFont::Bold);
	item_text->setFont(serifFont);
	item_text->setPos(3, 3);
	EllipseLineText *val = new EllipseLineText;
	val->ellipse = _ellipse;
	val->text = item_text;
	vectorPhoto.push_back(val);
	QListWidgetItem *qli = new QListWidgetItem(ui.listWidget_images);
	lListWidgetUserData data_i(_pos, filename);
	QIcon *icon = new QIcon(_image);
	qli->setIcon(*icon);
	qli->setText(name);
	map_data_listWidget_image.insert(qli, data_i);
	if(ui.listWidget_images->isHidden())
		ui.listWidget_images->show();
}

void MapViewer::picture_input(quint64 mac, QImage* image, QString name, QString filename, QPointF pos)
{
    mac++;
	QTextCursor cursor = ui.text_log->textCursor();
	cursor.clearSelection();
	cursor.movePosition(QTextCursor::End);
	QImage qim = *image;
	qim = qim.scaled(128, 128, Qt::KeepAspectRatio);
	cursor.insertImage(qim, name);
	cursor.insertText("\n");

	QImage qim2 = *image;
	QPixmap _image = QPixmap::fromImage(qim2);
	QGraphicsEllipseItem* _ellipse = new QGraphicsEllipseItem(-1.5, -1.5, 3, 3, 0, ui.graphicsView->scene());
	QBrush brush(QColor(255, 0, 255), Qt::SolidPattern);
	_ellipse->setBrush(brush);
	_ellipse->setData(0, filename);
	QPointF _pos;
	if(!howRecord::useSK42)
		_pos = recoder(pos);
	else
		_pos = recoderWGSToSK(pos);
	_ellipse->setPos(_pos);
	QGraphicsSimpleTextItem* item_text = new QGraphicsSimpleTextItem(name, _ellipse, ui.graphicsView->scene());
	QFont serifFont("Times", 2, QFont::Bold);
	item_text->setFont(serifFont);
	item_text->setPos(3, 3);
	EllipseLineText *val = new EllipseLineText;
	val->ellipse = _ellipse;
	val->text = item_text;
	vectorPhoto.push_back(val);
	QListWidgetItem *qli = new QListWidgetItem(ui.listWidget_images);
	lListWidgetUserData data_i(_pos, filename);
	QIcon *icon = new QIcon(_image);
	qli->setIcon(*icon);
	qli->setText(name);
	map_data_listWidget_image.insert(qli, data_i);
	if(ui.listWidget_images->isHidden())
		ui.listWidget_images->show();
}

void MapViewer::showImage(QListWidgetItem* itemWidget)
{
	lListWidgetUserData data_i = map_data_listWidget_image.value(itemWidget);
	if(data_i.getPath_image() != "")
    {
		QString filename = data_i.getPath_image();
		QPointF pos = data_i.getPos_image();
		if(pos.x() != 0 && pos.y() != 0)
            ui.graphicsView->centerOn(pos);
        QPixmap _pixmap(filename, "JPG");
		if(!_pixmap.isNull())
		{
			QLabel* _label = new QLabel(0,Qt::Drawer|Qt::WindowStaysOnTopHint);
			_label->setWindowTitle(filename);
            _label->setScaledContents(true);
			_label->setPixmap(_pixmap);
			_label->show();
		}
		else
		{
			_log->tracer(InsertColor(QString("Not Pixmap"), COLOR_TEXT_RED));
			_log->tracer(filename);
		}
	}
}
void MapViewer::create_connection(MessageHandler* mess, quint64 mac)
{
    mess->tablets.setEllipse(mac, ui.graphicsView->scene());
    QPixmap pict = QPixmap(":/MapViewer/Resources/AK-74.png", "PNG");
	if(pict.isNull())
        qDebug() << "error";
	ClientConnection* cl_con = mess->tablets.getClientConnection(mac);
    cl_con->icon_pix = new QGraphicsPixmapItem(pict, mess->tablets.getEllipse(mac),
                                               ui.graphicsView->scene());
	cl_con->icon_pix->setScale(0.1);
	cl_con->icon_pix->setEnabled(ClientConnection::doPictures);
    cl_con->icon_pix->setVisible(ClientConnection::doPictures);
	
	QString *str = mess->tablets.getNameConnection(mac);
	QListWidgetItem *it = new QListWidgetItem(*mess->tablets.getNameConnection(mac));
	it->setData(0, double(mac));
	it->setData(1, *str);
    ui.listWidget->addItem(it);
	if(!mess->comp)
		mess->tablets.setWidgetId(mac, ui.listWidget->count()-1);
	else
		mess->tablets.setWidgetId(mac, -1);
}

void MapViewer::change_name_con(MessageHandler* msgHandler, quint64 mac, QString name)
{
	create_connection(msgHandler, mac);
	int id = msgHandler->tablets.getWidgetId(mac);
	msgHandler->tablets.setTextForItem(mac, name);
    QListWidgetItem* l_w = ui.listWidget->item(id);
	l_w->setData(1, name);
	ui.listWidget->item(id)->setText(name);
}

void MapViewer::checkConfirm(quint16 _t)
{
	MessageHandler* mes = list_connection.getCon(0);
	if(mes)
	{
		typedef QVector<Point_tab*>::Iterator pti;
		typedef QVector<Mark_tab*>::Iterator mti;
		for(int i = 0; i < mes->tablets.getCountConnection(); i++)
		{
			ClientConnection* iter_con = mes->tablets.getClientConnectionFromWidgetId(i);
			if(iter_con)
			{
				for(QMap<int, Path_point*>::Iterator i1 = iter_con->paths.begin(); i1 != iter_con->paths.end(); i1++)
				{
					Path_point* i2 = *i1;
					for(pti iter = i2->points.begin(); iter != i2->points.end(); iter++)
					{
						Point_tab *sel_p = *iter;
						if(sel_p->id_message_point == _t)
						{
							QBrush brush(Qt::green);
							sel_p->this_point->setBrush(brush);
							return;
						}
						if(sel_p->marks.count() > 0)
						{
							for(mti iter_m = sel_p->marks.begin(); iter_m != sel_p->marks.end(); iter_m++)
							{
								Mark_tab *sel_m = *iter_m;
								if(sel_m->id_message_mark == _t)
								{
									QBrush brush(Qt::blue);
									sel_m->this_mark->setBrush(brush);
									return;
								}
							}
						}
					}
				}
			}
		}
	}
	return;
}

void MapViewer::change_state(MessageHandler* msgHandler, quint64 mac, uchar state)
{
	ClientConnection* con = msgHandler->tablets.getClientConnection(mac);
	if(state == (uchar)0 && con->getEllipse())
	{
		QBrush brush(QColor(170, 170, 255), Qt::SolidPattern);
		con->getEllipse()->setBrush(brush);
	}
	if(state == (uchar)1 && con->getEllipse())
	{
		QBrush brush(Qt::red, Qt::SolidPattern);
		con->getEllipse()->setBrush(brush);
	}
	if(state == (uchar)2 && con->getEllipse())
	{
		QBrush brush(Qt::yellow, Qt::SolidPattern);
		con->getEllipse()->setBrush(brush);
	}
}
void MapViewer::sendTo3D(TELEMETRY* pmsg)
{
	if(list_connection.list_mes_handlers.count() > 1)
	{
		MessageHandler* mes = list_connection.getCon(1);
		if(mes)
		{
			QPointF _pos(pmsg->pos.x, pmsg->pos.y);
			if(pmsg->state == (uchar)1)
            {
                _pos = recoder(_pos);
                pmsg->pos.x = _pos.x();
                pmsg->pos.y = _pos.y();
            }
			mes->sendMessage((const char*)pmsg, sizeof(TELEMETRY), 0);
		}
	}
}
void MapViewer::setPos(quint64 mac, QPointF e, QGraphicsEllipseItem* ellipse, QByteArray name)
{
    name.size();
	if(ellipse)
	{
		MessageHandler* mes = list_connection.getCon(0);
        mes->tablets.getClientConnection(mac)->setPosition(e);
	}
}
void MapViewer::selectedObject(QPointF pos)
{
	ui.text_infoObject->clear();
	Append(ui.text_infoObject, QString::fromLocal8Bit("Г-К"), QString("#FF0000"));
	QPointF position = recoderMSKToSK42(pos);
	QString str = tr("X = %1;<br/>Y = %2;").arg(QString::number(position.x(), 'f')).arg(QString::number(position.y(), 'f'));
	Append(ui.text_infoObject, str, QString("#000000"));
	Append(ui.text_infoObject, QString("WGS-84"), QString("#0000FF"));
	QPointF pos1;
	if(!howRecord::useSK42)
		pos1 = recoderToGrad(pos);
	else
		pos1 = recoderSKToWGS(pos);
	int gradx, minutex; float secx;
	int grady, minutey; float secy;
	gradx = (int)pos1.x();
	minutex = (pos1.x() - gradx) * 60;
	secx = ((pos1.x()-gradx)*60 - minutex) * 60;
	grady = (int)pos1.y();
	minutey = (pos1.y() - grady) * 60;
	secy = ((pos1.y()-grady)*60 - minutey) * 60;
	str = tr("Lx = %1° %2' %3\";<br/>By = %4° %5' %6\";").arg(gradx).arg(minutex).arg(secx).arg(grady).arg(minutey).arg(secy);
	Append(ui.text_infoObject, str, QString("#000000"));
}

void MapViewer::messageToGui(MessageHandler* msgHandler, quint64 mac, QString name, QString text)
{
	text = "\"" + text + "\"";
	name = name + ": ";
	QTextCursor cursor = ui.text_log->textCursor();
	QString t = QString(InsertColor(name, COLOR_TEXT_BLUE)) + QString(InsertColor(text, COLOR_TEXT_GREEN));
	t = StringColorParser::parseString(t, &cursor);
	ui.text_log->append("\n");
	_log->tracer(t);
}
void MapViewer::disconnected(MessageHandler* mess)
{
	_log->tracer(InsertColor(QString("Got disconnect. Ready to new."), COLOR_TEXT_RED));
	deleteLayerConnect(mess);
	if(!mess->comp)
		ui.listWidget->clear();
	list_connection.remove(mess);
	mess->deleteLater();
	mess = 0;
}

void MapViewer::readyRead(MessageHandler* msgHandler)
{
	if (msgHandler)
	{
		QByteArray ba;

		while (msgHandler->recvMessage(ba))
		{
            DebugMessage(ba, msgHandler);
		}
	}
}

void MapViewer::drawSuperMap(QString name)
{

    /*if (FILE * file = fopen(name.toStdString().c_str(), "rb"))
    {
        TDataPlane dataPlane;
        dataPlane.Read(file);
        fclose(file);

        TDataPlaneIterator * iter = dataPlane.CreateIterator();
        int co = 0;
        while (iter->Next())
        {
            const TDataPlaneElement& elem = iter->Current();

            co++;
            QPointF point_lt(elem.Left, elem.Top);
            //point_lt = recoderSK42ToMSK(point_lt);
            QPointF point_rb(elem.Right, elem.Bottom);
            //point_rb = recoderSK42ToMSK(point_rb);
            QRectF r1(0, 0, point_lt.x()-point_rb.x(), point_lt.y()-point_rb.y());
            //qDebug() << r1;
            //qDebug() << ui.graphicsView->sceneRect();
            QGraphicsRectItem *r;
            if(elem.Value != 0)
            {
                r = new QGraphicsRectItem(r1, 0, ui.graphicsView->scene());
                rect_vect.push_back(r);
                r->setPos(point_lt.x(), point_lt.y());
                QBrush brush(Qt::white, Qt::SolidPattern);
                brush.setColor(Qt::black);
                r->setBrush(brush);
                QPen pen(brush.color());
                r->setPen(pen);
            }
        }
        delete iter;
    }*/
}

bool MapViewer::eventFilter(QObject *obj, QEvent *ev)
{
    //sphere::ProcessMessages();

	if(obj == ui.centralWidget)
	//{
        if(ev->type() == QEvent::Resize)
        {
				int cw_w = ui.centralWidget->width();  // ширина центрального виджета
				int cw_h = ui.centralWidget->height(); // высота центрального виджета

				// resize для graphicsView
				ui.graphicsView->resize(ui.centralWidget->width(), ui.centralWidget->height());

				// передвигаем listWidget_images
				int lw_i_w = ui.listWidget_images->width();
				int lw_i_h = ui.listWidget_images->height();
				ui.listWidget_images->move((cw_w - lw_i_w - 45), (cw_h/2 - lw_i_h/2) );

				// передвигаем widget_3
				int w_3_w = ui.widget_3->width();
				int w_3_h = ui.widget_3->height();
				ui.widget_3->move((cw_w - w_3_w - 45), (cw_h - w_3_h - 45));

				// передвигаем tableWidget
				int tw_w = ui.tableWidget->width();
				ui.tableWidget->move((cw_w - tw_w - 45), 15);

				QRect rect = ui.centralWidget->geometry();
				scroll->setGeometry(0, 0, 285, rect.height()-20);
        }
		switch(ev->type())
		{
		case QEvent::MouseMove :
			{
				QCursor c = ui.graphicsView->cursor();
				QPoint pos_c = c.pos();
				pos_c = ui.graphicsView->mapFromGlobal(pos_c);
				QPointF temp_point = ui.graphicsView->mapToScene(pos_c);
                qreal x = temp_point.x();
                qreal y = temp_point.y();
                QPointF position = QPointF(x, y);
				x = position.x();
				y = position.y();
				QString x_str = QString::number(x, 'f');
				QString y_str = QString::number(y, 'f');
				QString str = "x = " + x_str;
				str+= "; y = ";
				str+= y_str;
				str+= ";";
				str+= "\t|\t";
				QPointF _p = QPointF(x, y);
				if(!howRecord::useSK42)
					_p = recoderToGrad(_p);
				else
					_p = recoderSKToWGS(_p);
				degree_minute_sec deg(_p);
                str+=tr("Lx = %1° %2' %3\"; By = %4° %5' %6\";").arg(deg.deg_x).arg(deg.minute_x).arg(deg.sec_x).arg(deg.deg_y).arg(deg.minute_y).arg(deg.sec_y);
				ui.statusBar->showMessage(str);
				break;
			}
		case QEvent::MouseButtonDblClick:
			{
				if(add_point && con_sel && !add_point_mark)
				{
					QCursor c = ui.graphicsView->cursor();
					QPoint pos_c = c.pos();
					pos_c = ui.graphicsView->mapFromGlobal(pos_c);
					QPointF p = ui.graphicsView->mapToScene(pos_c);
					QGraphicsScene* scene = ui.graphicsView->scene();
					MessageHandler* m_h = list_connection.getCon(0);
				
					Point_tab* p_t = new Point_tab();
					p_t->this_point = new QGraphicsEllipseItem(-1.5, -1.5, 3, 3, 0, scene);
					p_t->this_point->setBrush(QBrush(QColor(85, 85, 127), Qt::SolidPattern));
					p_t->this_point->setPos(p.x(), p.y());
                    p_t->id_point = ++path_sel->id_point;
					p_t->id_message_point = m_h->getIdMessage() + 1;
					p_t->id_path = path_sel->id;
					if(p_t->id_point != 1)
					{
						QPointF prev_point = path_sel->points[p_t->id_point-2]->this_point->pos();
						QPointF this_point = p_t->this_point->pos();
						QPen pen(Qt::green, 0.5, Qt::DashDotLine, Qt::RoundCap, Qt::RoundJoin);
						p_t->line_point = new QGraphicsLineItem(prev_point.x(), prev_point.y(), this_point.x(), this_point.y(), 0, scene);
						p_t->line_point->setPen(pen);
                    }
                    new QTreeWidgetItem(it_sel, QStringList(p_t->ToString()));
                    path_sel->points.push_back(p_t);
					point_sel = p_t;
					if(m_h)
					{
						QPointF _t_sk;
						if(!howRecord::useSK42)
							_t_sk = recoderToGrad(p);
						else
							_t_sk = recoderSKToWGS(p);
						m_h->sendMessage_addPoint(selected_mac, _t_sk, p_t->ToString(), p_t->id_point, p_t->id_path);
					}
					MessageHandler* m_c = list_connection.getCon(1);
					if(m_c)
						m_c->sendPointToComp(selected_mac, p, p_t->ToString(), p_t->id_point, 1);
				}
				else if(add_point_mark && point_sel)
				{
					QCursor c = ui.graphicsView->cursor();
					QPoint pos_c = c.pos();
					pos_c = ui.graphicsView->mapFromGlobal(pos_c);
					QPointF p = ui.graphicsView->mapToScene(pos_c);
					QGraphicsScene* scene = ui.graphicsView->scene();
					MessageHandler* m_h = list_connection.getCon(0);
					Mark_tab* m_t = new Mark_tab();

					m_t->this_mark = new QGraphicsEllipseItem(-1.5, -1.5, 3, 3, 0, scene);
				
					m_t->this_mark->setBrush(QBrush(QColor(85, 85, 127), Qt::SolidPattern));
					m_t->this_mark->setPos(p.x(), p.y());
					m_t->id_mark = ++con_sel->mark_static_id;
				
					QPointF prev_point = point_sel->this_point->pos();
					QPointF this_point = m_t->this_mark->pos();
					QPen pen(Qt::red, 0.5, Qt::DashDotLine, Qt::RoundCap, Qt::RoundJoin);
					m_t->line_mark = new QGraphicsLineItem(prev_point.x(), prev_point.y(), this_point.x(), this_point.y(), 0, scene);
					m_t->line_mark->setPen(pen);
					m_t->id_message_mark = m_h->getIdMessage()+1;

					point_sel->marks.push_back(m_t);
					ui.listWidget_mark->addItem(m_t->ToString());
					ui.addMark->setEnabled(true);
					add_point_mark = false;
					if(m_h)
					{
						m_h->sendMessage_addMark(selected_mac, p, m_t->ToString(), point_sel, m_t->id_mark);
					}
					MessageHandler* m_c = list_connection.getCon(1);
					if(m_c)
						m_c->sendPointToComp(selected_mac, p, m_t->ToString(), m_t->id_mark, 2);
					//point_sel = p;
				}
			}
        case QEvent::MouseButtonRelease:
			{
				bool right_button = false;
				QMouseEvent *e = (QMouseEvent *)ev;
				Qt::MouseButton s = e->button();
                if(s == Qt::RightButton)
					right_button = true;
				if(right_button && obj->parent() != ui.treeWidget)
				{
					QPoint pos = e->globalPos();
					QPoint pos_c = pos;
					pos_c = ui.graphicsView->mapFromGlobal(pos_c);
					QPointF p = ui.graphicsView->mapToScene(pos_c);
					QGraphicsItem* it = ui.graphicsView->scene()->itemAt(p);
					if(it)
					{
						if(qgraphicsitem_cast<QGraphicsEllipseItem*>(it))
						{
							QGraphicsEllipseItem* t = (QGraphicsEllipseItem*)it;
							QMenu menu (this);
							QAction* action1 = menu.addAction(QString::fromLocal8Bit("Создать фантом"));
                            QAction* action2 = menu.addAction(QString::fromLocal8Bit("Разрушить фантом"));
							QAction* action = menu.exec(e->globalPos());
							MessageHandler* m_h = list_connection.getCon(0);
							if(action == action1)
                            {
								if(t->data(2).toBool() == true)
								{
									QMessageBox *mes = new QMessageBox(QMessageBox::Critical, QString::fromLocal8Bit("Внимание!"), QString::fromLocal8Bit("Фантом для данного объекта был создан ранее!"), QMessageBox::Close);
									mes->show();
								}
								else
								{
									quint64 mac = t->data(1).toDouble();
									ClientConnection *con = m_h->tablets.getClientConnection(mac);
									if(con)
									con->addFantom();
								}
							}
							else if(action == action2)
							{
								quint64 mac = t->data(1).toDouble();
								ClientConnection *con = m_h->tablets.getClientConnection(mac);
								con->deleteFantom();
							}
						}
                        if(qgraphicsitem_cast<Fantom*>(it))
						{
							Fantom* t = (Fantom*)it;
							QMenu menu (this);
							QAction* action1 = menu.addAction(QString::fromLocal8Bit("Разрешить перемещение"));
							QAction* action2 = menu.addAction(QString::fromLocal8Bit("Запретить перемещение"));
							QAction* action3 = menu.addAction(QString::fromLocal8Bit("VisibilityMap..."));
							QAction* actionPath = menu.addAction(QString::fromLocal8Bit("PathFind..."));
							QAction* actionCoveringMap = menu.addAction("CoveringMap..");
							QAction* actionCoveringPoint = menu.addAction("CoveringPoint..");
							QAction* actionReachableBound = menu.addAction("ReachableBound..");
							QAction* action4 = menu.addAction(QString::fromLocal8Bit("Отмена"));
							QMenu* mType = menu.addMenu(QString::fromLocal8Bit("Тип Объекта"));

							//30-мм автоматический гранатомет АГС-17 'Пламя'
							//Гранатометчик с РПГ-7
							//122-мм самоходная гаубица 2C1 Гвоздика
							//БМП-2

							QAction* a1 = mType->addAction(QString::fromLocal8Bit("30-мм автоматический гранатомет АГС-17 'Пламя'"));
							QAction* a2 = mType->addAction(QString::fromLocal8Bit("Гранатометчик с РПГ-7"));
							QAction* a3 = mType->addAction(QString::fromLocal8Bit("122-мм самоходная гаубица 2C1 Гвоздика"));
                            QAction* a4 = mType->addAction(QString::fromLocal8Bit("БМП-2"));

							QAction* action = menu.exec(e->globalPos());

							if (action==a1||
								action==a2||
								action==a3||
                                action==a4)
							{
								t->objType(action->text().toLocal8Bit().data());
								t->updateData();
							}


							MessageHandler* m_h = list_connection.getCon(0);
							if (action==actionCoveringMap)
							{
                                QPointF point2 = t->pos();

								double deltaX = (6178618.16+6177068.35)*0.5;
								double deltaY = (37413798.94+37415078.40)*0.5;

								TSphereCoveringMapRequestPacket temp;
								temp.SrcX = -(point2.x()-deltaX);//point1->Point().X;
								temp.SrcY = point2.y()-deltaY;//point1->Point().Y;
                                temp.ObjectType = t->objType();//"Автоматчик (Россия)";
								QString path_appData = qApp->applicationDirPath() + "/Data";
								temp.MatrixName = QString(path_appData+"/CityGradient_(3).hgtmf").toStdString();//d:/sphere/MapViewer/Data/CityGradient_mask_547x645.hgtmf";
								temp.OutputPathName = path_appData.toStdString();//"d:/sphere/MapViewer/Data/";

								_log->tracer(QString("SphereCoveringMapRequestPacket")+temp.MatrixName.c_str()+temp.OutputPathName.c_str());

								msg::TDataSetPtr system = msg::TDataSet::ConstructorWrite(MT_ID(TSphereCoveringMapRequestPacket));
								temp.Write(system);
								sphere::Send("CoveringMap_ClientPortToService", system);

								_log->tracer(QString("send.. SphereCoveringMapRequestPacket ")+temp.MatrixName.c_str()+" out: "+temp.OutputPathName.c_str());
							}						
							if (action==actionCoveringPoint)
							{
                                QPointF point2 = t->pos();

								double deltaX = (6178618.16+6177068.35)*0.5;
								double deltaY = (37413798.94+37415078.40)*0.5;

								TSphereCoveringPointRequestPacket temp;
								temp.SrcX = -(point2.x()-deltaX);//point1->Point().X;
								temp.SrcY = point2.y()-deltaY;//point1->Point().Y;
                                temp.ObjectType = t->objType();//"Автоматчик (Россия)";
                                QString path_appData = qApp->applicationDirPath() + "/Data";
								temp.MatrixName = QString(path_appData+"/CityGradient_(3).hgtmf").toStdString();//d:/sphere/MapViewer/Data/CityGradient_mask_547x645.hgtmf";
								temp.OutputPathName = path_appData.toStdString();//"d:/sphere/MapViewer/Data/";

								_log->tracer(QString("SphereCoveringPointRequestPacket")+temp.MatrixName.c_str()+temp.OutputPathName.c_str());

								msg::TDataSetPtr system = msg::TDataSet::ConstructorWrite(MT_ID(TSphereCoveringPointRequestPacket));
								temp.Write(system);
								sphere::Send("CoveringPoint_ClientPortToService", system);

								_log->tracer(QString("send.. SphereCoveringPointRequestPacket ")+temp.MatrixName.c_str()+" out: "+temp.OutputPathName.c_str());
							}						
							if (action==actionPath)
							{
                                TSpherePathfindRequestPacket temp;

								QPointF point1,point2;

								point1 = t->getEllipse()->pos();//selected_obj->pos();
                                point2 = t->pos();

								double deltaX = (6178618.16+6177068.35)*0.5;
								double deltaY = (37413798.94+37415078.40)*0.5;

								temp.SrcX = -(point1.x()-deltaX);//point1->Point().X;
								temp.SrcY = point1.y()-deltaY;//point1->Point().Y;
								temp.DstX = -(point2.x()-deltaX);//point2->Point().X;
								temp.DstY = point2.y()-deltaY;//point2->Point().Y;

								static int requestID = 0;

								temp.RequestID  = requestID++;
								QString path_appData = qApp->applicationDirPath() + "/Data/";
								temp.MatrixName = QString(path_appData+"CityGradient_b.hgtmf").toStdString();//d:/sphere/MapViewer/Data/CityGradient_mask_547x645.hgtmf";

								msg::TDataSetPtr system = msg::TDataSet::ConstructorWrite(MT_ID(TSpherePathfindRequestPacket));
								temp.Write(system);
								sphere::Send("Pathfind_ClientPortToService", system);
							}
							if (action==actionReachableBound)
							{
                                TSphereReachableBoundRequestPacket temp;
								
                                QPointF point2 = t->pos();

								double deltaX = (6178618.16+6177068.35)*0.5;
								double deltaY = (37413798.94+37415078.40)*0.5;

                                temp.SrcX = -(point2.x()-deltaX);
                                temp.SrcY = point2.y()-deltaY;
                                temp.Speed = 5;
								temp.Time = 30;

								static int requestID = 0;

								temp.RequestID  = requestID++;
								QString path_appData = qApp->applicationDirPath() + "/Data/";
								temp.MatrixName = QString(path_appData+"CityGradient_b.hgtmf").toStdString();//d:/sphere/MapViewer/Data/CityGradient_mask_547x645.hgtmf";

								msg::TDataSetPtr system = msg::TDataSet::ConstructorWrite(MT_ID(TSphereReachableBoundRequestPacket));
								temp.Write(system);
								sphere::Send("ReachableBound_ClientPortToService", system);
							}
							if(action == action1)
                            {
								t->setAcceptDrops(true);
								t->setFlag(QGraphicsItem::ItemIsMovable, true);
							}
							if(action == action2)
							{
								t->setAcceptDrops(false);
								t->setFlag(QGraphicsItem::ItemIsMovable, false);
							}
							if(action == action3)
                            {
                                QPointF point2 = t->pos();

								//6177596.03, 37414370.63
								//
								//POINT2 leftTop(6178618.16, 37413798.94);							
								//POINT2 rightBottom(6177068.35, 37415078.40);

								double deltaX = (6178618.16+6177068.35)*0.5;
                                double deltaY = (37413798.94+37415078.40)*0.5;

                                TSphereVisMapRequestPacket temp;
                                temp.AddPoint(-(point2.x()-deltaX), (point2.y()-deltaY));
                                temp.HeightOverMap=1.8;
                                temp.MaximumDistance = 1500;
                                QString path_appData = "Data\\";
                                temp.MatrixName        = QString(path_appData+"CityGradient_b.hgtmf").toStdString();
                                temp.OutputPathName    = path_appData.toStdString();
                                temp.SmoothProbability = false;
                                temp.AppInfo           = TCommunicator::Instance().ApplicationInfo;

                                _log->tracer(QString::fromLocal8Bit("!!!!!path_appData")+path_appData+"\n");
                                _log->tracer(QString::fromLocal8Bit("!!!!!MatrixName")+QString::fromLocal8Bit(temp.MatrixName.c_str())+"\n");
                                _log->tracer(QString::fromLocal8Bit("!!!!!OutputPathName")+QString::fromLocal8Bit(temp.OutputPathName.c_str())+"\n");

                                msg::TDataSetPtr system = msg::TDataSet::ConstructorWrite(MT_ID(TSphereVisMapRequestPacket));
                                temp.Write(system);
                                sphere::Send("VisMap_ClientPortToService", system);
							}
                        }
					}
				}
				else
				{
					if(contol_key && select_position)
					{
						QCursor c = ui.graphicsView->cursor();
						QPoint pos_c = c.pos();
						pos_c = ui.graphicsView->mapFromGlobal(pos_c);
						QPointF p = ui.graphicsView->mapToScene(pos_c);

						QPointF pos1;
						if(!howRecord::useSK42)
							pos1 = recoderToGrad(p);
						else
							pos1 = recoderSKToWGS(p);
						emit selectPosition(pos1);
						select_position = false;
						break;
					}
				}
            }//*/
		case QEvent::MouseButtonPress:
			{		
				if(!select_position)
                {
					if(shift_key)
					{
						QCursor c = ui.graphicsView->cursor();
						QPoint pos_c = c.pos();
						pos_c = ui.graphicsView->mapFromGlobal(pos_c);
                        QPointF p = ui.graphicsView->mapToScene(pos_c);
				
						QGraphicsItem* it = ui.graphicsView->scene()->itemAt(p.x(), p.y());
						if(it)
						{
							if(selected_obj)
							{
								if(qgraphicsitem_cast<QGraphicsPolygonItem*>(selected_obj))
								{
									QGraphicsPolygonItem* t = (QGraphicsPolygonItem*)selected_obj;
									QPen pen(Qt::black);
									t->setPen(pen);
								}
								else
									if(qgraphicsitem_cast<QGraphicsEllipseItem*>(it))
									{
										QGraphicsEllipseItem* t = (QGraphicsEllipseItem*)selected_obj;
										QPen pen(Qt::black);
										t->setPen(pen);
									}
                            }
							if(qgraphicsitem_cast<QGraphicsPolygonItem*>(it))
							{
								QGraphicsPolygonItem* t = (QGraphicsPolygonItem*)it;
								QPen pen(Qt::darkRed);
								pen.setWidth(2);
								t->setPen(pen);
								QPointF pos = t->pos();
								selected_obj = t;
								emit selectObject(pos);
							}
							else if(qgraphicsitem_cast<QGraphicsEllipseItem*>(it))
							{
								QGraphicsEllipseItem* t = (QGraphicsEllipseItem*)it;
								QPen pen(Qt::darkRed);
								pen.setWidth(1);
								t->setPen(pen);
								QPointF pos = t->pos();
								selected_obj = t;
								emit selectObject(pos);
							}
						}
					}
					else if(contol_key)
					{
						QCursor c = ui.graphicsView->cursor();
						QPoint pos_c = c.pos();
						pos_c = ui.graphicsView->mapFromGlobal(pos_c);
                        QPointF p = ui.graphicsView->mapToScene(pos_c);

						QGraphicsItem* it = ui.graphicsView->scene()->itemAt(p.x(), p.y());
						if(it)
						{
							if(qgraphicsitem_cast<QGraphicsEllipseItem*>(it))
							{
								QGraphicsEllipseItem* t = (QGraphicsEllipseItem*)it;
								if(t->data(0) != "")
								{
									QString filename = t->data(0).toString();
									if(filename != "")
									{
										QImage _image(filename, "JPG");
										QLabel* _label = new QLabel();
										_label->move(c.pos());
										_label->setPixmap(QPixmap::fromImage(_image));
										_label->show();
                                        contol_key = false;
									}
								}
							}
						}
					}
                }

			}
			break;
			//}
		case QEvent::Wheel :
			if (contol_key)
			{
				QWheelEvent *e = (QWheelEvent*)ev;
				if(e->delta() < 0)
					ui.graphicsView->scale(0.95, 0.95);
				else
					ui.graphicsView->scale(1.05, 1.05);
				return true;
			}
                break;
		default:
            break;
		}
	return QMainWindow::eventFilter(obj, ev);
}

bool MapViewer::initialMap(QString file_name)
{
	bool b = ReadFileMap(file_name, &map_objects);
	if(!b)
	{
		qDebug("Error ReadFileMap");
        _log->tracer(InsertColor(QString("Error read file [%1]").arg(file_name), COLOR_TEXT_RED));
		return false;
	}
    QPointF pos_view;
	QBrush brush(QColor(255, 85, 0, 170), Qt::SolidPattern);
	QPen pen(Qt::white);
	for(int i = 0; i<map_objects.getCount(); i++)
	{
		QPolygonF *poly = new QPolygonF();
		
		int _type = map_objects.getObject(i).type_obj;
		typedef QVector<QPointF>::iterator VI;
		QVector<QPointF> temp = map_objects.getObject(i).getContur();
		switch(map_objects.getObject(i).type_obj)
		{
		case 1: case 5:
			brush.setColor(QColor(165, 165, 165));
			break;
		case 2:
			brush.setColor(QColor(0, 255, 0));
			break;
		case 3: case 4:
			brush.setColor(QColor(0, 0, 255));
			break;
		case 8:
			brush.setColor(QColor(85, 255, 255, 120));
			break;
		default:
			brush.setColor(QColor(255, 85, 0, 170));
			break;
		}
		qreal min_x, max_x, min_y, max_y;
		min_x = temp[0].x();
		max_x = temp[0].x();
		min_y = temp[0].y();
		max_y = temp[0].y();
		for (VI iter = temp.begin(); iter != temp.end(); iter++)
		{
			QPointF e = *iter;
			qreal _x = e.x();
			qreal _y = e.y();
			if(_x < min_x)
				min_x = _x;
			if(_x > max_x)
				max_x = _x;
			if(_y < min_y)
				min_y = _y;
			if(_y > max_y)
                max_y = _y;
		}
		for (VI iter = temp.begin(); iter != temp.end(); iter++)
		{
			QPointF e = *iter;
			e.setX(e.x() - min_x);
			e.setY(e.y() - min_y);
            poly->push_back(e);
		}
		QGraphicsPolygonItem * _item = 0;
		QVector<QGraphicsLineItem *> line;
		QGraphicsRectItem *rect = 0;
		QGraphicsTextItem *text = 0;
		switch(_type)
		{
		case 1: case 2: case 3: case 8:
			_item = new QGraphicsPolygonItem(*poly, 0, ui.graphicsView->scene());
			break;
		case 0: case 6:
			{
				if(poly->count() > 1)
					_item = new QGraphicsPolygonItem(*poly, 0, ui.graphicsView->scene());
				else
				{
					rect = new QGraphicsRectItem(poly[0].first().x()-5, poly[0].first().y()-5, 10, 10, 0, ui.graphicsView->scene());
					brush.setColor(Qt::black);
				}
			}
			break;
		case 4: case 5:
			{
				QPointF _prev;
				int _it = 0;
				foreach(QPointF _point, poly->toList())
				{
					if(_it>0)
					{
						QGraphicsLineItem *_line = new QGraphicsLineItem(_prev.x(), _prev.y(), _point.x(), _point.y(), 0, ui.graphicsView->scene());
						line.push_back(_line);
					}
					_prev = _point;
					_it++;
				}
			}
			break;
		case 7:
            {
				text = new QGraphicsTextItem(map_objects.getObject(i).text, 0, ui.graphicsView->scene());
				QFont serifFont2("Times", 20, QFont::Bold);
				text->setFont(serifFont2);
				text->setPos(min_x, min_y);
			}
		default:
			break;
        }
		if(_item != 0)
		{
			_item->setBrush(brush);
			pen.setColor(brush.color());
			_item->setPen(pen);
			_item->setPos(min_x, min_y);
		} else if(rect != 0)
		{
			rect->setBrush(brush);
			pen.setColor(brush.color());
			rect->setPen(pen);
			rect->setPos(min_x, min_y);
		} else {
			foreach(QGraphicsLineItem* obj, line)
			{
				pen.setColor(brush.color());
				pen.setWidth(4);
				obj->setPen(pen);
				obj->setPos(min_x, min_y);
			}
        }
		pos_view = QPointF(min_x, min_y);
    }
	ui.graphicsView->scale(4, 4);
    ui.graphicsView->centerOn(pos_view);

	return true;
}

struct _points
{
	double x;
	double y;
};

struct __infoObject
{
	int type_object;
	int count_point;
};

bool MapViewer::ReadFileMap(QString file_name, MapObjects *_map_obj)
{
	QFile m_file(file_name);

	if (!m_file.open(QIODevice::ReadOnly))
    {
        qDebug() << m_file.errorString();
		return false;
    }

	QDataStream stream(&m_file);
    double offset_x = 0;
    double offset_y = 0;
	int count_point = -1;
    char* temp = (char*)malloc(sizeof(_points));
	stream.readRawData(temp, sizeof(_points));
	_points* offsets = (_points*)temp;
	offset_x = offsets->x;
    offset_y = offsets->y;
	while(!stream.atEnd())
    {
		stream.readRawData(temp, sizeof(__infoObject));
		__infoObject* read_result = (__infoObject*)temp;
		count_point = read_result->count_point;

        MapObjectContur map_ob;
        map_ob.type_obj = read_result->type_object;
		
		if(count_point>0)
        {
			for (int i = 0; i < count_point; i++)
			{
				stream.readRawData(temp, sizeof(_points));
				_points* point = (_points*)temp;
                map_ob.addPoint(point->x/*-offset_y*/, point->y/*-(_x-offset_x)*/);
			}
			if(map_ob.type_obj == 7)
			{
				char* _t = (char*)malloc(32);
				stream.readRawData(_t, 32);
				map_ob.text = QString::fromLocal8Bit(_t);
				free(_t);
			}
			_map_obj->addObject(&map_ob);
        }
	}
	free(temp);
	return true;
}

void MapViewer::keyPressEvent ( QKeyEvent * event )
{
	if(event->type() == QEvent::KeyPress)
    {
		if (event->key() == Qt::Key_Control) {
			if(select_position)
				ui.graphicsView->viewport()->setCursor(Qt::CrossCursor);
			MapViewer::contol_key = true;
		}

		if(event->key() == Qt::Key_Shift) {
			MapViewer::shift_key = true;
		}
	}
}

void MapViewer::keyReleaseEvent ( QKeyEvent * event )
{
	if(event->type() == QEvent::KeyRelease)
    {
		if (event->key() == Qt::Key_Control) {
			MapViewer::contol_key = false;
		}

		if(event->key() == Qt::Key_Shift) {
			MapViewer::shift_key = false;
		}
	}
}

