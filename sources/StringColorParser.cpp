#include "StringColorParser.h"


StringColorParser::StringColorParser()
{
}


StringColorParser::~StringColorParser()
{
}

QString StringColorParser::parseString(QString str, QTextCursor* cursor)
{
	QString result_string = "";
	if(cursor)
	{
		//int pos_cursor = cursor->position();
		cursor->clearSelection();
		cursor->movePosition(QTextCursor::End);
		int j = 0;
		int last_index = 0;
		int last_index_color = 0;
		QTextCharFormat charFormat;
		QColor _color(COLOR_TEXT_BLACK);
		charFormat.setFontWeight(QFont::Normal);
		while((j = str.indexOf("@\"", j)) != -1)
		{
			charFormat.setForeground(_color);
			charFormat.setFontWeight(QFont::Normal);
			QString left_str = str.mid(last_index, j-last_index);
			result_string += left_str;
			//cursor->setPositio
			cursor->insertText(left_str, charFormat);
			StringColor* str_color_temp = new StringColor;
			QString temp_str = str.mid(j+2);
			QString right_str = parseColor(temp_str, cursor, &last_index_color, str_color_temp);
			if(str_color_temp->text != "")
			{
				QColor _tempColor(str_color_temp->color);
				charFormat.setForeground(_tempColor);
				charFormat.setFontWeight(QFont::Bold);
				cursor->insertText(str_color_temp->text, charFormat);
				result_string += str_color_temp->text;
			}
			delete str_color_temp;
			last_index = j+2 + last_index_color;
			++j;
		}
		charFormat.setForeground(_color);
		charFormat.setFontWeight(QFont::Normal);
		if(last_index != str.size())
		{
			QString last = str.mid(last_index, str.size()-last_index);
			cursor->insertText(last, charFormat);
			result_string += last;
		}
		cursor->insertText(" ", charFormat);
		//cursor->setPosition(pos_cursor);
	}
	return result_string;
}

QString StringColorParser::parseColor(QString str, QTextCursor* cursor, int* index, StringColor* str_color_temp)
{
	int j = 0;
	j = str.indexOf("\":");
	if(j == -1)
	{
		str_color_temp->text = "";
		str_color_temp->index = -1;
		str_color_temp->text = "";
		return "";
	}
	*index = j+9;
	QString color_str = str.mid(j+2, 7);
	QString text = str.mid(0, j);
	str_color_temp->color = color_str;
	str_color_temp->text = text;
	str_color_temp->index = *index;
	//qDebug() << color_str;
	return str;
}

QString StringColorParser::clearString(QString str)
{
	int j = 0;
	while((j = str.indexOf("@\"", j)) != -1)
	{
		str.remove(j, 2);
		clearColor(str);
		j++;
	}
	return str;
}

QString StringColorParser::clearColor(QString &str)
{
	int j = 0;
	j = str.indexOf("\":");
	if(j != -1)
	{
		str.remove(j, 9);
	}
	return "";
}
