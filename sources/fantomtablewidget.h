#ifndef FANTOMTABLEWIDGET_H
#define FANTOMTABLEWIDGET_H

#undef max

#include <QWidget>
#include "ui_fantomtablewidget.h"

#include "ClientConnection.h"

class MapViewer;

struct EmulData
{
	QString name;
	QPointF position;
	double speed;
	double direction;
	int type_obj;
};

class FantomTableWidget : public QWidget
{
	Q_OBJECT

	int selectedType;
public:
	FantomTableWidget(const MapViewer* _m_v, QWidget *parent = 0);
	~FantomTableWidget();

	QGridLayout *gr_l;
	QLabel *name_l;
	QLineEdit *name_edit;
	QLabel *position_l;
	QLabel *pos_x_l;
	QLineEdit *pos_x_le;
	QLabel *pos_y_l;
	QLineEdit *pos_y_le;
	QPushButton *sel_pos;
	MapViewer* map_viewer;
	QPushButton* addTable;
	QPushButton* cancel;
	QListView* _listView;
	QComboBox* list_type;

private:
	Ui::FantomTableWidget ui;

	private slots:
		void hideSelectPosition();
		void setPosition(QPointF);
		void addTableToMV();
		void destroyW();
signals:
		void clickSelectPosition();
		void addTS(EmulData data);
};

#endif // FANTOMTABLEWIDGET_H
