//---------------------------------------------------------------------------


#pragma hdrstop

#define  _USE_MATH_DEFINES
#include "math.h"

#include "SphereReachableBoundAnswerPacket.h"
#include "message/simple/tfieldholder.h"

//---------------------------------------------------------------------------

#define EPSILON 0.0001

#ifdef _CC_BOR_
	#pragma argsused
  #pragma package(smart_init)
#endif

MT_IMPL(TSphereReachableBoundAnswerPacket)
	MT_FIELD("Type",					 "Type",  					msg::sfGuid,  0, "")
  MT_FIELD("RequestID", 		 "RequestID", 			msg::sfInt32,  0, "")

  MT_FIELD("PointsInResult", "PointsInResult",	msg::sfInt32,  0, "")
  MT_FIELD("ResultBuffer",   "ResultBuffer", 		msg::sfBlob,  0, "")

  MT_FIELD("OutputMatrixFileName", "OutputMatrixFileName",  msg::sfVarChar,   0, "")
  MT_FIELD("OutputQuadTreeFileName", "OutputQuadTreeFileName",  msg::sfVarChar,   0, "")

  MT_FIELD("Warnings", "Warnings",  msg::sfVarChar,   0, "")

  MT_FIELD("AppInfo.ID",    "AppInfo.ID",  msg::sfVarChar,   0, "")
  MT_FIELD("AppInfo.Host",    "AppInfo.Host",  msg::sfVarChar,   0, "")
  MT_FIELD("AppInfo.Category",    "AppInfo.Category",  msg::sfVarChar,   0, "")
MT_END(TSphereReachableBoundAnswerPacket)

GUID	TSphereReachableBoundAnswerPacket::Type = {0x62ef1113, 0xba84, 0x4bbb, 0x8d, 0x60, 0x6e, 0x36, 0x0b, 0x7a, 0xfa, 0x27};

TSphereReachableBoundAnswerPacket::TSphereReachableBoundAnswerPacket()
	:RequestID(0)
{
}

bool TSphereReachableBoundAnswerPacket::operator == (const TSphereReachableBoundAnswerPacket& s)
{
  return false;
}

bool TSphereReachableBoundAnswerPacket::Write(msg::TDataSetPtr s)
{
  s->Append();
  s->FieldByName("Type").PutGuid(Type);

  s->FieldByName("RequestID").PutInt32(RequestID);

  s->FieldByName("OutputMatrixFileName").PutVarChar(OutputMatrixFileName);
  s->FieldByName("OutputQuadTreeFileName").PutVarChar(OutputQuadTreeFileName);

  s->FieldByName("Warnings").PutVarChar(Warnings);

  s->FieldByName("AppInfo.ID").PutVarChar(AppInfo.ID);
  s->FieldByName("AppInfo.Host").PutVarChar(AppInfo.Host);
  s->FieldByName("AppInfo.Category").PutVarChar(AppInfo.Category);
  return true;
}

bool TSphereReachableBoundAnswerPacket::Read(msg::TDataSetPtr s)
{
  s->FieldByName("Type").GetGuid();

  RequestID = s->FieldByName("RequestID").GetInt32();

  OutputMatrixFileName 	= s->FieldByName("OutputMatrixFileName").GetVarChar();
  OutputQuadTreeFileName 	= s->FieldByName("OutputQuadTreeFileName").GetVarChar();

  Warnings 							= s->FieldByName("Warnings").GetVarChar();

  AppInfo.ID       = s->FieldByName("AppInfo.ID").GetVarChar();
  AppInfo.Host     = s->FieldByName("AppInfo.Host").GetVarChar();
  AppInfo.Category = s->FieldByName("AppInfo.Category").GetVarChar();

  return true;
}
//*/
