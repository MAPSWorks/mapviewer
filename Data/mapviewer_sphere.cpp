#pragma once

#include<QtGui>

#include "mapviewer.h"
#include "message/simple/tfieldholder.h"
#include "core/sphereDefs.h"

bool operator ==(const GUID &ob1, const GUID &ob2)
{
    if(ob1.Data1 == ob2.Data1)
        if(ob1.Data2 == ob2.Data2)
            if(ob1.Data3 == ob2.Data3)
                //if(strcmp((const char*)ob1.Data4, (const char*)ob2.Data4) == 0)
                    return true;
    return false;
}

void MapViewer::GetPacketFromPathfindService(TPort* port, msg::TDataSetPtr ds)
{
	bool typedPacket = !ds->FieldByName("Type").Empty();
    if (typedPacket && (ds->FieldByName("Type").GetGuid() == TSpherePathfindAnswerPacket::Type))
	{ // это pu
		TSpherePathfindAnswerPacket temp;
		temp.Read(ds);
		int count = temp.Result.size();

		if (!temp.Warnings.empty())
			_log->tracer(temp.Warnings.c_str());
//		if (temp.Warnings == "")
// 		{		
// 			int n = 0;
// 		} else 
// 		{
// 			int n = 0;
// 		}
		QGraphicsPathItem* path = new QGraphicsPathItem(0,ui.graphicsView->scene());
		//UNDO_DESC("создана полилиния пути");
		//GPline * pline = GPlineConstPtr(Drawing->CreateObject("GPline")).HackPtr();

		//PLINE3 pl;
		QPainterPath pp;

		double deltaX = (6178618.16+6177068.35)*0.5;
		double deltaY = (37413798.94+37415078.40)*0.5;

		for (COUNT i = 0, in = temp.Result.size(); i < in; i++)
		{
			double x = -temp.Result[i].X+deltaX;
			double y = temp.Result[i].Y+deltaY;

			if (i == 0)
				pp.moveTo(x,y);
			else
				pp.lineTo(x,y);
			//pl.Add(POINT3(x, y, 0));
			//line->ad
		}

		path->setPath(pp);

		vectorSphereShapes.push_back(path);

		//pline->GetGPlineFrom(pl);
		//PEN pen(TColor(ColorToRGB(clYellow) | 0xFF000000), 2, psSolid);
		//pline->SetCanvas.Pen = pen;
		//pline->Notify(gooChanged);
	}
}

void MapViewer::GetPacketFromVisMapService(TPort* port, msg::TDataSetPtr ds)
{
	bool typedPacket = !ds->FieldByName("Type").Empty();
	if (typedPacket && ds->FieldByName("Type").GetGuid() == TSphereVisMapAnswerPacket::Type)
	{ // это pu
		TSphereVisMapAnswerPacket temp;
		temp.Read(ds);
		int n = 0;
		if (!temp.Warnings.empty())
			_log->tracer(temp.Warnings.c_str());
		// reader
		//if (FILE * file = fopen(temp.OutputMatrixFileName.c_str(), "rb"))
        std::string data_file = std::string(path_app.toStdString() + temp.OutputQuadTreeFileName);
        int x = 0;
        while(x != -1)
        {
            x= data_file.find('\\');
            if(x != -1)
                data_file.replace(x, 1, "/");
        }
        if (FILE * file = fopen(data_file.c_str(), "rb"))
		{
			//TDataPlaneRegular dataPlane;
			TDataPlaneQuadTree dataPlane;
			dataPlane.Read(file);
			fclose(file);

			int allcount = 0;
			int whitecount = 0;

			//Bricks.Clear();

			//UNDO_SKIP("");
			//if (TDataPlaneIterator * iter = dataPlane.CreateBoundedIterator(1, 10000000))
			if (TDataPlaneIterator * iter = dataPlane.CreateIterator())
			{
				//LOCKDRW lock(Drawing);
				double deltaX = (6178618.16+6177068.35)*0.5;				
				double deltaY = (37413798.94+37415078.40)*0.5;

				while (iter->Next())
				{
					const TDataPlaneElement& elem = iter->Current();

					double Left   = -elem.Left+deltaX;
					double Top    = elem.Top+deltaY;
					double Right  = -elem.Right+deltaX;
					double Bottom = elem.Bottom+deltaY;

					double gr = elem.Value;
						
					QPointF point_lt(Left, Top);
					QPointF point_rb(Right, Bottom);//

					//QRectF r1(0, 0, point_lt.x()-point_rb.x(), point_lt.y()-point_rb.y());
					QRectF r1(point_lt,point_rb);

					QGraphicsRectItem *r = new QGraphicsRectItem(r1, 0, ui.graphicsView->scene());

					//BRICK2 br;
					//br << POINT2(elem.Left, elem.Top);
					//br << POINT2(elem.Right, elem.Bottom);
					//Bricks << br;

					//r->setPos(point_lt.x(), point_lt.y());
					r->setPos(0,0);
                    QBrush brush(Qt::white, Qt::SolidPattern);
                    brush.setColor(QColor(gr/*255*//*128*/,0,0,128)/*Qt::black*/);
                    r->setBrush(brush);
					QPen pen(brush.color());
					r->setPen(pen);

					vectorSphereShapes.push_back(r);

					whitecount++;
				}
				//Drawing->Repaint();
				delete iter;
			}
			int n = 0;
		}
	}

	//drawSuperMap
}

