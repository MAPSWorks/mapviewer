#ifndef _CLIENTCONNECTIONS_
#define _CLIENTCONNECTIONS_

#include <QtGui/QtGui>
#include <QtGui/QtEvents>

extern "C"
{
#include <proj_api.h>
}

struct howRecord
{
	static bool useSK42;
};

/*struct howRender
{
	static double x_max;
	static double y_max;
	static double x_min;
	static double y_min;
};*/

#define max(a, b)(((a)>(b)) ? (a):(b))

struct degree_minute_sec
{
	degree_minute_sec(QPointF pos)
	{
		//int gradx, minutex; float secx;
		//int grady, minutey; float secy;
		deg_x = (int)pos.x();
		minute_x = (pos.x() - deg_x) * 60;
		sec_x = ((pos.x()-deg_x)*60 - minute_x) * 60;
		deg_y = (int)pos.y();
		minute_y = (pos.y() - deg_y) * 60;
		sec_y = ((pos.y()-deg_y)*60 - minute_y) * 60;
	}
	int deg_x, deg_y;
	int minute_x, minute_y;
	float sec_x, sec_y;
};

inline QPointF recoder(QPointF pos)
{
	const QString GEOPROJ4 = "+proj=longlat +ellps=WGS84 +datum=WGS84 +no_defs";
	projPJ wgs84Proj = pj_init_plus( GEOPROJ4.toLocal8Bit().data() );
	const QString myProj = "+proj=utm +zone=37 +ellps=WGS84 +datum=WGS84 +units=m +no_defs";
	projPJ myProj4 = pj_init_plus(myProj.toLocal8Bit().data());
	double x, y, z;
	x = pos.x(); //0.97278017506624093;//6177561.7933134781;//55.736198425292969;
	y = pos.y(); //0.65688086683003122;//37414386.181224316;//37.636501312255859;
	z = 0;
	double northing = x * DEG_TO_RAD;
	double easthing = y * DEG_TO_RAD;
	int projResult = pj_transform( wgs84Proj, myProj4, 1, 0, &easthing, &northing, &z );
	easthing = easthing + 37000000;
	if(pj_strerrno( projResult ) < 0)
		return QPointF(0, 0);
	return QPointF(northing, easthing);
}

inline QPointF recoderSKToWGS(QPointF pos)
{
	const QString GEOPROJ4 = "+proj=longlat +ellps=WGS84 +datum=WGS84 +no_defs";
	projPJ wgs84Proj = pj_init_plus( GEOPROJ4.toLocal8Bit().data() );
	const QString myProj = "+proj=tmerc +lat_0=0 +lon_0=39 +k=1 +x_0=500000 +y_0=0 +ellps=krass +units=m +no_defs";//"+proj=latlong +ellps=krass +datum=WGS84 +units=m +no_defs";//"+proj=utm +zone=37 +ellps=WGS84 +datum=WGS84 +units=m +no_defs";
	projPJ myProj4 = pj_init_plus(myProj.toLocal8Bit().data());
	double x, y, z;
	x = pos.x(); //0.97278017506624093;//6177561.7933134781;//55.736198425292969;
	y = pos.y(); //0.65688086683003122;//37414386.181224316;//37.636501312255859;
	z = 0;
	double northing = x; // * DEG_TO_RAD;
	double easthing = y-7000000; // * DEG_TO_RAD;
	int projResult = pj_transform( myProj4, wgs84Proj, 1, 0, &easthing, &northing, &z );
	northing = northing * RAD_TO_DEG;
	easthing = easthing * RAD_TO_DEG;
	if(pj_strerrno( projResult ) < 0)
		return QPointF(0, 0);
	return QPointF(northing, easthing);
}

