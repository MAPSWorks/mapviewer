#pragma once
#include <QtConcurrentRun>

#undef max
#undef min

#include "messages.h"

#include "core/sphereFile.h"
#include "message/msgDataSet.h"
#include "port/portImpl.h"

class TMyReceiver:public QObject
{
	Q_OBJECT
public:
	void ReceiveCallback(TPort* port, msg::TDataSetPtr dataSet)
	{
		//TMyClass var;
		//var.Read(dataSet);

		//if (!badData){

// 			TEXTMSG_t var;
// 			var.Read(dataSet);
// 
// 			//++TotalCall;
// 			//printf("%d \n",TotalCall);
// 			printf("hdr: id=%ui, mac=%ll, tip=%ui \n",var.hdr.id,var.hdr.mac,var.hdr.tip );
// 			printf("text  \n",var.text);
		//}

            emit doUrl(QString()/*var.text*/);
    }

signals:
	void doUrl(QString);
};
