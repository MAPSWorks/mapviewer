//---------------------------------------------------------------------------

#ifndef SphereReachableBoundRequestPacketH
#define SphereReachableBoundRequestPacketH
//---------------------------------------------------------------------------
#pragma once 

#undef max
#undef min

#include "message/msgDataSet.h"
#include "message/msgType.h"
#include "port/portImpl.h"

// это запрос.
class PACKAGE	TSphereReachableBoundRequestPacket
{
public:
	static	GUID							Type;

  double										SrcX; // это расположение.
  double										SrcY;

  double										Speed;
  double										Time;

  int												RequestID; // идентификатор запроса приходит снаружи.

  std::string								MatrixName;
  std::string								OutputPathName;

  sphere::IApplicationInfo	AppInfo;

	TSphereReachableBoundRequestPacket();

  bool operator == (const TSphereReachableBoundRequestPacket& s);

  virtual bool Write(msg::TDataSetPtr storage);
  virtual bool Read(msg::TDataSetPtr storage);
};

MT_DECL(TSphereReachableBoundRequestPacket)

#endif
//*/