inline QPointF recoderWGSToSK(QPointF pos)
{
	const QString GEOPROJ4 = "+proj=longlat +ellps=WGS84 +datum=WGS84 +no_defs";
	projPJ wgs84Proj = pj_init_plus( GEOPROJ4.toLocal8Bit().data() );
	const QString myProj = "+proj=tmerc +lat_0=0 +lon_0=39 +k=1 +x_0=500000 +y_0=0 +ellps=krass +units=m +no_defs";//"+proj=latlong +ellps=krass +datum=WGS84 +units=m +no_defs";//"+proj=utm +zone=37 +ellps=WGS84 +datum=WGS84 +units=m +no_defs";
	projPJ myProj4 = pj_init_plus(myProj.toLocal8Bit().data());
	double x, y, z;
	x = pos.x(); //0.97278017506624093;//6177561.7933134781;//55.736198425292969;
	y = pos.y(); //0.65688086683003122;//37414386.181224316;//37.636501312255859;
	z = 0;
	double northing = x * DEG_TO_RAD;
	double easthing = y * DEG_TO_RAD;
	int projResult = pj_transform( wgs84Proj, myProj4, 1, 0, &easthing, &northing, &z );
	northing = northing;//* RAD_TO_DEG;
	easthing = easthing + 7000000;//* RAD_TO_DEG;
	if(pj_strerrno( projResult ) < 0)
		return QPointF(0, 0);
	return QPointF(northing, easthing);
}

inline QPointF recoderToGrad(QPointF pos)
{
	const QString GEOPROJ4 = "+proj=longlat +ellps=WGS84 +datum=WGS84 +no_defs";
	projPJ wgs84Proj = pj_init_plus( GEOPROJ4.toLocal8Bit().data() );
	const QString myProj = "+proj=utm +zone=37 +ellps=WGS84 +datum=WGS84 +units=m +no_defs";
	projPJ myProj4 = pj_init_plus(myProj.toLocal8Bit().data());
	double x, y, z;
	x = pos.x(); //0.97278017506624093;//6177561.7933134781;//55.736198425292969;
	y = pos.y(); //0.65688086683003122;//37414386.181224316;//37.636501312255859;
	z = 0;
	double northing = x; // * DEG_TO_RAD;
	double easthing = y - 37000000; // * DEG_TO_RAD;
	int projResult = pj_transform( myProj4, wgs84Proj, 1, 0, &easthing, &northing, &z );
	northing = northing * RAD_TO_DEG;
	easthing = easthing * RAD_TO_DEG;
	if(pj_strerrno( projResult ) < 0)
		return QPointF(0, 0);
	return QPointF(northing, easthing);
}

inline QPointF recoderMSKToSK42(QPointF pos)
{
	const QString GEOPROJ4 = "+proj=utm +zone=37 +ellps=WGS84 +datum=WGS84 +units=m +no_defs";
	projPJ wgs84Proj = pj_init_plus( GEOPROJ4.toLocal8Bit().data() );
	const QString myProj = "+proj=tmerc +lat_0=0 +lon_0=39 +k=1 +x_0=500000 +y_0=0 +ellps=krass +units=m +no_defs";
	projPJ myProj4 = pj_init_plus(myProj.toLocal8Bit().data());
	double x, y, z;
	x = pos.x(); //0.97278017506624093;//6177561.7933134781;//55.736198425292969;
	y = pos.y(); //0.65688086683003122;//37414386.181224316;//37.636501312255859;
	z = 0;
	double northing = x; // * DEG_TO_RAD;
	double easthing = y - 37000000; // * DEG_TO_RAD;
	int projResult = pj_transform( wgs84Proj, myProj4, 1, 0, &easthing, &northing, &z );
	//northing = northing * RAD_TO_DEG;
	easthing = easthing + 7000000;
	if(pj_strerrno( projResult ) < 0)
		return QPointF(0, 0);
	return QPointF(northing, easthing);
}

