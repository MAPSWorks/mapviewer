#include "LogMessage.h"
#include "StringColorParser.h"

LogMessage::LogMessage(QWidget *parent, Qt::WFlags flags)
	: QMainWindow(parent, flags)
{
	ui.setupUi(this);

	checkBox_2 = new QCheckBox();
	checkBox_2->setObjectName(QString::fromUtf8("checkBox_2"));
	checkBox_2->setGeometry(QRect(9, 9, 420, 20));
	checkBox_2->setMinimumSize(QSize(420, 20));
	checkBox_2->setBaseSize(QSize(420, 20));
	checkBox_2->setLayoutDirection(Qt::LeftToRight);
	checkBox_2->setChecked(true);
	textEdit = new QTextEdit();
	textEdit->setObjectName(QString::fromUtf8("textEdit"));
	textEdit->setGeometry(QRect(9, 35, 420, 300));
	textEdit->setMinimumSize(QSize(420, 300));
	textEdit->setBaseSize(QSize(420, 300));
	textEdit->setAutoFormatting(QTextEdit::AutoAll);
	textEdit->setReadOnly(true);
	lineEdit = new QLineEdit();
	lineEdit->setObjectName(QString::fromUtf8("lineEdit"));
	lineEdit->setGeometry(QRect(9, 371, 420, 20));
	lineEdit->setMinimumSize(QSize(420, 20));
	lineEdit->setBaseSize(QSize(420, 20));
	checkBox = new QCheckBox();
	checkBox->setObjectName(QString::fromUtf8("checkBox"));
	checkBox->setGeometry(QRect(9, 345, 420, 20));
	checkBox->setMinimumSize(QSize(420, 20));
	checkBox->setBaseSize(QSize(420, 20));
	checkBox->setChecked(true);

	checkBox_2->setText(QApplication::translate("LogMessageClass", "\320\241\320\276\321\205\321\200\320\260\320\275\320\270\321\202\321\214 \320\273\320\276\320\263 \320\272\320\260\320\272 HTML", 0, QApplication::UnicodeUTF8));
	checkBox->setText(QApplication::translate("LogMessageClass", "Moved ScrollBar?", 0, QApplication::UnicodeUTF8));

	QWidget* centr_widget = new QWidget();
	QGridLayout* layout = new QGridLayout(centr_widget);
	layout->addWidget(checkBox_2);
	layout->addWidget(textEdit);
	layout->addWidget(checkBox);
	layout->addWidget(lineEdit);

	setCentralWidget(centr_widget);

	//QLayout* layout = this->layout();
	//qDebug() << layout->objectName();
	moved_to_scroll = checkBox->isChecked();
	connect(checkBox, SIGNAL( stateChanged(int)), this, SLOT(check_box_changed(int)));
}


LogMessage::~LogMessage()
{
	file_log.close();
	//LogWindow.exit();
}
void LogMessage::saveHTML()
{
	if(checkBox_2->isChecked())
	{
		QFile html_file;
		html_file.setFileName(fileHtml);
		if (!html_file.open(QIODevice::ReadWrite | QIODevice::Text))
		{
			return;
		}
		QTextStream out(&html_file);
		out << textEdit->toHtml();
		foreach(QString str, name_resorce)
		{
			QVariant im = textEdit->document()->resource(QTextDocument::ImageResource, str);
			QImage picture=im.value<QImage>();
			str = path_to_log_folder->absolutePath() + "/" + str;
			if(!picture.save(str, "png"))
				qDebug() << str;
			else
				qDebug() << "error";
		}
		html_file.close();
	}
}

void LogMessage::check_box_changed(int val)
{
    qDebug() << val;
	moved_to_scroll = checkBox->isChecked();
	QScrollBar* sb = textEdit->verticalScrollBar();
	sb->setTracking(moved_to_scroll);
}
bool LogMessage::InitLog(QString nameLogFile)
{
	if(!QDir().exists(nameLogFile))
		QDir().mkdir(nameLogFile);
	nameLogFile = nameLogFile + QDateTime::currentDateTime().toString().replace(QString(":"), QString("."));
	//nameLogFile.replace(QString(":"), QString("."));
	path_to_log_folder = new QDir(nameLogFile);
	if(!path_to_log_folder->mkdir(nameLogFile))
		return false;
	/*QString path_d = "/"+ QDateTime::currentDateTime().toString();
	path_d.replace(QString(":"), QString("."));
	//path_d.replace(QString(" "), QString("_"));
	path_d = QDir().currentPath() + path_d;
	QString str_n = QDir().currentPath() + QString::fromLocal8Bit("/тест");
	qDebug() << path_d;
	qDebug() << QDir().mkdir(path_d);
	QDir dir(str_n);
	if(dir.exists())
	{
		qDebug() << dir.homePath();
		QFile _f;
		QString _n = dir.path() + "/example.txt";
		_f.setFileName(_n);
		if (!_f.open(QIODevice::ReadWrite | QIODevice::Text | QIODevice::Append))
		{
			qDebug() << "error";
		}
		else
			qDebug() << dir.absolutePath();
		qDebug() << "ok!";
	}
	//qDebug() << dir.mkdir(;
	qDebug() << dir.path();*/
	qDebug() << path_to_log_folder->absolutePath();
	nameLogFile = nameLogFile + "/" + QString("%1_LOG.log").arg(QDateTime::currentDateTime().toString()).replace(QString(":"), QString("."));
	//nameLogFile.replace(QString(":"), QString("."));
	file_log.setFileName(nameLogFile);
	if (!file_log.open(QIODevice::ReadWrite | QIODevice::Text | QIODevice::Append))
	{
		//LogWindow.exit();
		return false;
	}

	fileHtml = QString(nameLogFile + ".html");

	return true;
}

void LogMessage::tracer(QString mes)
{
	QString time = QDateTime::currentDateTime().time().toString();
	time = InsertColor("[", COLOR_TEXT_BLACK) + InsertColor(time, COLOR_TEXT_ORANGE) +
		InsertColor("]", COLOR_TEXT_BLACK) + InsertColor(": ", COLOR_TEXT_BLACK);
	
	QTextCursor cursor = textEdit->textCursor();
	QString str = tr("%1%2\n").arg(time).arg(mes);
	StringColorParser::parseString(str, &cursor);

	QTextStream out(&file_log);
	out.setCodec(QTextCodec::codecForName("UTF-8"));
	out << StringColorParser::clearString(str);
	
	textEdit->append("\n");
	if(moved_to_scroll)
		textEdit->setTextCursor(cursor);
}

void LogMessage::tracer(QImage* picture, QString name)
{
	name = name + ".png";
	QString time = QDateTime::currentDateTime().time().toString();
	time = InsertColor(QuoteSquare(time), COLOR_TEXT_ORANGE) + InsertColor(": ", COLOR_TEXT_BLACK);
	QTextCursor cursor = textEdit->textCursor();
	StringColorParser::parseString(time, &cursor);
	QImage qim = *picture;
	qim = qim.scaled(128, 128, Qt::KeepAspectRatio);
	name_resorce.push_back(name);
	textEdit->document()->addResource(QTextDocument::ImageResource, QUrl(name), qim);
	cursor.insertImage(qim, name);
	if(moved_to_scroll)
		textEdit->setTextCursor(cursor);
	textEdit->append("\n");
	textEdit->append("\n");
}
