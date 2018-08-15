#pragma once
#include <QtGui>

#define COLOR_TEXT_BLACK	"#000000"
#define COLOR_TEXT_WHITE	"#FFFFFF"
#define COLOR_TEXT_RED		"#FF0000"
#define COLOR_TEXT_GREEN	"#008000"
#define COLOR_TEXT_YELLOW	"#FFFF00"
#define COLOR_TEXT_BLUE		"#0000FF"
#define COLOR_TEXT_PINK		"#FF00FF"
#define COLOR_TEXT_ORANGE	"#FF6600"
#define COLOR_TEXT_DARK_GREEN	"#006400"

//#define InsertColor(t,c)	"@\"" + t + "\"" + ":" + c
//#define QuoteDouble(t)	"\"" + t + "\""
//#define QuoteOne(t)	"\'" + t + "\'"
//#define QuoteSquare(t)	"[" + t + "]"

inline QString InsertColor(QString t, QString c) {
	return QString("@\"" + t + "\"" + ":" + c);
}
inline QString QuoteDouble(QString t) {
	return QString("\"" + t + "\"");
}
inline QString QuoteOne(QString t) {
	return QString("\'" + t + "\'");
}
inline QString QuoteSquare(QString t) {
	return QString("[" + t + "]");
}

template<class T> void Append(T tem, QString strText, QString color)
{
	if( tem )
	{//<font  size="+1" color="Red"><strong><i>HTML – язык разметки гипертекста </i></strong></font>
		tem->append("<font  size=\"+2\" color=" + color + "><strong>" + strText + "<\\strong><\\font>");
		tem->append("<font color = black><\\font>");
	}
}

struct StringColor
{
	int index;
	QString text;
	QString color;
};

class StringColorParser
{
public:
	StringColorParser();
	~StringColorParser();
	static QString parseString(QString str, QTextCursor* cursor);
	static QString clearString(QString str);
private:
	static QString parseColor(QString str, QTextCursor* cursor, int* index, StringColor* str_color_temp);
	static QString clearColor(QString &str);
};