inline QPointF recoderSK42ToMSK(QPointF pos)
{
	const QString GEOPROJ4 = "+proj=tmerc +lat_0=0 +lon_0=39 +k=1 +x_0=500000 +y_0=0 +ellps=krass +units=m +no_defs";//+proj=utm +zone=37 +ellps=WGS84 +datum=WGS84 +units=m +no_defs";
	projPJ wgs84Proj = pj_init_plus( GEOPROJ4.toLocal8Bit().data() );
	const QString myProj = "+proj=utm +zone=37 +ellps=WGS84 +datum=WGS84 +units=m +no_defs";//+proj=tmerc +lat_0=0 +lon_0=39 +k=1 +x_0=500000 +y_0=0 +ellps=krass +units=m +no_defs";
	projPJ myProj4 = pj_init_plus(myProj.toLocal8Bit().data());
	double x, y, z;
	x = pos.x(); //0.97278017506624093;//6177561.7933134781;//55.736198425292969;
	y = pos.y(); //0.65688086683003122;//37414386.181224316;//37.636501312255859;
	z = 0;
	double northing = x; // * DEG_TO_RAD;
	double easthing = y-7000000/* + 37000000*/; // * DEG_TO_RAD;
	int projResult = pj_transform( wgs84Proj, myProj4, 1, 0, &easthing, &northing, &z );
	//northing = northing * RAD_TO_DEG;
	easthing = easthing + 37000000;
	if(pj_strerrno( projResult ) < 0)
		return QPointF(0, 0);
	return QPointF(northing, easthing);
}

static quint64 forRectText = 0;

class RectText : public QGraphicsRectItem
{
public:
	RectText(QRectF &_rect, QGraphicsItem* _parent, QGraphicsScene* _scene, QString head="No Head", QString body="No body")
		: QGraphicsRectItem(_rect, _parent, _scene)
	{
		text_header = 0;
		text_body = 0;
        head = "";
        body = "";
		id = forRectText++;
	}

	static QMap<quint64, RectText*> mapRectText;

	enum { Type = 102 };
    int type() const {
        return Type;
    }
	void setTextForHeader(QString _str)
	{
		text_header->setPlainText(_str);
	}
	void setTextForBody(QString _str)
	{
		text_body->setPlainText(_str);
	}
	void installText(QString _head, QString _body, QFont _font1, QFont _font2, QGraphicsScene* _scene, QPointF offset1, QPointF offset2)
	{
		if(!text_header)
			text_header = new QGraphicsTextItem(_head, this, _scene);
		if(!text_body)
			text_body = new QGraphicsTextItem(_body, this, _scene);
		//qDebug() << pos();
		text_header->setPos(offset1);
		text_body->setPos(offset2);
		text_header->setFont(_font1);
		text_body->setFont(_font2);
		QRectF r1 = text_header->boundingRect();
		QRectF r2 = text_body->boundingRect();
		QRectF r(0, 0, max(r1.width(), r2.width()), max(r1.height(), r2.height()));
		setRect(r);
	}
	QGraphicsTextItem* getTextHeaderItem()
	{
		return text_header;
	}
	QGraphicsTextItem* getTextBodyItem()
	{
		return text_body;
	}
	void setPosition(QPointF _pos)
	{
		setPos(_pos);
		verifyCollidins();
	}
	void verifyCollidins()
	{
		foreach(RectText* obj, mapRectText)
		{
			if(obj != this)
			if(collidesWithItem(obj))
			{
				QRectF r = boundingRect();
				QPointF _t2 = obj->pos();
                //QPointF _temp = mapToScene(_t2);
				QPointF p = mapToItem(this, _t2);
				QPointF t_p = pos();
				setPosition(QPointF(p.x(), t_p.y()-r.height()));
			}
		}
	}
	quint64 id;
private:
	QGraphicsTextItem* text_header;
	QGraphicsTextItem* text_body;
};

