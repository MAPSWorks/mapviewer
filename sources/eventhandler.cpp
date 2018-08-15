#include "eventhandler.h"
#include <QtGui/QtGui>

EventHandler::EventHandler(QObject *parent)
	: QObject(parent)
{
	parent_object = parent;
}

EventHandler::~EventHandler()
{

}

bool EventHandler::eventFilter(QObject *obj, QEvent *ev)
{
    qDebug() << obj;
    qDebug() << ev;
	return true;
}
