#include <Qt/qfile.h>
#include "MapObjectContur.h"
#include "LogMessage.h"

inline bool ReadFileMap(LogMessage* _log, QString file_name, MapObjects *_map_obj)
{
	_log->tracer(QString("Start read file_map.."));
	QFile m_file(file_name);

	if (!m_file.open(QIODevice::ReadOnly))
	{
		//::MessageBox(0, L"Error read file!", 0, 0);
		return false;
	}

	_log->tracer(QString("file open"));

	QDataStream stream(&m_file);
	double offset_x;
	double offset_y;
	int count_point;
	double *_doub = new double();
	int *_int = new int();
	char* temp = new char();
	int size_int = sizeof(int);
	int size_double = sizeof(double);

	_log->tracer(QString("init value - ok"));
	stream.readRawData(temp, size_double);
	_doub = (double*)temp;
	offset_x = *_doub;
	stream.readRawData(temp, size_double);
	_doub = (double*)temp;
	offset_y = *_doub;

	_log->tracer(QString("read params - ok"));

	int value_i = 0;
	while(!stream.atEnd())
	{
		_log->tracer(QString("read %1 circle").arg(value_i));
		//value_i++;
		int type_obj = 0;
		stream.readRawData(temp, size_int);
		_int = (int*)temp;
		type_obj = *_int;

		stream.readRawData(temp, size_int);
		_int = (int*)temp;
		count_point = *_int;

		MapObjectContur *map_ob = new MapObjectContur();
		map_ob->type_obj = type_obj;

		_log->tracer(QString("enter cycle"));
		for (int i = 0; i <count_point; i++)
		{
			qreal _x, _y;
			stream.readRawData(temp, size_double);
			_doub = (double*)temp;
			_x = *_doub;
			stream.readRawData(temp, size_double);
			_doub = (double*)temp;
			_y = *_doub;

			//QPointF *point = new QPointF(-(_x-offset_x), _y-offset_y);
			map_ob->addPoint(_x/*-offset_y*/, _y/*-(_x-offset_x)*/);
			_log->tracer(QString("point add %1").arg(value_i));
		}
		_map_obj->addObject(map_ob);
		_log->tracer(QString("object add %1").arg(value_i));
		value_i++;
	}
	//::MessageBox(0, L"End ReadFile", 0, 0);
	return true;
}