class Mark_tab
{
public:
    Mark_tab(){}
    ~Mark_tab(){}
	QGraphicsEllipseItem* this_mark;
	QGraphicsLineItem* line_mark;
	int id_mark;
	quint16 id_message_mark;
	QString ToString()
	{
		return QString("%1 mark").arg(id_mark);
	}
};
class Point_tab
{
public:
    Point_tab(){static_id_mark = 0; line_point = 0;}
    ~Point_tab(){}
	QGraphicsEllipseItem* this_point;
	QVector<Mark_tab*> marks;
	QGraphicsLineItem* line_point;
	int static_id_mark;
	int id_point;
	quint16 id_message_point;
	int id_path;
	QString ToString()
	{
		return QString("%1").arg(id_point) + QString(" point");
	}
};

class Path_point
{
public:
	Path_point(){id_point = 0;}
	~Path_point(){}
	int id_point;
	int id;
	QStringList toListString()
	{
		QStringList str;
		str.append("Path");
		str.append(QString::number(id));
		return str;
	}
	QVector<Point_tab*> points;
};

class Fantom : public QGraphicsEllipseItem
{
public:
	Fantom(QGraphicsItem *_parent = 0, QGraphicsScene *scene = 0) : QGraphicsEllipseItem(_parent, scene){};
	Fantom(QGraphicsEllipseItem *client_obj, qreal x, qreal y, qreal _w, qreal _h, QString _text, QGraphicsItem *parent_obj = 0, QGraphicsScene *scene = 0)
		: QGraphicsEllipseItem(-_w/2, -_h/2, _w, _h, parent_obj, scene) {
			setPos(x, y);
			_objType="30-мм автоматический гранатомет АГС-17 'Пламя'";
			w=_w; h=_h;
			opacity_fantom = new QGraphicsOpacityEffect();
			opacity_fantom->setOpacity(0.5);
			setGraphicsEffect(opacity_fantom);
			QBrush brush(Qt::red, Qt::SolidPattern);
			setBrush(brush);
			parent = client_obj;
			QPointF point_parent = client_obj->pos();
			line = new QGraphicsLineItem(point_parent.x(), point_parent.y(), x+w/2, y+w/2, 0, scene);
			QPen pen(Qt::black, 0.5, Qt::DashDotLine, Qt::RoundCap, Qt::RoundJoin);
			line->setPen(pen);
			opacity_line = new QGraphicsOpacityEffect();
			opacity_line->setOpacity(0.5);
			line->setGraphicsEffect(opacity_line);
			QPointF pos_t;
			if(!howRecord::useSK42)
				pos_t = recoderToGrad(pos());
			else
				pos_t = recoderSKToWGS(pos());
			degree_minute_sec deg(pos_t);
			QString str = QObject::tr("Lx=%1°%2'%3\nBy=%4°%5'%6").arg(deg.deg_x).arg(deg.minute_x).arg(deg.sec_x).arg(deg.deg_y).arg(deg.minute_y).arg(deg.sec_y);
			//_text += "\n" + str;
			coord = new QGraphicsTextItem(str, this, scene);
			QFont serifFont2("Times", 2, QFont::Bold);
			coord->setFont(serifFont2);
			coord->setPos(5, 14/*x+w/2+1, y-10*/);
			opacity_coord = new QGraphicsOpacityEffect();
			opacity_coord->setOpacity(0.7);
			coord->setGraphicsEffect(opacity_coord);

			text = new QGraphicsTextItem(_text, this, scene);
			QFont serifFont("Times", 2, QFont::Bold);
			text->setFont(serifFont);
			text->setPos(5, 7/*x+w/2+1, y-10*/);
			//text->setPlainText(_text);
			opacity_text = new QGraphicsOpacityEffect();
			opacity_text->setOpacity(0.7);
			text->setGraphicsEffect(opacity_text);
			QRectF r1 = text->boundingRect();
			QRectF r2 = coord->boundingRect();
			qreal w_f, h_f, w_f_m, h_f_m;
			if(r1.width() > r2.width())
				w_f_m = r1.width();
			else
				w_f_m = r2.width();
			if(r1.height() > r2.height())
				h_f_m = r1.height();
			else
				h_f_m = r2.height();

			w_f = w_f_m-4;
			h_f = h_f_m-8;
			
			r1 = text->mapRectToParent(r1);
			r2 = coord->mapRectToParent(r2);
			qreal r_x, r_y;
			r_x = r1.x()+(w_f_m - w_f)/2;
			r_y = r1.y()+(h_f_m - h_f)/2;
			rect_text = new QGraphicsRectItem(QRectF(r_x, r_y, w_f, h_f*2), this, scene);
	};
	~Fantom(){};
	void updateData()
	{
		QPointF point_parent = parent->pos();
		QPointF position = pos();
		line->setLine(point_parent.x(), point_parent.y(), position.x(), position.y());
		line->setGraphicsEffect(opacity_line);

		//////////////////////////////////////////////////////////////////////////
		QRectF r1 = text->boundingRect();
		QRectF r2 = coord->boundingRect();
		qreal w_f, h_f, w_f_m, h_f_m;
		if(r1.width() > r2.width())
			w_f_m = r1.width();
		else
			w_f_m = r2.width();
		if(r1.height() > r2.height())
			h_f_m = r1.height();
		else
			h_f_m = r2.height();

		w_f = w_f_m-4;
		h_f = h_f_m-8;
		
		r1 = text->mapRectToParent(r1);
		r2 = coord->mapRectToParent(r2);
		qreal r_x, r_y;
		r_x = r1.x()+(w_f_m - w_f)/2;
		r_y = r1.y()+(h_f_m - h_f)/2;
		
		rect_text->setRect(QRectF(r_x, r_y, w_f, h_f*2-2));//h_f*2
	};
    QGraphicsTextItem* getText(){ return text; }
    QGraphicsLineItem* getLine(){ return line; }
    QGraphicsEllipseItem* getEllipse(){ return parent; }
	enum { Type = 101 };
    int type() const {return Type;}
    const std::string& objType(){return _objType;}
	void objType(const std::string& typ){_objType=typ;}
private:
	qreal w, h;
	QGraphicsRectItem* rect_text;
	QGraphicsEllipseItem* parent;
	QGraphicsLineItem* line;
	QGraphicsTextItem* text;
	QGraphicsTextItem* coord;
	QGraphicsOpacityEffect *opacity_fantom;
	QGraphicsOpacityEffect *opacity_text;
	QGraphicsOpacityEffect *opacity_line;
	QGraphicsOpacityEffect *opacity_coord;
	std::string _objType;
protected:
	void mouseMoveEvent(QGraphicsSceneMouseEvent *event)
	{
		QPointF pos_t;
		if(!howRecord::useSK42)
			pos_t = recoderToGrad(pos());
		else
			pos_t = recoderSKToWGS(pos());
		degree_minute_sec deg(pos_t);
		QString str = QObject::tr("Type: %1\nLx=%2°%3'%4\nBy=%5°%6'%7").arg(QString::fromLocal8Bit(_objType.c_str())).arg(deg.deg_x).arg(deg.minute_x).arg(deg.sec_x).arg(deg.deg_y).arg(deg.minute_y).arg(deg.sec_y);
		coord->setPlainText(str);
		updateData();
		QGraphicsItem::mouseMoveEvent(event);
	}
};

