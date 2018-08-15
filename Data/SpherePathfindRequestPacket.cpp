//---------------------------------------------------------------------------


#pragma hdrstop

#define  _USE_MATH_DEFINES
#include "math.h"

#include "SpherePathfindRequestPacket.h"
#include "message/simple/tfieldholder.h"

//---------------------------------------------------------------------------

#define EPSILON 0.0001

#ifdef _CC_BOR_
	#pragma argsused
  #pragma package(smart_init)
#endif

GUID	TSpherePathfindRequestPacket::Type = {0x73330b6a, 0x7972, 0x4aba, 0xa8, 0xa0, 0x5e, 0x2b, 0xef, 0x64, 0x77, 0xa9};

MT_IMPL(TSpherePathfindRequestPacket)
	MT_FIELD("Type",	"Type",  msg::sfGuid,  0, "")

  MT_FIELD("SrcX",  "SrcX",  msg::sfDouble,  0, "")
 	MT_FIELD("SrcY",  "SrcY",  msg::sfDouble,  0, "")

  MT_FIELD("DstX", "DstX", msg::sfDouble,  0, "")
 	MT_FIELD("DstY", "DstY", msg::sfDouble,  0, "")

  MT_FIELD("RequestID", "RequestID", msg::sfInt32,  0, "")

  MT_FIELD("MatrixName", "MatrixName", msg::sfVarChar,  0, "")

  MT_FIELD("AppInfo.ID",    "AppInfo.ID",  msg::sfVarChar,   0, "")
  MT_FIELD("AppInfo.Host",    "AppInfo.Host",  msg::sfVarChar,   0, "")
  MT_FIELD("AppInfo.Category",    "AppInfo.Category",  msg::sfVarChar,   0, "")

MT_END(TSpherePathfindRequestPacket)

TSpherePathfindRequestPacket::TSpherePathfindRequestPacket()
	:SrcX(0)
	,SrcY(0)
	,DstX(0)
	,DstY(0)
	,RequestID(0)
{
}

bool TSpherePathfindRequestPacket::operator == (const TSpherePathfindRequestPacket& s)
{
  return false;
}

bool TSpherePathfindRequestPacket::Write(msg::TDataSetPtr s)
{
  s->Append();

  s->FieldByName("Type").PutGuid(Type);

  s->FieldByName("SrcX").PutDouble(SrcX);
  s->FieldByName("SrcY").PutDouble(SrcY);

  s->FieldByName("DstX").PutDouble(DstX);
  s->FieldByName("DstY").PutDouble(DstY);

  s->FieldByName("RequestID").PutInt32(RequestID);

  s->FieldByName("MatrixName").PutVarChar(MatrixName);

  s->FieldByName("AppInfo.ID").PutVarChar(AppInfo.ID);
  s->FieldByName("AppInfo.Host").PutVarChar(AppInfo.Host);
  s->FieldByName("AppInfo.Category").PutVarChar(AppInfo.Category);

  return true;
}

bool TSpherePathfindRequestPacket::Read(msg::TDataSetPtr s)
{
  s->FieldByName("Type").GetGuid();

  SrcX = s->FieldByName("SrcX").GetDouble();
  SrcY = s->FieldByName("SrcY").GetDouble();

  DstX = s->FieldByName("DstX").GetDouble();
  DstY = s->FieldByName("DstY").GetDouble();

  RequestID = s->FieldByName("RequestID").GetInt32();

  MatrixName = s->FieldByName("MatrixName").GetVarChar();

  AppInfo.ID       = s->FieldByName("AppInfo.ID").GetVarChar();
  AppInfo.Host     = s->FieldByName("AppInfo.Host").GetVarChar();
  AppInfo.Category = s->FieldByName("AppInfo.Category").GetVarChar();

  return true;
}


//*/
