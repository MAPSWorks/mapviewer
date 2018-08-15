#include "messagehandler.h"

#include "sphere_messages.h"

#include "mapviewer.h"

#include <QtNetwork/QTcpSocket>
#include <QTextCodec>

typedef QString(*IC)(QString, QString);
IC ic = &InsertColor;
#define SP(p, t) StringColorParser::parseString(p, t)
#define S_N(p) QString::number(p)

MessageHandler::MessageHandler(QString _path_app, LogMessage* log_m, QObject * parent, QTcpSocket * s, bool auto_r)
    : QObject(parent)
    , sock(s)
{
	path_app = _path_app;
    reset();
	auto_repeat = auto_r;
	count_picture = 0;
    if (sock)
    {
		qDebug() << sock->readBufferSize();
		tip = -1;
		_log = log_m;
		_bufferMes = new MessageBuffer(_log, auto_repeat);
        sock->setParent(this);
        connect(sock, SIGNAL(disconnected()), this, SLOT(disconnected()));
        connect(sock, SIGNAL(readyRead()), this, SLOT(readyRead()));
		connect(_bufferMes, SIGNAL(sendMessagePack(QByteArray)), this, SLOT(sendPack(QByteArray)));
		//point_item = new QGraphicsEllipseItem();
		create = false;
		comp = false;
    }

// 	sphere::Initialize();
// 
// 	TMyReceiver receiver;
// 
// 	sphere::CreatePort("portIn", sphere::EV_MessageReceiver(receiver, &TMyReceiver::ReceiveCallback));
// 	sphere::CreatePort("portOut");
}

MessageHandler::~MessageHandler()
{
    sock->deleteLater();
    sock = 0;
	if(_bufferMes)
		_bufferMes->close();
}

void MessageHandler::close_slot()
{
	if(_bufferMes)
		_bufferMes->close();
}

void MessageHandler::disconnected()
{
	sock->deleteLater();
	sock = 0;
	emit disconnected(this);
}
void MessageHandler::readyRead()
{
	emit readyRead(this);
}
void MessageHandler::onCreate(quint64 mac)
{
	//emit onCreate(this, mac);
}
void MessageHandler::sendMessage(const char * p, int sz, int rep)
{
    if (sock)
    {
		HEADER * phdr = (HEADER *)p;
		MessageStatus *m_s = new MessageStatus;
		m_s->mac = phdr->mac;
        m_s->id_message = phdr->tip;

        QByteArray block;
        QDataStream out(&block, QIODevice::WriteOnly);
        out.setVersion(QDataStream::Qt_4_6);

        /// qDebug() << "send size: " << sz;
		
        out << (quint32)sz;
        out.writeRawData(p, (quint32)sz);

        sock->write(block);

		m_s->size_message = (quint32)sz;
		m_s->buffer = block;
		m_s->time_mark = QTime::currentTime();

		if(rep)
		_bufferMes->addMessage(m_s->id_message, m_s);

		//QString str = "Send size: ["+ sz;
		//str +=  "]\nWritten block size: [" + block.size();
		//str += "]";//).arg(sz).arg(block.size());
		//_log->tracer(str);
        /// qDebug() << "Written block size: " << block.size();
    }
}

void MessageHandler::sendPack(QByteArray pack)
{
	if(sock)
	{
		sock->write(pack);
	}
};

bool MessageHandler::recvMessage(QByteArray & m)
{
	//if(!create)
	//emit onCreate(this, point_item);
    if (sock)
    {
        QDataStream in(sock);
        in.setVersion(QDataStream::Qt_4_6);

        qDebug() << "Bytes Available on socket: " << sock->bytesAvailable();
		/*QString str1 = tr("Bytes Available on socket: [%1]").arg(sock->bytesAvailable());
		_log->tracer(str1);*/
		//quint64 _s = sock->bytesAvailable();
        if (size == 0)
        {
            if (sock->bytesAvailable() < sizeof(size))
                    return false;
            in >> size;
        }

        qDebug() << "Got size: " << size;
		/*QString str2 = tr("Got size: [%1]").arg(size);
		_log->tracer(str2);*/

        // We need more data?
        if (sock->bytesAvailable() < size)
            return false;

        m.resize(size);
        in.readRawData(m.data(), size);

        qDebug() << "Bytes Available after read: " << sock->bytesAvailable();
		/*QString str3 = tr("Bytes Available after read: [%1]").arg(sock->bytesAvailable());
		_log->tracer(str3);*/
        // Reset
        reset();

        return true;
    }

    return false;
}

