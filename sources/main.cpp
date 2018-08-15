#include "mapviewer.h"
#include "MapObjectContur.h"
#include "LogMessage.h"
#include "StringColorParser.h"
#include <QtGui/QApplication>
#include <QtGui/QPicture>
#include <QtGui/QtGui>
#include <QImageReader>

int main(int argc, char **argv)
{
    QApplication app(argc, argv);
    sphere::SetArgcArgv(argc, argv);
	QString path_app = app.applicationDirPath() + "/";
	app.addLibraryPath(path_app + "plugins/");
    qDebug() << app.libraryPaths();
    qDebug() << QImageReader::supportedImageFormats();
	if(!QDir().exists(path_app + "_Picture"))
		QDir().mkdir(path_app + "_Picture");
	qsrand(QTime(0,0,0).secsTo(QTime::currentTime()));

	LogMessage*_log = new LogMessage;// = new LogMessage();
	
	if(!_log->InitLog(QString(path_app + "_LOGS/")))
    {
		return 0;
	}
	_log->show();
    _log->tracer(path_app);
	_log->tracer(QString("Init scene..."));

    MapViewer w(path_app, _log);
	QGraphicsScene scene;
	scene.setSceneRect(0, 0,6189008, 37515714);

	scene.setItemIndexMethod(QGraphicsScene::NoIndex);
	QBrush brush(QColor(138, 255, 156, 70));
	scene.setBackgroundBrush(brush);
	w.ui.graphicsView->setRenderHint(QPainter::Antialiasing, false);
	w.ui.graphicsView->setDragMode(QGraphicsView::ScrollHandDrag);
	w.ui.graphicsView->setOptimizationFlags(QGraphicsView::DontSavePainterState);
	w.ui.graphicsView->setViewportUpdateMode(QGraphicsView::FullViewportUpdate);
    w.ui.graphicsView->setTransformationAnchor(QGraphicsView::AnchorUnderMouse);

	w.ui.graphicsView->setScene(&scene);

	w.ui.graphicsView->viewport()->setMouseTracking(true);
	w.ui.graphicsView->viewport()->installEventFilter(&w);
    w.ui.treeWidget->viewport()->installEventFilter(&w);

	_log->tracer(QString("Read contur..."));
    if(!w.initialMap(path_app + QString("contur_map.mm")))
	{
        qDebug("Error InitMap!");
		_log->tracer(QString("Read contur - error"));
	}
	_log->tracer(QString("Read contur complete"));
	w.show();

	_log->tracer(QString("Load map..."));
	//readMap();
	_log->tracer(QString("Load map complete"));
	QTimer timer;
	QObject::connect(&timer, SIGNAL(timeout()), &scene, SLOT(advance()));
	timer.start(1000 / 33);

	_log->tracer(QString("Init scene complete"));

	try
	{
		app.exec();
	}
	catch(...)
	{
		//Exit with Errors..
	}

	return 0;
}
