#include "layers.h"

#include <QListWidget>
#include <QPushButton>
#include <QGridLayout>

void Layers::init()
{
	QWidget* gr = new QWidget(0/*this->parentWidget()*/);

	QListWidget* list = new QListWidget(gr);
	list->addItem(QString::fromLocal8Bit("слой"));	

	QPushButton* add = new QPushButton(gr);
	add->setText("add");

	QPushButton* del = new QPushButton(gr);
	del->setText("del");

	QGridLayout *mainLayout = new QGridLayout;
	mainLayout->addWidget(list);//, 0, 0);
	mainLayout->addWidget(add);//, 1, 0/*, Qt::AlignTop*/);
	mainLayout->addWidget(del);//, 1, 1);

	gr->setLayout(mainLayout);
	gr->show();
}