void MessageHandler::sendMessage_addPoint(quint64 mac,QPointF point, QString name, int id_point, int id_path)
{
	//QByteArray ba;
	HEADER hr;
	hr.mac = mac;
	hr.id = COMMAND1_ID;
	hr.tip = ++tip;
	COMMAND1 com1;
	com1.hdr = hr;
	//QPointF pos = recoderToGrad(point);
	com1.cord.x = point.x();
	com1.cord.y = point.y();
	com1.cord.z = 0;
	//quint16 params[8];// = quint16[];
	//params[0] = 0;
	//params[1] = 0;
	com1.params[0] = 1;//params;
	com1.params[1] = 0;
	com1.params[2] = id_point;
	com1.params[3] = id_path;
	//char* text = new char[96];
	//qstrncpy(text, name.data(), sizeof(name));
	qstrncpy(com1.text, name.toUtf8().data(), name.size()+1);
	QString str = ""; //tr(" [ID=%1] Send message add point: mac: [%2] id: [%3] x: [%4] y: [%5] z: [%6] params[0]=[%7] params[1]=[%8] params[2]=[%9] text: [%10]")
		//.arg(hr.tip).arg(hr.mac).arg(hr.id).arg(com1.cord.x).arg(com1.cord.y).arg(com1.cord.z).arg(com1.params[0]).arg(com1.params[1]).arg(com1.params[2]).arg(name);
	str	+=	ic(" [ID=", COLOR_TEXT_BLACK)	+	ic(S_N(hr.tip), COLOR_TEXT_RED);
	str	+=	ic("] Send message add point: mac: [", COLOR_TEXT_BLACK)	+	ic(S_N(hr.mac), COLOR_TEXT_RED);
	str	+=	ic("] id: [", COLOR_TEXT_BLACK)	+	ic(S_N(hr.id), COLOR_TEXT_BLUE);
	str	+=	ic("] x: [", COLOR_TEXT_BLACK)	+	ic(S_N(com1.cord.x), COLOR_TEXT_GREEN);
	str	+=	ic("] y: [", COLOR_TEXT_BLACK)	+	ic(S_N(com1.cord.y), COLOR_TEXT_GREEN);
	str	+=	ic("] z: [", COLOR_TEXT_BLACK)	+	ic(S_N(com1.cord.z), COLOR_TEXT_GREEN);
	str	+=	ic("] params[0]=[", COLOR_TEXT_BLACK)	+	ic(S_N(com1.params[0]), COLOR_TEXT_ORANGE);
	str	+=	ic("] params[1]=[", COLOR_TEXT_BLACK)	+	ic(S_N(com1.params[1]), COLOR_TEXT_ORANGE);
	str	+=	ic("] params[2]=[", COLOR_TEXT_BLACK)	+	ic(S_N(com1.params[2]), COLOR_TEXT_ORANGE);
	str	+=	ic("] text: [", COLOR_TEXT_BLACK)	+	ic(name, COLOR_TEXT_BLUE);
	str	+=	ic("]", COLOR_TEXT_BLACK);
	_log->tracer(str);
	sendMessage((const char*)&com1, sizeof(com1));
}

