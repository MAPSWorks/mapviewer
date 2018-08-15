#ifndef _LOGMESSAGE_
#define _LOGMESSAGE_

#undef max

#include <QObject>
#include <QtGui/QtGui>
#include <QtCore/QFile>
#include "ui_widget_log.h"

class LogMessage : public QMainWindow
{
	Q_OBJECT

public:
	LogMessage(QWidget *parent = 0, Qt::WFlags flags = 0);
	~LogMessage();
	//static QCoreApplication LogWindow;

	/* Initialization LogMessage. Need transmit name logFile */
	bool InitLog(QString nameLogFile);
	void tracer(QString mes);
	void tracer(QImage* picture, QString name);
	void saveHTML();
private:
	QCheckBox *checkBox_2;
	QTextEdit *textEdit;
	QLineEdit *lineEdit;
	QCheckBox *checkBox;

	QFile file_log;
	Ui::LogMessageClass ui;
	bool moved_to_scroll;
	QString fileHtml;
	QVector<QString> name_resorce;
	QDir* path_to_log_folder;
private slots:
	void check_box_changed(int val);
};
#endif //_LOGMESSAGE_