class ClientConnection;

class TrailItem:public QObject
{
	Q_OBJECT
public:
	TrailItem(QGraphicsScene* scene,int lt=200,int countItems=42):_scene(scene),_lifeTime(lt),_tick(0),_countItems(countItems)
	{
		startTimer(10);
	}
	~TrailItem();
// 	{
// 		foreach(QAbstractGraphicsShapeItem* it,_items)
// 		{
// 			_scene->removeItem(it);
// 		}
// 		_items.clear();
// 		_conn->_trailItem=0;
// 	}
	QVector<QAbstractGraphicsShapeItem*> _items;
	QGraphicsScene* _scene;
	int _lifeTime;
	int _countItems;


	void setPos(QAbstractGraphicsShapeItem* _item,QPointF pos)
	{
		//item->type();
		QAbstractGraphicsShapeItem* item;
		switch (_item->type()) {
		case QGraphicsEllipseItem::Type:
			item = new QGraphicsEllipseItem(-2.0, -2.0, 4, 4, 0, _item->scene());
			item->setBrush(_item->brush());
			item->setPos(QPointF(pos.x()/*+20*/,pos.y()/*+20*/));
			break;
		case QGraphicsPathItem::Type:
			item = new QGraphicsPathItem(0/*_item*/,_item->scene());
			item->setPos(pos);
			break;
		case QGraphicsPolygonItem::Type:
			item = new QGraphicsPolygonItem(0/*_item*/,_item->scene());
			item->setPos(pos);
			break;
		case QGraphicsRectItem::Type:
			item = new QGraphicsRectItem(0/*_item*/,_item->scene());
			item->setPos(pos);
			break;
		case QGraphicsSimpleTextItem::Type:
			item = new QGraphicsSimpleTextItem(0/*_item*/,_item->scene());
			item->setPos(pos);
			break;
		//case QGraphicsLineItem::Type:
			//item = new QGraphicsLineItem(_item,_item->scene());
			//item->setPos(pos);
			//break;
		default:
			//itemPenWidth = 1.0;
			break;
		}
		//QAbstractGraphicsShapeItem* item = new QAbstractGraphicsShapeItem(_item);
		//_items.push_back(item);
		if (_items.size()>_countItems)
		{
			QAbstractGraphicsShapeItem* item_toDel = _items.back();

			_scene->removeItem(item_toDel);

			_items.pop_back();
		}
		QBrush br = item->brush();
		QColor c = br.color();
		c.setAlphaF(1.f);
		br.setColor(c);
		item->setBrush(br);

		_items.insert(0,item);		
	}
	ClientConnection* conn;
protected:
	//ClientConnection* _conn;
    quint64 _tick;
	void timerEvent(QTimerEvent * te)
	{
        qDebug() << te->type();
		++_tick;
		if (_tick>_lifeTime)
		{
			if (!_items.empty()){
			//if (_items.size()>_countItems)
			//{
				QAbstractGraphicsShapeItem* item = _items.back();
				_scene->removeItem(item);
				_items.pop_back();
				_tick=0;
			//}
			}
		}

		float dc = .04f/_countItems;

		foreach(QAbstractGraphicsShapeItem* item, _items)
		{
			//QPointF p=item->pos();

			QBrush br = item->brush();
			QPen pn = item->pen();

			QColor bc = br.color();
			QColor pc = pn.color();

			qreal ca = bc.alphaF();
 			ca -= dc;
						
 
 			if (ca<0){
 				_scene->removeItem(item);
 				continue;
			}

			bc.setAlphaF(ca);			
			br.setColor(bc);
			pc.setAlphaF(ca);
			pn.setColor(pc);

			item->setBrush(br);
			item->setPen(pn);
		}
		//te->
	}
};