void MessageHandler::sendMessage_addMark(quint64 mac, QPointF point, QString name, Point_tab* point_sel, int id_mark)
{
	HEADER hr;
	hr.mac = mac;
	hr.id = COMMAND1_ID;
	hr.tip = ++tip;
	COMMAND1 com1;
	com1.hdr = hr;
	QPointF pos;
	if(!howRecord::useSK42)
		pos = recoderToGrad(point);
	else
		pos = recoderSKToWGS(point);
	com1.cord.x = pos.x();
	com1.cord.y = pos.y();
	com1.cord.z = 0;
	//quint16 params[8];// = new quint16[8];
	com1.params[0] = 2;
	
	com1.params[1] = point_sel->id_point;
	com1.params[2] = id_mark;
	com1.params[3] = point_sel->id_path;
	//com1.params = params;
	qstrncpy(com1.text, name.toUtf8().data(), name.size()+1);
	QString str = "";//tr(" [ID=%1] Send message add mark: mac: [%2] id: [%3] x: [%4] y: [%5] z: [%6] params[0]=[%7] params[1]=[%8] params[2]=[%9] text: [%10]")
		//.arg(hr.tip).arg(hr.mac).arg(hr.id).arg(com1.cord.x).arg(com1.cord.y).arg(com1.cord.z).arg(com1.params[0]).arg(com1.params[1]).arg(com1.params[2]).arg(name);
	str	+=	ic(" [ID=", COLOR_TEXT_BLACK)	+	ic(S_N(hr.tip), COLOR_TEXT_RED);
	str	+=	ic("] Send message add mark: mac: [", COLOR_TEXT_BLACK)	+	ic(S_N(hr.mac), COLOR_TEXT_RED);
	str	+=	ic("] id: [", COLOR_TEXT_BLACK)	+	ic(S_N(hr.id), COLOR_TEXT_BLUE);
	str	+=	ic("] x: [", COLOR_TEXT_BLACK)	+	ic(S_N(com1.cord.x), COLOR_TEXT_GREEN);
	str	+=	ic("] y: [", COLOR_TEXT_BLACK)	+	ic(S_N(com1.cord.y), COLOR_TEXT_GREEN);
	str	+=	ic("] z: [", COLOR_TEXT_BLACK)	+	ic(S_N(com1.cord.z), COLOR_TEXT_GREEN);
	str	+=	ic("] params[0]=[", COLOR_TEXT_BLACK)	+	ic(S_N(com1.params[0]), COLOR_TEXT_ORANGE);
	str	+=	ic("] params[1]=[", COLOR_TEXT_BLACK)	+	ic(S_N(com1.params[1]), COLOR_TEXT_ORANGE);
	str	+=	ic("] params[2]=[", COLOR_TEXT_BLACK)	+	ic(S_N(com1.params[2]), COLOR_TEXT_ORANGE);
	str	+=	ic("] text: [", COLOR_TEXT_BLACK)	+	ic(name, COLOR_TEXT_BLUE);
	str	+=	ic("]", COLOR_TEXT_BLACK);
	_log->tracer(str);
	sendMessage((const char*)&com1, sizeof(com1));
}

void MessageHandler::sendMessage_emulate(quint64 mac, EmulData data)
{
	HEADER hr;
	hr.mac = mac;
	hr.id = DATAEMULATE_T;
	hr.tip = ++tip;
	QTextCodec *codec = QTextCodec::codecForName("KOI8-R");
	char* str = (char*)malloc(16);
	QByteArray ba2(codec->fromUnicode(data.name));
	qstrncpy(str, ba2.data(), 16);
	DataEmulateTable dt;
	dt.hdr = hr;
	qstrncpy(dt.name, str, 16);
	dt.pos.x = data.position.x();
	dt.pos.y = data.position.y();
	dt.pos.z = 0;
	dt.dirspd.x = data.speed;
	dt.dirspd.y = data.direction;
	sendMessage((const char*)&dt, sizeof(dt));
}

void MessageHandler::sendMessage_deleteEmul(quint64 mac)
{
	HEADER hr;
	hr.mac = mac;
	hr.id = DELETEEMUL_T;
	hr.tip = ++tip;
	sendMessage((const char*)&hr, sizeof(hr));
}

