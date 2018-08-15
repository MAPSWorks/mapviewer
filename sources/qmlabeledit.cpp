#include "qmlabeledit.h"

QMLabelEdit::QMLabelEdit(QLineEdit *parent): QObject(parent), editor(parent)
{
	search_widget = 0;
	popup_m = new QTreeWidget;
	popup_m->setWindowFlags(Qt::Popup);
	popup_m->setFocusPolicy(Qt::NoFocus);
	popup_m->setFocusProxy(parent);
	popup_m->setMouseTracking(true);

	popup_m->setColumnCount(2);
	popup_m->setUniformRowHeights(true);
	popup_m->setRootIsDecorated(false);
	popup_m->setEditTriggers(QTreeWidget::NoEditTriggers);
	popup_m->setSelectionBehavior(QTreeWidget::SelectRows);
	popup_m->setFrameStyle(QFrame::Box | QFrame::Plain);
	popup_m->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
	popup_m->header()->hide();

	popup_m->installEventFilter(this);

	connect(popup_m, SIGNAL(itemClicked(QTreeWidgetItem*,int)),
		SLOT(doneCompletion()));

	timer = new QTimer(this);
	timer->setSingleShot(true);
	timer->setInterval(100);
	connect(timer, SIGNAL(timeout()), SLOT(autoSuggest()));
	connect(editor, SIGNAL(textEdited(QString)), timer, SLOT(start()));
}

QMLabelEdit::~QMLabelEdit()
{
	delete popup_m;
}

bool QMLabelEdit::eventFilter(QObject *obj, QEvent *ev)
{
	if (obj != popup_m)
		return false;

	if (ev->type() == QEvent::MouseButtonPress) {
		popup_m->hide();
		editor->setFocus();
		return true;
	}

	if (ev->type() == QEvent::KeyPress) {

		bool consumed = false;
		int key = static_cast<QKeyEvent*>(ev)->key();
		switch (key) {
		case Qt::Key_Enter:
		case Qt::Key_Return:
			doneCompletion();
			consumed = true;

		case Qt::Key_Escape:
			editor->setFocus();
			popup_m->hide();
			consumed = true;

		case Qt::Key_Up:
		case Qt::Key_Down:
		case Qt::Key_Home:
		case Qt::Key_End:
		case Qt::Key_PageUp:
		case Qt::Key_PageDown:
			break;

		default:
			editor->setFocus();
			editor->event(ev);
			popup_m->hide();
			break;
		}

		return consumed;
	}

	return false;
}

void QMLabelEdit::doneCompletion()
{
	timer->stop();
	popup_m->hide();
	editor->setFocus();
	QTreeWidgetItem *item = popup_m->currentItem();
	if (item) {
		editor->setText(item->text(0).simplified());
		QMetaObject::invokeMethod(editor, "returnPressed");
	}
}

void QMLabelEdit::autoSuggest()
{
	QString str = editor->text();
	searchText(str);
}

void QMLabelEdit::searchText(QString str)
{
	if(search_widget)
	{
		str = str.toLower();
		int count_str = search_widget->count();
		QStringList choices;
		for(int i = 0; i<count_str; i++)
		{
			QString t_str = search_widget->item(i)->text();
			int size = str.count();
			QString ba = t_str;
			ba.truncate(size);
			ba = ba.toLower();
			if(str == ba)
				choices << t_str;
		}
		showCompletion(choices);
	}
}

void QMLabelEdit::preventSuggest()
{
	timer->stop();
}

void QMLabelEdit::showCompletion(const QStringList &choices)
{

	if (choices.isEmpty())
		return;

	const QPalette &pal = editor->palette();
	QColor color = pal.color(QPalette::Disabled, QPalette::WindowText);

	popup_m->setUpdatesEnabled(false);
	popup_m->clear();
	for (int i = 0; i < choices.count(); ++i) {
		QTreeWidgetItem * item;
		item = new QTreeWidgetItem(popup_m);
		item->setText(0, choices[i]);
		//item->setText(1, hits[i]);
		item->setTextAlignment(1, Qt::AlignRight);
		item->setTextColor(1, color);
	}
	popup_m->setCurrentItem(popup_m->topLevelItem(0));
	popup_m->resizeColumnToContents(0);
	popup_m->resizeColumnToContents(1);
	popup_m->adjustSize();
	popup_m->setUpdatesEnabled(true);

	int h = popup_m->sizeHintForRow(0) * qMin(7, choices.count()) + 3;
	popup_m->resize(popup_m->width(), h);

	popup_m->move(editor->mapToGlobal(QPoint(0, editor->height())));
	popup_m->setFocus();
	popup_m->show();
}

LineEdit_command::LineEdit_command(QWidget *parent): QLineEdit(parent)
{
	completer = new QMLabelEdit(this);

	connect(this, SIGNAL(returnPressed()),this, SLOT(doSearch()));
}

void LineEdit_command::doSearch()
{
	completer->preventSuggest();
	//QString url = QString(GSEARCH_URL).arg(text());
	//QDesktopServices::openUrl(QUrl(url));
}