class ClientConnection
{
public:
	ClientConnection();
	ClientConnection(quint64 id = -1, QString name = "NO_NAME", QPointF position = QPointF(0,0));
	~ClientConnection();

	static bool doTrailItem;
	static bool doPictures;

	int id_path;

    void setIdConnection(quint64 _id) { mac = _id; }
    void setNameConnection(QString _name) { name = _name; }
	void setPosition(QPointF pos)
	{
		if(ellipse)
		{
			ellipse->setPos(pos);
			position = pos;
			if(fantom)
				fantom->updateData();
			QPointF pos_t;
			if(!howRecord::useSK42)
				pos_t = recoderToGrad(position);
			else
				pos_t = recoderSKToWGS(position);
			degree_minute_sec deg(pos_t);
			//QString coord = "L="+QString::number(pos_t.x())+"\n"+"B="+QString::number(pos_t.y());
			QString str = QObject::tr("Lx=%1°%2'%3\nBy=%4°%5'%6").arg(deg.deg_x).arg(deg.minute_x).arg(deg.sec_x).arg(deg.deg_y).arg(deg.minute_y).arg(deg.sec_y);
			//text_coord->setPlainText(str);
			rect_text->setTextForBody(str);
			if(rect_text->pos() != st_rt_pos)
				rect_text->setPos(st_rt_pos);
			rect_text->verifyCollidins();
			line_rect->setLine(0, 0, rect_text->pos().x(), rect_text->pos().y());

            /*if (_trailItem)
				_trailItem->setPos(ellipse,pos);
			else
			{
				if (doTrailItem){
                    _trailItem = new TrailItem(ellipse->scene());
					_trailItem->conn = this;
					trailVec.push_back(_trailItem);
				}
            }*/
		}

		if (_pixmap)
			_pixmap->setPos(pos.x()-8,pos.y()-8);
	}

