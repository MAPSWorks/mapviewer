#include "ClientConnection.h"

QMap<quint64, RectText*> RectText::mapRectText;

bool ClientConnection::doTrailItem = false;
bool ClientConnection::doPictures = true;

QVector<TrailItem*> ClientConnection::trailVec;

ClientConnection::ClientConnection()
{
	_pixmap = 0;
}

ClientConnection::ClientConnection(quint64 _id /* = -1 */, QString _name /* =  */, QPointF _position /* = QPointF */)
{	
	ellipse = 0;
	mac = _id;
	name = _name;
	position = _position;
	static_id = 0;
	mark_static_id = -1;
	fantom = 0;
	st_rt_pos = QPointF(5, 7);

    //_trailItem = 0;

	_pixmap = 0;
	id_path = 0;
}


ClientConnection::~ClientConnection()
{
	RectText::mapRectText.remove(rect_text->id);
}

ListConnection::ListConnection()
{

}

ListConnection::~ListConnection()
{

}

bool ListConnection::isConnected(quint64 id)
{
	if(map_connection[id])
		return true;
	else
		return false;
}

quint64 ListConnection::getIdConnection(QString name)
{
	typedef QMap<quint64, ClientConnection*>::iterator MI;
	for(MI iter = map_connection.begin(); iter != map_connection.end(); iter++)
	{
		ClientConnection* e = *iter;
		if(e->getNameConnection() == name)
			return e->getIdConnection();
	}
    return 0;
}

QString* ListConnection::getNameConnection(quint64 id) {
	typedef QMap<quint64, ClientConnection*>::iterator MI;
	for(MI iter = map_connection.begin(); iter != map_connection.end(); iter++)
	{
		ClientConnection* e = *iter;
		if(e)
		if(e->getIdConnection() == id)
			return e->getNameConnection();
	}
    QString* str = new QString("");
    return str;
}
QPointF ListConnection::getPosition(QString name) {
	typedef QMap<quint64, ClientConnection*>::iterator MI;
	for(MI iter = map_connection.begin(); iter != map_connection.end(); iter++)
	{
		ClientConnection* e = *iter;
		if(e->getNameConnection() == name)
			return e->getPosition();
	}
	return QPointF();
}
QPointF ListConnection::getPosition(quint64 id) {
	typedef QMap<quint64, ClientConnection*>::iterator MI;
	for(MI iter = map_connection.begin(); iter != map_connection.end(); iter++)
	{
		ClientConnection* e = *iter;
		if(e->getIdConnection() == id)
			return e->getPosition();
	}
	return QPointF();
}
void ListConnection::setPosition(quint64 id, QPointF pos) {
	typedef QMap<quint64, ClientConnection*>::iterator MI;
	for(MI iter = map_connection.begin(); iter != map_connection.end(); iter++)
	{
		ClientConnection* e = *iter;
		if(e->getIdConnection() == id)
		{
			e->setPosition(pos);
			return;
		}
	}
	return;
}
void ListConnection::setPosition(QString name, QPointF pos) {
	typedef QMap<quint64, ClientConnection*>::iterator MI;
	for(MI iter = map_connection.begin(); iter != map_connection.end(); iter++)
	{
		ClientConnection* e = *iter;
		if(e->getNameConnection() == name)
		{
			e->setPosition(pos);
			return;
		}
	}
	return;
}

QGraphicsEllipseItem* ListConnection::getEllipse(quint64 mac)
{
	return map_connection[mac]->getEllipse();
}

ClientConnection* ListConnection::getClientConnectionFromWidgetId(int wi)
{
	typedef QMap<quint64, ClientConnection*>::Iterator MI;
	MI n = map_connection.begin();
	int i = map_connection.count();
	for(int j = 0; j < i; j++)
	{
		ClientConnection* e = *n;
		if(e->widgetId == wi)
		{
			//e->setPosition(pos);
			return e;
		}
		n++;
	}
	return NULL;
}

TrailItem::~TrailItem()
{
	foreach(QAbstractGraphicsShapeItem* it,_items)
	{
		_scene->removeItem(it);
	}
	_items.clear();
    //conn->_trailItem=0;
}
