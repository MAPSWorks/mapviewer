#ifndef DELETETABLEWIDGET_H
#define DELETETABLEWIDGET_H

#include <QWidget>
#include "ui_deletetablewidget.h"
#include "ClientConnection.h"

class DeleteTableWidget : public QWidget
{
	Q_OBJECT

public:
	DeleteTableWidget(QList<ClientConnection*> list_connection, QWidget *parent = 0);
	~DeleteTableWidget();

	QGridLayout *gr_l;
	QScrollArea *sc_a;
	QPushButton *del;
	QPushButton *cancele;
	QMap<quint64, QCheckBox*> listChEmulate;

private:
	Ui::DeleteTableWidget ui;
};

#endif // DELETETABLEWIDGET_H