	int static_id;
	int mark_static_id;
	void setBrushEllipse(QBrush brush)
	{
		if(!ellipse)
		{
			ellipse->setBrush(brush);
		}
	}
	void setPenEllipse(QPen pen)
	{
		if(!ellipse)
		{
			ellipse->setPen(pen);
		}
	}

	QVector<Point_tab*> points; //QMap<int, QVector<Point_tab*>> points;
	QMap<int, Path_point*> paths;

	QGraphicsPixmapItem* icon_pix;
	
    quint64 getIdConnection() const { return mac; }
    QString* getNameConnection() { return &name; }
    const QPointF getPosition() const { return position; }
    QGraphicsEllipseItem* getEllipse() const { return ellipse; }
	void setEllipse(QGraphicsScene* scene)
	{
		ellipse = new QGraphicsEllipseItem(-2.5, -2.5, 5, 5, 0, scene);
		QBrush brush(Qt::red, Qt::SolidPattern);
		ellipse->setBrush(brush);
		ellipse->setPos(QPointF(6177596.03, 37414370.63));
		//setPosition(QPointF(6177596.03, 37414370.63));
		//name_item = new QGraphicsSimpleTextItem(name, ellipse, scene);
		//QFont serifFont("Times", 2, QFont::Bold);
		//name_item->setFont(serifFont);
		//name_item->setPos(5, 7);
		QPointF pos_t;
		if(!howRecord::useSK42)
			pos_t = recoderToGrad(ellipse->pos());
		else
			pos_t = recoderSKToWGS(ellipse->pos());
		//QString coord = "L="+QString::number(pos_t.x())+"\n"+"B="+QString::number(pos_t.y());
		degree_minute_sec deg(pos_t);
		QString str = QObject::tr("Lx=%1°%2'%3\nBy=%4°%5'%6").arg(deg.deg_x).arg(deg.minute_x).arg(deg.sec_x).arg(deg.deg_y).arg(deg.minute_y).arg(deg.sec_y);
		//text_coord = new QGraphicsTextItem(str, ellipse, scene);
		//QFont serifFont2("Times", 1, QFont::Bold);
		//text_coord->setFont(serifFont);
		//text_coord->setPos(1, 7);
		int _max_size;
		if(name.size() > str.size())
			_max_size = name.size();
		else
			_max_size = str.size();
        QRectF temp_rect = QRectF(0, 0, _max_size, 10);
        rect_text = new RectText(temp_rect, (QGraphicsItem*)ellipse, (QGraphicsScene*)scene, (QString)name, (QString)str);
		rect_text->setPosition(QPointF(5, 7));
		RectText::mapRectText.insert(rect_text->id, rect_text);
		QFont serifFont("Times", 2, QFont::Bold);
		QFont serifFont2("Times", 2, QFont::Bold);
		rect_text->installText(name, str, serifFont, serifFont2, scene, QPointF(0, 0), QPointF(0, 3));
		ellipse->setData(1, mac);
		line_rect = new QGraphicsLineItem(0, 0, rect_text->pos().x(), rect_text->pos().y(), ellipse, scene);

		
        /*if (doTrailItem){
			_trailItem = new TrailItem(scene);
			_trailItem->conn = this;
			trailVec.push_back(_trailItem);
        }*/

// 		_pixmap = new QGraphicsPixmapItem(0,scene);
// 		QString path_appData = qApp->applicationDirPath() + "/Data/pic/";
// 		QPixmap pmap(path_appData+"minus.xpm");
// 		_pixmap->setPixmap(pmap);
    }

