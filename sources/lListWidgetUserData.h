//#include <QtGui/QtGUi>
#include <QtGui/QtGui>

class lListWidgetUserData
{
public:
    lListWidgetUserData();
    lListWidgetUserData(QPointF pos, QString filename);
	~lListWidgetUserData();
    inline QString getPath_image() {
		return path_image;
	}
    inline QPointF getPos_image() {
		return pos_image;
	}
private:
	QPointF pos_image;
	QString path_image;
};

