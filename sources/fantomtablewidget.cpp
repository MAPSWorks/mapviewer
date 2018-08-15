#include "fantomtablewidget.h"
#include "mapviewer.h"

FantomTableWidget::FantomTableWidget(const MapViewer* _m_v, QWidget *parent)
	: QWidget(parent)
{
	map_viewer = (MapViewer*)_m_v;
	//connect(_m_v, SIGNAL(selectPosition(QPointF)), this, SLOT(setPosition(QPointF)));
	ui.setupUi(this);
	setWindowModality(Qt::ApplicationModal);

	setWindowIcon(QIcon(":/MapViewer/Resources/addCon.png"));
	setWindowModality(Qt::ApplicationModal);
	setMinimumSize(215, 170);
	resize(215, 170);
	gr_l = new QGridLayout(this);
	name_l = new QLabel(QString::fromLocal8Bit("Имя:"), this);
	gr_l->addWidget(name_l, 0, 0);
	name_edit = new QLineEdit(QString::fromLocal8Bit("Планшет1"), this);
	gr_l->addWidget(name_edit, 0, 1);
	position_l = new QLabel(QString::fromLocal8Bit("Позиция:"), this);
	gr_l->addWidget(position_l, 1, 0);
	QLabel* _temp = new QLabel(QString("/"), this);
	gr_l->addWidget(_temp, 1, 1);
	pos_x_l = new QLabel(QString::fromLocal8Bit("X:"), this);
	gr_l->addWidget(pos_x_l, 2, 0);
	QPointF pos_default;
	if(!howRecord::useSK42)
		pos_default = recoderToGrad(QPointF(6177596.03, 37414370.63));
	else
		pos_default = recoderSKToWGS(QPointF(6157888.75, 7382259.50));
	pos_x_le = new QLineEdit(QString::number(pos_default.x(), 'f'));
	gr_l->addWidget(pos_x_le, 2, 1);
	pos_y_l = new QLabel(QString::fromLocal8Bit("Y:"), this);
	gr_l->addWidget(pos_y_l, 3, 0);
	pos_y_le = new QLineEdit(QString::number(pos_default.y(), 'f'));
	gr_l->addWidget(pos_y_le, 3, 1);
	sel_pos = new QPushButton(QString::fromLocal8Bit("Выбрать на карте..."), this);
	gr_l->addWidget(sel_pos, 4, 1);
	QLabel* _type = new QLabel(QString::fromLocal8Bit("Тип:"), this);
	gr_l->addWidget(_type, 5, 0);
	list_type = new QComboBox();
	_listView = new QListView(list_type);
	list_type->setStyleSheet("QListView::item {                              \
							  border-bottom: 5px solid white; margin:3px; }  \
							  QListView::item:selected {                     \
							  border-bottom: 5px solid black; margin:3px;    \
							  color: black;                                  \
							  }                                               \
							  ");
	int i = 0;
	foreach(QString _str, MapViewer::name_type)
	{
		i++;
		QIcon* _qi = 0;
		QPixmap _pix = MapViewer::pixmap_type.value(i);
		if(!_pix.isNull())
			_qi = new QIcon(_pix);

		list_type->addItem(*_qi, _str);
	}
	list_type->setView(_listView);
	gr_l->addWidget(list_type, 5, 1);
	cancel = new QPushButton(QString::fromLocal8Bit("Отмена"), this);
	gr_l->addWidget(cancel, 6, 0);
	addTable = new QPushButton(QString::fromLocal8Bit("Добавить"), this);
	gr_l->addWidget(addTable, 6, 1);

	connect(sel_pos, SIGNAL(clicked()), this, SLOT(hideSelectPosition()));
	connect(cancel, SIGNAL(clicked()), this, SLOT(destroyW()));
	connect(addTable, SIGNAL(clicked()), this, SLOT(addTableToMV()));

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

FantomTableWidget::~FantomTableWidget()
{

}

void FantomTableWidget::hideSelectPosition()
{
	hide();
	emit clickSelectPosition();
}

void FantomTableWidget::setPosition(QPointF _pos)
{
	pos_x_le->setText(QString::number(_pos.x(), 'f'));
	pos_y_le->setText(QString::number(_pos.y(), 'f'));
	show();
}

void FantomTableWidget::addTableToMV()
{
	EmulData _d;
	_d.name = name_edit->text();
	_d.position = QPointF(pos_x_le->text().toDouble(), pos_y_le->text().toDouble());
	_d.speed = 0;
    _d.direction = 0;
	QItemSelectionModel* _e = _listView->selectionModel();
	selectedType = _e->currentIndex().row();
	_d.type_obj = selectedType+1;
	//qDebug() << selectedType;
	emit addTS(_d);
	deleteLater();
}

void FantomTableWidget::destroyW()
{
	deleteLater();
}
