//---------------------------------------------------------------------------


#pragma hdrstop

#define  _USE_MATH_DEFINES
#include "math.h"

#include "SphereCoveringMapRequestPacket.h"
#include "message/simple/tfieldholder.h"

//---------------------------------------------------------------------------

#define EPSILON 0.0001

#ifdef _CC_BOR_
	#pragma argsused
  #pragma package(smart_init)
#endif

GUID	TSphereCoveringMapRequestPacket::Type = {0x73330b6a, 0x7972, 0x4aba, 0xa8, 0xa0, 0x5e, 0x2b, 0xef, 0x64, 0x77, 0xa9};

MT_IMPL(TSphereCoveringMapRequestPacket)
	MT_FIELD("Type",	"Type",  msg::sfGuid,  0, "")

  MT_FIELD("SrcX",  "SrcX",  msg::sfDouble,  0, "")
 	MT_FIELD("SrcY",  "SrcY",  msg::sfDouble,  0, "")

  MT_FIELD("RequestID", "RequestID", msg::sfInt32,  0, "")

  MT_FIELD("ObjectType", "ObjectType", msg::sfVarChar,  0, "")

  MT_FIELD("MatrixName", "MatrixName", msg::sfVarChar,  0, "")
  MT_FIELD("OutputPathName", "OutputPathName", msg::sfVarChar,  0, "")


  MT_FIELD("AppInfo.ID",    "AppInfo.ID",  msg::sfVarChar,   0, "")
  MT_FIELD("AppInfo.Host",    "AppInfo.Host",  msg::sfVarChar,   0, "")
  MT_FIELD("AppInfo.Category",    "AppInfo.Category",  msg::sfVarChar,   0, "")

MT_END(TSphereCoveringMapRequestPacket)

TSphereCoveringMapRequestPacket::TSphereCoveringMapRequestPacket()
	:SrcX(0)
	,SrcY(0)
	,RequestID(0)
{
}

bool TSphereCoveringMapRequestPacket::operator == (const TSphereCoveringMapRequestPacket& s)
{
  return false;
}

bool TSphereCoveringMapRequestPacket::Write(msg::TDataSetPtr s)
{
  s->Append();

  s->FieldByName("Type").PutGuid(Type);

  s->FieldByName("SrcX").PutDouble(SrcX);
  s->FieldByName("SrcY").PutDouble(SrcY);

  s->FieldByName("RequestID").PutInt32(RequestID);

  s->FieldByName("ObjectType").PutVarChar(ObjectType);

  s->FieldByName("MatrixName").PutVarChar(MatrixName);
  s->FieldByName("OutputPathName").PutVarChar(OutputPathName);


  s->FieldByName("AppInfo.ID").PutVarChar(AppInfo.ID);
  s->FieldByName("AppInfo.Host").PutVarChar(AppInfo.Host);
  s->FieldByName("AppInfo.Category").PutVarChar(AppInfo.Category);

  return true;
}

bool TSphereCoveringMapRequestPacket::Read(msg::TDataSetPtr s)
{
  s->FieldByName("Type").GetGuid();

  SrcX = s->FieldByName("SrcX").GetDouble();
  SrcY = s->FieldByName("SrcY").GetDouble();

  RequestID = s->FieldByName("RequestID").GetInt32();

  ObjectType = s->FieldByName("ObjectType").GetVarChar();

  MatrixName = s->FieldByName("MatrixName").GetVarChar();
  OutputPathName = s->FieldByName("OutputPathName").GetVarChar();

  AppInfo.ID       = s->FieldByName("AppInfo.ID").GetVarChar();
  AppInfo.Host     = s->FieldByName("AppInfo.Host").GetVarChar();
  AppInfo.Category = s->FieldByName("AppInfo.Category").GetVarChar();

  return true;
}

//*/
