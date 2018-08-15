#ifndef QMLABELEDIT_H
#define QMLABELEDIT_H

#include <QObject>
#include <QtGui>

class QLineEdit;

class QMLabelEdit : public QObject
{
	Q_OBJECT

public:
	QMLabelEdit(QLineEdit *parent = 0);
    ~QMLabelEdit();
	bool eventFilter(QObject *obj, QEvent *ev);
	void showCompletion(const QStringList &choices);
	void setWidgetForSearch(QListWidget* widget){
		search_widget = widget;
    }

public slots:
	void doneCompletion();
	void preventSuggest();
	void autoSuggest();

private:
	QLineEdit *editor;
	QTreeWidget *popup_m;
	QTimer *timer;
	QListWidget* search_widget;
	void searchText(QString str);
};

class LineEdit_command : public QLineEdit
{
	Q_OBJECT

public:
	LineEdit_command(QWidget *parent = 0);

	void setWidgetForSearch(QListWidget* widget){
		completer->setWidgetForSearch(widget);
    }

	protected slots:
		void doSearch();

private:
	QMLabelEdit *completer;
};

#endif // QMLABELEDIT_H
