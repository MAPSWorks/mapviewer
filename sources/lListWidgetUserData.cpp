#include "lListWidgetUserData.h"
lListWidgetUserData::lListWidgetUserData()
{
    this->pos_image = QPointF(0,0);
    this->path_image = "";
}

lListWidgetUserData::lListWidgetUserData(QPointF pos, QString filename){
    this->pos_image = pos;
    this->path_image = filename;
}


lListWidgetUserData::~lListWidgetUserData()
{
}