void MessageHandler::sendPointToComp(quint16 mac, QPointF point, QString name, quint16 id, int params)
{
	HEADER hr;
	hr.mac = mac;
	hr.id = COMMAND1_ID;
	hr.tip = ++tip;
	COMMAND1 com1;
	com1.hdr = hr;
	//QPointF pos = recoderToGrad(point);
	com1.cord.x = point.x();
	com1.cord.y = point.y();
	com1.cord.z = 0;
	switch(params)
	{
	case 1:
		com1.params[0] = 1;
		break;
	case 2:
		com1.params[0] = 2;
		break;
	case 4:
		com1.params[0] = 4;
		break;
	default:
		break;
	}
	com1.params[1] = id;
	com1.params[2] = id;
	//com1.params = params;
	qstrncpy(com1.text, name.toUtf8().data(), name.size()+1);
	sendMessage((const char*)&com1, sizeof(com1), 0);
}

void MessageHandler::sendMessage_chat(quint64 mac, char* mess)
{
	HEADER hr;
	hr.mac = mac;
	hr.id = TEXTMSG_ID;
	hr.tip = ++tip;
	TEXTMSG mes;
	mes.hdr = hr;
	//mes.text = mess;
	qstrncpy(mes.text, mess, 128);
    //QString str = tr(" [ID=%1] Send text: mac: [%2] id: [%3] text: [%4]").arg(hr.tip).arg(hr.mac).arg(hr.id).arg(QString::fromLocal8Bit(mess, 128));
    sendMessage((const char*)&mes, sizeof(mes));
    //_log->tracer("send message ok!");

// 	TEXTMSG_t var;
// 	var.hdr.id=hr.id;
// 	var.hdr.mac=hr.mac;
// 	var.hdr.tip=hr.tip;
// 	qstrncpy(var.text, mess, 128);
// 
// 	msg::TDataSetPtr dataSet = msg::TDataSet::ConstructorWrite(MT_ID(TEXTMSG_t));
// 	var.Write(dataSet);
// 
// 	char dst[256];
// 	sprintf(dst,"hdr: id=%ui, mac=%ll, tip=%ui \n",var.hdr.id,var.hdr.mac,var.hdr.tip );
// 	OutputDebugStringA(dst);
// 	sprintf(dst,"text  %s\n",var.text);
// 	OutputDebugStringA(dst);
// 	sprintf(dst,"message sended!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!\n");
// 	OutputDebugStringA(dst);
// 	sphere::Send("portOut", dataSet);
}

void MessageHandler::messageToGui(quint64 mac, QString text)
{
	QString *name = tablets.getNameConnection(mac);
	emit messageToGuiSignal(this, mac, *name, text);
}

void MessageHandler::deleteMessageFromBuffer(quint16 _tip)
{
	emit confirm_message(_tip);
	_bufferMes->deleteMessage(_tip);
}

void MessageHandler::repeatMessage(quint16 id)
{
	_bufferMes->repeatMessage(id);
}

void MessageHandler::dalnomerMark(QPointF pos, QString name, int size, quint16 _type)
{
	emit dalnomerMarkSignal(pos, name, size, _type);
}
bool ListClients::ifConnected(QTcpSocket * soc)
{
	typedef QList<MessageHandler*>::iterator LI;
	for(LI iter = list_mes_handlers.begin(); iter != list_mes_handlers.end(); iter++)
	{
		MessageHandler* e = *iter;
		if(e->getSock() == soc)
			return true;
	}

	return false;
}

union un
{
	quint32 _size32;
	quint16 _size16[2];
};

