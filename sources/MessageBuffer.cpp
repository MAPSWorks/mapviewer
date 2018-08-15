#include "MessageBuffer.h"


MessageBuffer::MessageBuffer(LogMessage* log, bool timerMes, QWidget *parent, Qt::WFlags flags)
    : QMainWindow(parent, flags)
{
	ui.setupUi(this);
	_log = log;
    show();
	if(timerMes)
    this->startTimer(10000);
}


MessageBuffer::~MessageBuffer()
{
}

void MessageBuffer::addMessage(quint16 id, MessageStatus* mes_st)
{
	buf_messages.insert(id, mes_st);
	ui.textEdit->append(tr("id: [%1]\tmac: [%2]\ttime_send: [%3]\tsize: [%4]\t").arg(id).arg(mes_st->mac)
		.arg(mes_st->time_mark.toString()).arg(mes_st->size_message));
}
void MessageBuffer::deleteMessage(quint16 id)
{
	ui.textEdit->clear();
	buf_messages.remove(id);
	typedef QMap<quint16, MessageStatus*>::Iterator mi;
	for(mi iter = buf_messages.begin(); iter != buf_messages.end(); iter++)
	{
		MessageStatus* e = *iter;
		ui.textEdit->append(tr("id: [%1]\tmac: [%2]\ttime_send: [%3]\tsize: [%4]\t").arg(e->id_message).arg(e->mac)
			.arg(e->time_mark.toString()).arg(e->size_message));
	}
}
void MessageBuffer::timerEvent(QTimerEvent *event)
{
	QTime _time = QDateTime::currentDateTime().time();
	typedef QMap<quint16, MessageStatus*>::Iterator mi;
	for(mi iter = buf_messages.begin(); iter != buf_messages.end(); iter++)
	{
		MessageStatus* e = *iter;
		int _t1 = e->time_mark.msecsTo(_time);
		//int _t2 = e->time_mark.msecsTo(e->time_mark);
		if(_t1 > 10000)
		{
			//emit sendMessagePack(e->buffer);
			//_log->tracer(tr("repeat send message: id: [%1] mac: [%2]").arg(e->id_message).arg(e->mac));
			this->deleteMessage(e->id_message);
			_log->tracer(tr("message not send.."));
		}
	}
}

void MessageBuffer::repeatMessage(quint16 id)
{
	MessageStatus* mes = buf_messages.value(id);
	if(mes)
	{
		// хм..
		//emit sendMessagePack(mes->buffer);
		//_log->tracer(tr("repeat send message: id: [%1] mac: [%2]").arg(mes->id_message).arg(mes->mac));
	}
}
