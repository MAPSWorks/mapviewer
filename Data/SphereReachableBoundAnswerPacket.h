//---------------------------------------------------------------------------

#ifndef SphereReachableBoundAnswerPacketH
#define SphereReachableBoundAnswerPacketH
//---------------------------------------------------------------------------
#pragma once

#pragma once

#undef max
#undef min

#include "message/msgDataSet.h"
#include "message/msgType.h"
#include "port/portImpl.h"

// это ответ.
class PACKAGE	TSphereReachableBoundAnswerPacket
{
public:
	static	GUID							Type;

  int												RequestID;

  std::string								OutputMatrixFileName;
  std::string								OutputQuadTreeFileName;

  std::string								Warnings;

	sphere::IApplicationInfo	AppInfo;



	TSphereReachableBoundAnswerPacket();

  bool operator == (const TSphereReachableBoundAnswerPacket& s);

  virtual bool Write(msg::TDataSetPtr storage);
  virtual bool Read(msg::TDataSetPtr storage);
};

MT_DECL(TSphereReachableBoundAnswerPacket)

#endif
//*/
