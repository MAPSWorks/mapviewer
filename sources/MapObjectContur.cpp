#include "MapObjectContur.h"
#include "mapviewer.h"

int MapObjectContur::counter_object = 0;

MapObjectContur::MapObjectContur()
{
	//text = (char*)malloc(32);
	id = counter_object;
	counter_object++;
}

MapObjectContur::MapObjectContur(QVector<QPointF> vec_p)
{
	//text = (char*)malloc(32);
	contur = vec_p;
	id = counter_object;
	counter_object++;
}

MapObjectContur::MapObjectContur(const MapObjectContur &map_obj)
{
	//text = (char*)malloc(32);
	//strcpy(text, map_obj.text);
	text = map_obj.text;
	type_obj = map_obj.type_obj;
	contur = map_obj.getContur();
	id = map_obj.getId();
}

MapObjectContur::MapObjectContur(MapObjectContur& src)
{
	//text = (char*)malloc(32);
	//strcpy(text, src.text);
	text = src.text;
	type_obj = src.type_obj;
	contur = src.getContur();
	id = src.getId();
}
MapObjectContur::MapObjectContur(const MapObjectContur *src)
{
	//text = (char*)malloc(32);
	//strcpy(text, src->text);
	text = src->text;
	type_obj = src->type_obj;
	contur = src->getContur();
	id = src->getId();
}

MapObjectContur::~MapObjectContur()
{
	//free(text);
}

MapObjectContur& MapObjectContur::operator=(const MapObjectContur& src)
{
	if (this == &src)
		return *this;
	type_obj = src.type_obj;
	contur = src.getContur();
	id = src.getId();
	return *this;
}

void MapObjectContur::addPoint(qreal x, qreal y)
{
	QPointF point(x, y);
	contur.push_back(point);
}

void MapObjectContur::addPoint(QPointF point)
{
	contur.push_back(point);
}
void MapObjectContur::addContur(QVector<QPointF> *vec_p)
{
	typedef QVector<QPointF>::iterator it;
	for(it iter = vec_p->begin(); iter != vec_p->end(); iter++)
	{
		QPointF* e = iter;
		contur.push_back(*e);
	}
}

MapObjects::MapObjects(const MapObjects &src)
{
	count_obj = src.getCountObjects();
	objects_map_contur = src.getConturs();
}
