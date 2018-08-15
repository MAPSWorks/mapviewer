#ifndef EVENTHANDLER_H
#define EVENTHANDLER_H

#include <QObject>

class EventHandler : public QObject
{
	Q_OBJECT

public:
	EventHandler(QObject *parent);
	~EventHandler();

private:

	QObject *parent_object;

	bool eventFilter(QObject *obj, QEvent *ev);
};

#endif // EVENTHANDLER_H
