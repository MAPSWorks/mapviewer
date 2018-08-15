//---------------------------------------------------------------------------

#ifndef SphereCoveringMapAnswerPacketH
#define SphereCoveringMapAnswerPacketH
//---------------------------------------------------------------------------
#pragma once

#pragma once

#undef max
#undef min

#include "message/msgDataSet.h"
#include "message/msgType.h"
#include "port/portImpl.h"

// это ответ.
class PACKAGE	TSphereCoveringMapAnswerPacket
{
public:
	static	GUID							Type;

  int												RequestID;

  std::string								OutputMatrixFileName;
  std::string								OutputQuadTreeFileName;

  std::string								Warnings;

	sphere::IApplicationInfo	AppInfo;



	TSphereCoveringMapAnswerPacket();

  bool operator == (const TSphereCoveringMapAnswerPacket& s);

  virtual bool Write(msg::TDataSetPtr storage);
  virtual bool Read(msg::TDataSetPtr storage);
};

MT_DECL(TSphereCoveringMapAnswerPacket)


#endif

//*/
