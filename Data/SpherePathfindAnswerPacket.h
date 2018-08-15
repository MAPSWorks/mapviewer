//---------------------------------------------------------------------------

#ifndef SpherePathfindAnswerPacketH
#define SpherePathfindAnswerPacketH
//---------------------------------------------------------------------------
#pragma once

//#pragma once

#undef max
#undef min

#include "message/msgDataSet.h"
#include "message/msgType.h"
#include "port/portImpl.h"

// это ответ.
class PACKAGE	TSpherePathfindAnswerPacket
{
public:
	static	GUID							Type;

	class TRawPoint
  {
  public:
  	double X, Y;

    TRawPoint()
    	:X(0)
      ,Y(0)
    {
    }

    TRawPoint(double x, double y)
    	:X(x)
      ,Y(y)
    {
    }
  };

  int												RequestID;
  vector<TRawPoint>					Result;
  std::string								Warnings;

	sphere::IApplicationInfo	AppInfo;

	TSpherePathfindAnswerPacket();

  bool operator == (const TSpherePathfindAnswerPacket& s);

  virtual bool Write(msg::TDataSetPtr storage);
  virtual bool Read(msg::TDataSetPtr storage);
};

MT_DECL(TSpherePathfindAnswerPacket)

#endif
//*/
