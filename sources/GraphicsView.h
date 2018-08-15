#include <QtGui/QtGui>

class GraphicsView : public QGraphicsView
{
public:
	GraphicsView()
	{
		setWindowTitle(tr("Model Viewer"));
	}

protected:
	void resizeEvent(QResizeEvent *event) {
		if (scene())
			scene()->setSceneRect(
			QRect(QPoint(0, 0), event->size()));
		QGraphicsView::resizeEvent(event);
	}
};