	static QVector<TrailItem*> trailVec;

	static void claerTrailVec()
	{
		foreach(TrailItem* i,trailVec)
		{
			delete i;
		}

		trailVec.clear();
	}

	void statePictures(bool _state)
	{
		icon_pix->setEnabled(_state);
		icon_pix->setVisible(_state);
	}

	void setTextForItem(QString text)
	{
		rect_text->setTextForHeader(text);
		//name_item->setText(text);
	}

	void addFantom()
	{
		fantom = new Fantom(ellipse, ellipse->pos().x()+20, ellipse->pos().y()+20, ellipse->rect().height(), ellipse->rect().width(), "Fantom\n"+name, 0, ellipse->scene());
		ellipse->setData(2, true);
		//qDebug() << fantom->type();
		//ellipse->installSceneEventFilter(fantom);
	}
	void deleteFantom()
	{
		if(fantom)
		{
			//ellipse->removeSceneEventFilter(fantom);
			ellipse->scene()->removeItem(fantom->getLine());
			ellipse->scene()->removeItem(fantom->getText());
			ellipse->scene()->removeItem(fantom);
			fantom = 0;
			ellipse->setData(2, false);
		}
	}
	RectText* getRectText(){ return rect_text; }
	int widgetId;
	//QGraphicsSimpleTextItem* name_item;
	//QGraphicsTextItem* text_coord;
	//QPointF getPosition(quint16 id);

private:
    //friend TrailItem;
	quint64 mac;
	QString name;
	QGraphicsEllipseItem* ellipse;

    //TrailItem* _trailItem;

	QGraphicsPixmapItem* _pixmap;

	QGraphicsLineItem* line_rect;
	QPointF position;
	Fantom* fantom;
	RectText* rect_text;
	QPointF st_rt_pos;
};

class ListConnection
{
public:
	ListConnection();
	~ListConnection();

	bool isConnected(quint64 id);
	//bool isConnected(QString name);
	bool addConnection(quint64 id, QString name = "NO_NAME", QPointF point = QPointF(0,0))
	{
		map_connection.insert(id, new ClientConnection(id, name, point));
		return true;
	}
	bool deleteConnection(quint64 id)
	{
        if(map_connection.remove(id)>0)
            return true;
        else
            return false;
    }
	quint64 getIdConnection(QString name);
	QString* getNameConnection(quint64 id);
	QPointF getPosition(QString name);
	QPointF getPosition(quint64 id);
	void setPosition(quint64 id, QPointF pos);
	void setPosition(QString name, QPointF pos);
	QGraphicsEllipseItem* getEllipse(quint64 mac);
	void setName(quint64 mac, QString name)
	{
		map_connection[mac]->setNameConnection(name);
    }

	void setTextForItem(quint64 mac, QString text)
	{
		map_connection[mac]->setTextForItem(text);
	}
	void setEllipse(quint64 mac, QGraphicsScene* scene)
	{
		map_connection[mac]->setEllipse(scene);
	}
	void setWidgetId(quint64 mac, int wi)
	{
		map_connection[mac]->widgetId = wi;
	}
	int getWidgetId(quint64 mac)
	{
		return map_connection[mac]->widgetId;
	}

	ClientConnection* getClientConnectionFromWidgetId(int wi);

	ClientConnection* getClientConnection(quint64 mac)
	{
		return map_connection[mac];
	}
	int getCountConnection()
	{
		return map_connection.count();
	}

	QMap<quint64, ClientConnection*> getMapConnection()
	{
		return map_connection;
	}


private:
	QMap<quint64, ClientConnection*> map_connection;

};


#endif //_CLIENTCONNECTIONS_