void MapViewer::GetPacketFromCoveringMapService(TPort* port, msg::TDataSetPtr ds)
{
	bool typedPacket = !ds->FieldByName("Type").Empty();
    if (typedPacket && (ds->FieldByName("Type").GetGuid() == TSphereCoveringMapAnswerPacket::Type))
	{ // это pu
		TSphereCoveringMapAnswerPacket temp;
		temp.Read(ds);
		int n = 0;

		if (!temp.Warnings.empty())
			_log->tracer(temp.Warnings.c_str());

		// reader
		//if (FILE * file = fopen(temp.OutputMatrixFileName.c_str(), "rb"))
		if (FILE * file = fopen(temp.OutputQuadTreeFileName.c_str(), "rb"))
		{
			//TDataPlaneRegular dataPlane;
			TDataPlaneQuadTree dataPlane;
			dataPlane.Read(file);
			fclose(file);

			int allcount = 0;
			int whitecount = 0;

			if (TDataPlaneIterator * iter = dataPlane.CreateIterator())
			{
				double deltaX = (6178618.16+6177068.35)*0.5;				
				double deltaY = (37413798.94+37415078.40)*0.5;

				while (iter->Next())
				{
					const TDataPlaneElement& elem = iter->Current();

					//if (whitecount%10==0){
					double Left   = -elem.Left+deltaX;
					double Top    = elem.Top+deltaY;
					double Right  = -elem.Right+deltaX;
					double Bottom = elem.Bottom+deltaY;

					double gr = elem.Value;

					QPointF point_lt(Left, Top);
					QPointF point_rb(Right, Bottom);//

					QRectF r1(point_lt,point_rb);

					QGraphicsRectItem *r = new QGraphicsRectItem(r1, 0, ui.graphicsView->scene());

					r->setPos(0,0);
					QBrush brush(Qt::white, Qt::SolidPattern);
					if (fabs(gr-1) < 0.001)
						brush.setColor(QColor(gr,128,gr,128));
					else if (fabs(gr-2) < 0.001)
						brush.setColor(QColor(gr,128,128,128));
					r->setBrush(brush);
					QPen pen(brush.color());
					r->setPen(pen);
					//}

					vectorSphereShapes.push_back(r);

					whitecount++;
				}
				delete iter;
			}
			int n = 0;
		}
	}
}

