#include "deletetablewidget.h"

DeleteTableWidget::DeleteTableWidget(QList<ClientConnection*> list_connection, QWidget *parent)
	: QWidget(parent)
{
	ui.setupUi(this);
	setWindowIcon(QIcon(":/MapViewer/Resources/delCon.png"));
	setWindowModality(Qt::ApplicationModal);
	setMinimumSize(215, 170);
	resize(215, 170);

	gr_l = new QGridLayout(this);
	sc_a = new QScrollArea(this);
	
	int i = 0;

	QGridLayout* gr_l2 = new QGridLayout();

	foreach(ClientConnection* obj, list_connection)
	{
		QCheckBox *ch = new QCheckBox(*obj->getNameConnection());
		gr_l2->addWidget(ch, i, 0);
		ch->setProperty("mac", obj->getIdConnection());
		listChEmulate.insert(obj->getIdConnection(), ch);
		i++;
	}
	QWidget* w_l = new QWidget();
	w_l->setMaximumWidth(215);
	//gr_l->addLayout(gr_l2, 0, 0);
	w_l->setLayout(gr_l2);
	sc_a->setWidget(w_l);
	gr_l->addWidget(sc_a, 0, 0);
	sc_a->show();
	del = new QPushButton(QString::fromLocal8Bit("Удалить"));
	gr_l->addWidget(del, 1, 0, Qt::AlignRight);
	cancele = new QPushButton(QString::fromLocal8Bit("Отмена"));
	gr_l->addWidget(cancele, 1, 0, Qt::AlignLeft);

	QSize size;
	size = this->size();
	QDesktopWidget *d = QApplication::desktop();
	int w = d->width();   // returns screen width
	int h = d->height();  // returns screen height
	int mw = size.width();
	int mh = size.height();
	int cw = (w/2) - (mw/2);
	int ch = (h/2) - (mh/2);
	move(cw,ch);
	show();
}

DeleteTableWidget::~DeleteTableWidget()
{

}
