//---------------------------------------------------------------------------

#ifndef SphereCoveringMapRequestPacketH
#define SphereCoveringMapRequestPacketH
//---------------------------------------------------------------------------
#pragma once

#undef max
#undef min

#include "message/msgDataSet.h"
#include "message/msgType.h"
#include "port/portImpl.h"

// это запрос.
class PACKAGE	TSphereCoveringMapRequestPacket
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

	TSphereCoveringMapRequestPacket();

  bool operator == (const TSphereCoveringMapRequestPacket& s);

  virtual bool Write(msg::TDataSetPtr storage);
  virtual bool Read(msg::TDataSetPtr storage);
};

MT_DECL(TSphereCoveringMapRequestPacket)

#endif
//*/