void DebugMessage(const QByteArray & ba, MessageHandler* mes)
{
    HEADER * phdr = (HEADER *)ba.data();
    //bool _add = false;

    //qDebug("Got message from device: %016llx", phdr->mac);
	//mes->_log->tracer(MessageHandler::tr("Got message from device: %1").arg(phdr->mac));
	if(!mes->tablets.isConnected(phdr->mac))
	{
		mes->tablets.addConnection(phdr->mac);
		emit mes->onCreate(phdr->mac);
	}
	else
		qDebug() << "Conn yes";

    switch (phdr->id)
    {
        case TELEMETRY_ID:
        {
            qDebug() << "id = TELEMETRY_ID";
            TELEMETRY * pmsg = (TELEMETRY *)ba.data();

            QTextCodec *codec = QTextCodec::codecForName("KOI8-R");

			if(*mes->tablets.getNameConnection(phdr->mac) == "NO_NAME" && pmsg->state != (uchar)0)
			{
				mes->tablets.setName(phdr->mac, codec->toUnicode(pmsg->name));
				mes->change_name(phdr->mac, codec->toUnicode(pmsg->name));
			}
			//emit mes->onCreate(phdr->mac);

            qDebug() << "name: " << codec->toUnicode(pmsg->name);
			qDebug() << "state: " << pmsg->state;

            if (pmsg->state == (uchar)1)
            {
                qDebug() << "coords: " << pmsg->pos.x << " " << pmsg->pos.y << " " << pmsg->pos.z;
				QPointF position(pmsg->pos.x, pmsg->pos.y);
				if(!howRecord::useSK42)
					position = recoder(position);
				else
					position = recoderWGSToSK(position);
				QByteArray b = pmsg->name;
				emit mes->signal_pos(phdr->mac, position, b);
				mes->change_state(phdr->mac, pmsg->state);
				/*mes->_log->tracer(MessageHandler::tr("id: [TELEMETRY_ID] name: [%1] state: [%2] x: [%3] y: [%4] z: [%5] dir: [%6] speed: [%7]").arg(codec->toUnicode(pmsg->name)).arg(pmsg->state)
					.arg(pmsg->pos.x).arg(pmsg->pos.y).arg(pmsg->pos.z).arg(pmsg->dirspd.x).arg(pmsg->dirspd.y));*/
            }
			else if(pmsg->state == (uchar)0)
			{
				mes->change_state(phdr->mac, pmsg->state);
			}
			else if(pmsg->state == (uchar)2)
			{
				mes->change_state(phdr->mac, pmsg->state);
			}

            qDebug() << "dir: " << pmsg->dirspd.x << " speed: " << pmsg->dirspd.y;
			//mes->_log->tracer(MessageHandler::tr("id: [TELEMETRY_ID] name: [%1] state: [%2] x: [%3] y: [%4] z: [%5] dir: [%6] speed: [%7]").arg(codec->toUnicode(pmsg->name)).arg(pmsg->state)
			//	.arg(pmsg->pos.x).arg(pmsg->pos.y).arg(pmsg->pos.z).arg(pmsg->dirspd.x).arg(pmsg->dirspd.y));
			QString str = "";
			str	+=	ic("id: [", COLOR_TEXT_BLACK)	+	ic("TELEMETRY_ID", COLOR_TEXT_PINK);
			str	+=	ic("] name: [", COLOR_TEXT_BLACK)	+	ic(codec->toUnicode(pmsg->name), COLOR_TEXT_BLUE);
			str	+=	ic("] state: [", COLOR_TEXT_BLACK)	+	ic(S_N(pmsg->state), COLOR_TEXT_RED);
			str	+=	ic("] x: [", COLOR_TEXT_BLACK)	+	ic(S_N(pmsg->pos.x), COLOR_TEXT_GREEN);
			str	+=	ic("] y: [", COLOR_TEXT_BLACK)	+	ic(S_N(pmsg->pos.y), COLOR_TEXT_GREEN);
			str	+=	ic("] z: [", COLOR_TEXT_BLACK)	+	ic(S_N(pmsg->pos.z), COLOR_TEXT_GREEN);
			str	+=	ic("] dir: [", COLOR_TEXT_BLACK)	+	ic(S_N(pmsg->dirspd.x), COLOR_TEXT_ORANGE);
			str	+=	ic("] speed: [", COLOR_TEXT_BLACK)	+	ic(S_N(pmsg->dirspd.y), COLOR_TEXT_ORANGE);
			str	+=	ic("]", COLOR_TEXT_BLACK);
			mes->_log->tracer(str);
			mes->sendTo3DComp(pmsg);
        }
        break;

        case CONNLOST_ID:
			mes->_log->tracer(QString(InsertColor("id = CONNLOST_ID  Lost connection with device [", COLOR_TEXT_BLACK) + InsertColor(QString::number(phdr->mac), COLOR_TEXT_BLUE) + InsertColor("]", COLOR_TEXT_BLACK)));
        break;

        case TEXTMSG_ID:
        {
            qDebug() << "id = TEXTMSG_ID";
            TEXTMSG * pmsg = (TEXTMSG *)ba.data();

            QTextCodec *codec = QTextCodec::codecForName("KOI8-R");
            qDebug() << "text: " << codec->toUnicode(pmsg->text);
			mes->messageToGui(pmsg->hdr.mac, codec->toUnicode(pmsg->text));

        }
        break;

        case COMMAND1_ID:
			{
				qDebug() << "id = COMMAND1_ID";
				
				COMMAND1* com1 = (COMMAND1*)ba.data();
				if(com1->params[0] == 4)
                {
                    QPointF pos_m(com1->cord.x, com1->cord.y);
					QTextCodec *codec = QTextCodec::codecForName("KOI8-R");
					QString name(codec->toUnicode(com1->text));
                    mes->dalnomerMark(pos_m, name, sizeof(com1->text), com1->params[2]);
				}
				if(com1->params[0] == 3)
				{
					QString str("Read image");
					mes->_log->tracer(InsertColor(str, COLOR_TEXT_BLACK));
					count_picture++;
					union un def;
					def._size16[0]=com1->params[1];
					def._size16[1]=com1->params[2];
					quint32 _size = def._size32;
//					QTextCodec *codec = QTextCodec::codecForName("KOI8-R");
//					QString name_text(codec->toUnicode(com1->text));
					QString mac = QString::number(com1->hdr.mac, 'f', 0)+"_";
					QString name = mac+QString::number(count_picture);
					QPointF pos;
					pos.setX(com1->cord.x);
					pos.setY(com1->cord.y);
                    QString filename = mes->path_app + "_Picture/" + name + ".jpg";
					//QFile _file;
					//_file.setFileName(filename);
					//bool y = _file.isOpen();
					//if (!_file.open(QIODevice::ReadWrite))
					//{
					//	mes->_log->tracer(_file.errorString());
					//	return;
					//}
					int m_s = ba.size();
					//char* temp = (char*)malloc(ba.size());
					//qstrncpy(temp, ba.data(), m_s);
					//char* n_temp = (char*)malloc(_size);
					QByteArray n_temp;
					int j = 0;
                    //int size_com1 = sizeof(com1);
					for(int i = m_s-_size; i<m_s; i++)
					{
						n_temp[j]=ba.at(i);
						//qDebug() << i << '\t' << ba.at(i);
						j++;
					}//*/
					QImage *image = new QImage;
                    image->loadFromData(n_temp,"JPG");
					if(image != 0)
					{
						mes->picture_input(com1->hdr.mac, image, name, filename, pos);
						mes->_log->tracer(image, name);
                        qDebug() << image->save(filename, "JPG");
					}
					else
					{
						mes->_log->tracer(InsertColor("Error read image!", COLOR_TEXT_RED));
					}
					
					//QDataStream stream(&_file);
					//stream.writeRawData(n_temp, _size);
					//_file.write(n_temp, _size);
					mes->_log->tracer(filename);
					//mes->readPicture(pos, _file.fileName(), name_text, com1->hdr.mac);
					//_file.close();
				}
			}
        break;

        case COMMAND2_ID:
            qDebug() << "id = COMMAND2_ID";
        break;

        case COMMAND3_ID:
            qDebug() << "id = COMMAND3_ID";
        break;

		case MSGSTATUS_ID:
			{
				MSGSTATUS* msgst = (MSGSTATUS*)ba.data();
				if(msgst->status == 0) // доставлено
				{
					mes->_log->tracer(MessageHandler::tr("Confirm message: id: [%1], status: [%2]").arg(msgst->hdr.tip).arg(msgst->status));
					mes->deleteMessageFromBuffer(msgst->hdr.tip);
				}
				else// 1 - не доставлено
				{
					mes->repeatMessage(msgst->hdr.tip);
				}
			}
		break;

		case IDENTCOMP_ID:
			{
				mes->comp = true;
				mes->hidePoint(phdr->mac);
			}
		break;


        default:
			{
				mes->_log->tracer(InsertColor("id = Unknown", COLOR_TEXT_RED));
				qDebug() << "id = Unknown";
			}
        break;
    }
}