void MapViewer::GetPacketFromCoveringPointService(TPort* port, msg::TDataSetPtr ds)
{
	bool typedPacket = !ds->FieldByName("Type").Empty();
    if (typedPacket && (ds->FieldByName("Type").GetGuid() == TSphereCoveringPointAnswerPacket::Type))
	{ // это pu
		TSphereCoveringPointAnswerPacket temp;
		temp.Read(ds);
		int n = 0;

		if (!temp.Warnings.empty())
			_log->tracer(temp.Warnings.c_str());

		// reader
		//if (FILE * file = fopen(temp.OutputMatrixFileName.c_str(), "rb"))
		if (FILE * file = fopen(temp.OutputQuadTreeFileName.c_str(), "rb"))
		{
			//TDataPlaneRegular dataPlane;
			TDataPlaneQuadTree dataPlane;
			dataPlane.Read(file);
			fclose(file);

			int allcount = 0;
			int whitecount = 0;

			if (TDataPlaneIterator * iter = dataPlane.CreateIterator())
			{
				double deltaX = (6178618.16+6177068.35)*0.5;				
				double deltaY = (37413798.94+37415078.40)*0.5;

				while (iter->Next())
				{
					const TDataPlaneElement& elem = iter->Current();

					//if (whitecount%10==0){
					double Left   = -elem.Left+deltaX;
					double Top    = elem.Top+deltaY;
					double Right  = -elem.Right+deltaX;
					double Bottom = elem.Bottom+deltaY;

					double gr = elem.Value;

					QPointF point_lt(Left, Top);
					QPointF point_rb(Right, Bottom);//

					QRectF r1(point_lt,point_rb);

					QGraphicsRectItem *r = new QGraphicsRectItem(r1, 0, ui.graphicsView->scene());

					r->setPos(0,0);
					QBrush brush(Qt::white, Qt::SolidPattern);
					if (fabs(gr-1) < 0.001)
						brush.setColor(QColor(gr,128,gr,128));
					else if (fabs(gr-2) < 0.001)
						brush.setColor(QColor(gr,128,128,128));
					r->setBrush(brush);
					QPen pen(brush.color());
					r->setPen(pen);
					//}

					vectorSphereShapes.push_back(r);

					whitecount++;
				}
				delete iter;
			}
			int n = 0;
		}
	}
}

void MapViewer::GetPacketFromReachableBoundService(TPort* port, msg::TDataSetPtr ds)
{
	bool typedPacket = !ds->FieldByName("Type").Empty();
	if (typedPacket && ds->FieldByName("Type").GetGuid() == TSphereReachableBoundAnswerPacket::Type)
	{ // это pu
		TSphereReachableBoundAnswerPacket temp;
		temp.Read(ds);
		int n = 0;

		if (!temp.Warnings.empty())
			_log->tracer(temp.Warnings.c_str());

		// reader
		//if (FILE * file = fopen(temp.OutputMatrixFileName.c_str(), "rb"))
		if (FILE * file = fopen(temp.OutputQuadTreeFileName.c_str(), "rb"))
		{
			//TDataPlaneRegular dataPlane;
			TDataPlaneQuadTree dataPlane;
			dataPlane.Read(file);
			fclose(file);

			int allcount = 0;
			int whitecount = 0;

			if (TDataPlaneIterator * iter = dataPlane.CreateIterator())
			{
				double deltaX = (6178618.16+6177068.35)*0.5;				
				double deltaY = (37413798.94+37415078.40)*0.5;

				while (iter->Next())
				{
					const TDataPlaneElement& elem = iter->Current();

					//if (whitecount%10==0){
					double Left   = -elem.Left+deltaX;
					double Top    = elem.Top+deltaY;
					double Right  = -elem.Right+deltaX;
					double Bottom = elem.Bottom+deltaY;

					double gr = elem.Value;

					QPointF point_lt(Left, Top);
					QPointF point_rb(Right, Bottom);//

					QRectF r1(point_lt,point_rb);

					QGraphicsRectItem *r = new QGraphicsRectItem(r1, 0, ui.graphicsView->scene());

					r->setPos(0,0);
					QBrush brush(Qt::white, Qt::SolidPattern);
					brush.setColor(QColor(gr*4,gr*4,255,128));
					r->setBrush(brush);
					QPen pen(brush.color());
					r->setPen(pen);
					//}

					vectorSphereShapes.push_back(r);

					whitecount++;
				}
				delete iter;
			}
			int n = 0;
		}
	}
}

void MapViewer::clearShapes()
{
	foreach(QAbstractGraphicsShapeItem* sh, vectorSphereShapes)
	{
		ui.graphicsView->scene()->removeItem(sh);
	}

	vectorSphereShapes.clear();
}
