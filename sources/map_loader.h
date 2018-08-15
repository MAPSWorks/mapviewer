//#ifndef _MAPLOADER_
//#define _MAPLOADER_

#include "Includes/Mapapi.h"
#include "Includes/Maptype.h"
#include "Includes/Seekapi.h"
#include "Includes/Mapcreat.h"

#include <QtGui/QtGui>

HMAP myMap;

inline void readMap()
{
	const char* mFileMap = "Moscow_map/Moscow_2.sit";
	char* rsc = new char[250];
	mapGetRscByName(mFileMap, rsc, 250);
	CHAR* name_dll = "gisacces.dll";
	HINSTANCE libinst = ::LoadLibraryA(name_dll);
	myMap = ::mapOpenData(mFileMap, GENERIC_READ);
	int count_objects = ::mapTotalSeekObjectCount(myMap);
	HSELECT inSelect = ::mapCreateMapSelectContext(myMap);
	int temp;
	temp = mapClearSelectContext(inSelect);
	temp = mapIsSelectActive(inSelect);
	int new_count = mapSeekSelectObjectCount(myMap, inSelect);

	HOBJ hObj = mapCreateObject(myMap,1,IDDOUBLE3,0);
	int scale = mapGetMapScale(myMap);

	//int qa = mapGetCurrentPointFormat(myMap);

	mapSetMapPrecision(myMap);

	int z = mapGetMapPrecision(myMap);

	MAPREGISTEREX *_m = new MAPREGISTEREX();
	DATUMPARAM *_p = new DATUMPARAM();
	ELLIPSOIDPARAM *_e = new ELLIPSOIDPARAM();
	mapGetDocProjection(myMap,
		_m,
		_p,
		_e);

	MAPREGISTER *m_r = new MAPREGISTER();
	LISTREGISTER *l_r = new LISTREGISTER();
	int rez = mapGetMapInfoByName(mFileMap,
		0,
		m_r,
		l_r);
	qDebug() << "OK";
}

inline QPointF recoder(QPointF pos)
{
	GEODEGREE *coord_x = new GEODEGREE();
	coord_x->Degree = pos.x();
	double temp_min = (pos.x() - floor(pos.x()));
	temp_min = temp_min*60;
	coord_x->Minute = temp_min;
	double temp_sec = (temp_min - floor(temp_min))*60;
	coord_x->Second = temp_sec;//(temp_sec - floor(temp_sec))/60;
	GEODEGREE *coord_y = new GEODEGREE();
	coord_y->Degree = pos.y();
	double temp_min2 = (pos.y() - floor(pos.y()));
	temp_min2 = temp_min2*60;
	coord_y->Minute = temp_min2;
	double temp_sec2 = (temp_min2 - floor(temp_min2))*60;
	coord_y->Second = temp_sec2;
	 double* _x = new double();
	 double* _y = new double();
	 mapDegreeToRadian(coord_x, _x);
	 mapDegreeToRadian(coord_y, _y);
	 mapGeoToPlane(myMap, _x, _y);
	 return QPointF(*_x, *_y);

}

//#endif
