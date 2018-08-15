//#include "mapviewer.h"
#include <QtGui/QFrame>

class Layers:public QFrame
{
	//Q_OBJECT
public:
	Layers(QWidget* p=0,Qt::WindowFlags f=0):QFrame(p,f){setVisible(false);}
	//
	void init();

protected slots:
	void addLayer();
	//void delLayer();
};
