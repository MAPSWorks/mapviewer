#ifndef _MAPOBJECTCONTUR_
#define _MAPOBJECTCONTUR_

#undef max
#undef min

#include <QtGui/QtGui>
#include <QtGui/qgraphicsscene.h>

class MapObjectContur;

typedef QVector<QPointF*> VectorPoint;
typedef QVector<MapObjectContur*> VectorMapContur;

class MapObjectContur
{
public:
	MapObjectContur();
	MapObjectContur(QVector<QPointF> vec_p);
	MapObjectContur(const MapObjectContur &map_obj);
	MapObjectContur(MapObjectContur& src);
	MapObjectContur(const MapObjectContur *src);

	MapObjectContur& operator=(const MapObjectContur& src);
	~MapObjectContur();
	static int counter_object;

	int type_obj;
	
	QString text;

	void addPoint(qreal x, qreal y);
	void addPoint(QPointF point);
	void addContur(QVector<QPointF> *vec_p);

	QVector<QPointF> getContur() const
	{
		return contur;
	}
	int getId() const
	{
		return id;
	}

private:
	QVector<QPointF> contur;
	int id;
};

class MapObjects
{
	int count_obj;
public:
    MapObjects(){ count_obj = -1; }
	MapObjects(const MapObjects &src);
    ~MapObjects(){}
	
	void addObject(MapObjectContur obj)
	{
		objects_map_contur.push_back(obj);
		count_obj++;
    }

	MapObjectContur getObject(int number) const
	{
		return objects_map_contur.at(number);
    }

	void deleteObject(int number)
	{
		objects_map_contur.remove(number);
		count_obj--;
    }

	int getCount() const
	{
		return objects_map_contur.size();
    }

    int getCountObjects() const { return count_obj; }

    QVector<MapObjectContur> getConturs() const { return objects_map_contur; }

private:
	QVector<MapObjectContur> objects_map_contur;
};
#endif //_MAPOBJECTCONTUR
