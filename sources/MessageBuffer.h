#ifndef _MESSAGEBUFFER_
#define _MESSAGEBUFFER_

#include <QObject>
#include <QtGui/QtGui>
#include "ui_widget_message_buffer.h"
#include "LogMessage.h"

struct MessageStatus
{
	QTime time_mark;
	quint16 id_message;
	quint32 size_message;
	quint64 mac;
	QByteArray buffer;
};
class MessageBuffer : public QMainWindow
{
	Q_OBJECT

public:
	MessageBuffer(LogMessage* log, bool timerMes = true, QWidget *parent = 0, Qt::WFlags flags = 0);
	~MessageBuffer();
	void addMessage(quint16 id, MessageStatus* mes_st);
	void deleteMessage(quint16 id);
	void repeatMessage(quint16 id);

private:
	QMap<quint16, MessageStatus*> buf_messages;
	Ui::MessageBufferClass ui;
	LogMessage* _log;

	void timerEvent(QTimerEvent *event);

//private slots:
//	void repeat_send();

signals:
	void sendMessagePack(QByteArray mes);
};

#endif //_MESSAGEBUFFER_
