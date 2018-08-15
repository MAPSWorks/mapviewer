//---------------------------------------------------------------------------

#ifndef SpherePathfindRequestPacketH
#define SpherePathfindRequestPacketH
//---------------------------------------------------------------------------
#pragma once

#undef max
#undef min

#include "message/msgDataSet.h"
#include "message/msgType.h"
#include "port/portImpl.h"

// это запрос.
class PACKAGE	TSpherePathfindRequestPacket
{
public:
	static	GUID							Type;

  double										SrcX;
  double										SrcY;

  double										DstX;
  double										DstY;

  int												RequestID; // идентификатор запроса приходит снаружи.

  std::string								MatrixName;

  sphere::IApplicationInfo	AppInfo;

	TSpherePathfindRequestPacket();

  bool operator == (const TSpherePathfindRequestPacket& s);

  virtual bool Write(msg::TDataSetPtr storage);
  virtual bool Read(msg::TDataSetPtr storage);
};

MT_DECL(TSpherePathfindRequestPacket)

#endif
//*/
