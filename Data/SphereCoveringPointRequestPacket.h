//---------------------------------------------------------------------------

#ifndef SphereCoveringPointRequestPacketH
#define SphereCoveringPointRequestPacketH
//---------------------------------------------------------------------------
#pragma once

#include "message/msgDataSet.h"
#include "message/msgType.h"
#include "port/portImpl.h"

// это запрос.
class PACKAGE	TSphereCoveringPointRequestPacket
{
public:
	static	GUID							Type;

  double										SrcX; // это расположение.
  double										SrcY;

  int												RequestID; // идентификатор запроса приходит снаружи.

  std::string								MatrixName;
  std::string								OutputPathName;
  std::string								ObjectType;

  sphere::IApplicationInfo	AppInfo;

	TSphereCoveringPointRequestPacket();

  bool operator == (const TSphereCoveringPointRequestPacket& s);

  virtual bool Write(msg::TDataSetPtr storage);
  virtual bool Read(msg::TDataSetPtr storage);
};

MT_DECL(TSphereCoveringPointRequestPacket)

#endif
//*/
