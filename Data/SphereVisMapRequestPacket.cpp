//---------------------------------------------------------------------------


#pragma hdrstop

#define  _USE_MATH_DEFINES
#include "math.h"

#include "SphereVisMapRequestPacket.h"
#include "message/msgField.h"

//---------------------------------------------------------------------------

#define EPSILON 0.0001

#ifdef _CC_BOR_
	#pragma argsused
  #pragma package(smart_init)
#endif

GUID	TSphereVisMapRequestPacket::Type = {0x73330b6a, 0x7972, 0x4aba, 0xa8, 0xa0, 0x5e, 0x2b, 0xef, 0x64, 0x77, 0xa9};

//---------------------------------------------------------------------------
MT_IMPL(TSphereVisMapRequestPacket)
	MT_FIELD("Type",	"Type",  msg::sfGuid,  0, "")

  //MT_FIELD("SrcX",  "SrcX",  msg::sfDouble,  0, "")
 	//MT_FIELD("SrcY",  "SrcY",  msg::sfDouble,  0, "")

  MT_FIELD("PointsElementsCount", "PointsElementsCount",  msg::sfInt32,  0, "")
  MT_FIELD("Points",      				"Points", 							msg::sfBlob,   0, "")

  MT_FIELD("RequestID", "RequestID", msg::sfInt32,  0, "")

  MT_FIELD("MatrixName", "MatrixName", msg::sfVarChar,  0, "")
  MT_FIELD("OutputPathName", "OutputPathName", msg::sfVarChar,  0, "")

  MT_FIELD("HeightOverMap",  "HeightOverMap",  msg::sfDouble,  0, "")

  MT_FIELD("MaximumDistance",  "MaximumDistance",  msg::sfDouble,  0, "")
  MT_FIELD("SmoothProbability", "SmoothProbability", msg::sfBool, 0, "")

  MT_FIELD("AppInfo.ID",    "AppInfo.ID",  msg::sfVarChar,   0, "")
  MT_FIELD("AppInfo.Host",    "AppInfo.Host",  msg::sfVarChar,   0, "")
  MT_FIELD("AppInfo.Category",    "AppInfo.Category",  msg::sfVarChar,   0, "")

MT_END(TSphereVisMapRequestPacket)

//---------------------------------------------------------------------------

TSphereVisMapRequestPacket::TSphereVisMapRequestPacket()
	:RequestID(0)
  ,HeightOverMap(0.0)
  ,MaximumDistance(1500)
  ,SmoothProbability(true)
{
}
//---------------------------------------------------------------------------


bool TSphereVisMapRequestPacket::operator == (const TSphereVisMapRequestPacket& s)
{
  return false;
}
//---------------------------------------------------------------------------


bool TSphereVisMapRequestPacket::Write(msg::TDataSetPtr s)
{
  s->Append();

  s->FieldByName("Type").PutGuid(Type);

  //s->FieldByName("SrcX").PutDouble(SrcX);
  //s->FieldByName("SrcY").PutDouble(SrcY);
  int size = Points.size();
  s->FieldByName("PointsElementsCount").PutInt32(size);
	s->FieldByName("Points").PutBlob((unsigned char *) &Points[0], sizeof(double)*size);


  s->FieldByName("RequestID").PutInt32(RequestID);

  s->FieldByName("MatrixName").PutVarChar(MatrixName);
  s->FieldByName("OutputPathName").PutVarChar(OutputPathName);

	s->FieldByName("HeightOverMap").PutDouble(HeightOverMap);
  s->FieldByName("MaximumDistance").PutDouble(MaximumDistance);

  s->FieldByName("SmoothProbability").PutBool(SmoothProbability);

  s->FieldByName("AppInfo.ID").PutVarChar(AppInfo.ID);
  s->FieldByName("AppInfo.Host").PutVarChar(AppInfo.Host);
  s->FieldByName("AppInfo.Category").PutVarChar(AppInfo.Category);

  return true;
}
//---------------------------------------------------------------------------


bool TSphereVisMapRequestPacket::Read(msg::TDataSetPtr s)
{
  s->FieldByName("Type").GetGuid();

  //SrcX = s->FieldByName("SrcX").GetDouble();
  //SrcY = s->FieldByName("SrcY").GetDouble();
 	int count = s->FieldByName("PointsElementsCount").GetInt32();
  if (Points.size() != count)
  	Points.resize(count);
  s->FieldByName("Points").GetBlob((unsigned char *)&Points[0], count*sizeof(double));


  RequestID 				= s->FieldByName("RequestID").GetInt32();

  MatrixName 				= s->FieldByName("MatrixName").GetVarChar();
  OutputPathName 		= s->FieldByName("OutputPathName").GetVarChar();

  HeightOverMap 		= s->FieldByName("HeightOverMap").GetDouble();
  MaximumDistance 	= s->FieldByName("MaximumDistance").GetDouble();

  SmoothProbability = s->FieldByName("SmoothProbability").GetBool();

  AppInfo.ID        = s->FieldByName("AppInfo.ID").GetVarChar();
  AppInfo.Host      = s->FieldByName("AppInfo.Host").GetVarChar();
  AppInfo.Category  = s->FieldByName("AppInfo.Category").GetVarChar();

  return true;
}
//---------------------------------------------------------------------------


void	TSphereVisMapRequestPacket::AddPoint(double x, double y)
{
	Points.push_back(x);
  Points.push_back(y);
}
//---------------------------------------------------------------------------


GUID	TSphereCrossVisMapRequestPacket::Type = {0x73330b6a, 0x7972, 0x4aba, 0xa8, 0xa0, 0x5e, 0x2b, 0xeb, 0x64, 0x77, 0xa9};

//---------------------------------------------------------------------------
MT_IMPL(TSphereCrossVisMapRequestPacket)
	MT_FIELD("Type",	"Type",  msg::sfGuid,  0, "")

  MT_FIELD("PointsElementsCount", "PointsElementsCount",  msg::sfInt32,  0, "")
  MT_FIELD("Points",      				"Points", 							msg::sfBlob,   0, "")

  MT_FIELD("RequestID", "RequestID", msg::sfInt32,  0, "")

  MT_FIELD("MatrixName", "MatrixName", msg::sfVarChar,  0, "")

  MT_FIELD("HeightOverMap",  "HeightOverMap",  msg::sfDouble,  0, "")

  MT_FIELD("MaximumDistance",  "MaximumDistance",  msg::sfDouble,  0, "")

  MT_FIELD("AppInfo.ID",    "AppInfo.ID",  msg::sfVarChar,   0, "")
  MT_FIELD("AppInfo.Host",    "AppInfo.Host",  msg::sfVarChar,   0, "")
  MT_FIELD("AppInfo.Category",    "AppInfo.Category",  msg::sfVarChar,   0, "")

MT_END(TSphereCrossVisMapRequestPacket)

//---------------------------------------------------------------------------

TSphereCrossVisMapRequestPacket::TSphereCrossVisMapRequestPacket()
	:RequestID(0)
  ,HeightOverMap(0.0)
  ,MaximumDistance(1500)
{
}
//---------------------------------------------------------------------------


bool TSphereCrossVisMapRequestPacket::operator == (const TSphereCrossVisMapRequestPacket& s)
{
  return false;
}
//---------------------------------------------------------------------------


bool TSphereCrossVisMapRequestPacket::Write(msg::TDataSetPtr s)
{
  s->Append();

  s->FieldByName("Type").PutGuid(Type);

  //s->FieldByName("SrcX").PutDouble(SrcX);
  //s->FieldByName("SrcY").PutDouble(SrcY);
  int size = Points.size();
  s->FieldByName("PointsElementsCount").PutInt32(size);
	s->FieldByName("Points").PutBlob((unsigned char *) &Points[0], sizeof(double)*size);


  s->FieldByName("RequestID").PutInt32(RequestID);

  s->FieldByName("MatrixName").PutVarChar(MatrixName);

	s->FieldByName("HeightOverMap").PutDouble(HeightOverMap);
  s->FieldByName("MaximumDistance").PutDouble(MaximumDistance);

  s->FieldByName("AppInfo.ID").PutVarChar(AppInfo.ID);
  s->FieldByName("AppInfo.Host").PutVarChar(AppInfo.Host);
  s->FieldByName("AppInfo.Category").PutVarChar(AppInfo.Category);

  return true;
}
//---------------------------------------------------------------------------


bool TSphereCrossVisMapRequestPacket::Read(msg::TDataSetPtr s)
{
  s->FieldByName("Type").GetGuid();

  //SrcX = s->FieldByName("SrcX").GetDouble();
  //SrcY = s->FieldByName("SrcY").GetDouble();
 	int count = s->FieldByName("PointsElementsCount").GetInt32();
  if (Points.size() != count)
  	Points.resize(count);
  s->FieldByName("Points").GetBlob((unsigned char *)&Points[0], count*sizeof(double));


  RequestID = s->FieldByName("RequestID").GetInt32();

  MatrixName = s->FieldByName("MatrixName").GetVarChar();

  HeightOverMap = s->FieldByName("HeightOverMap").GetDouble();
  MaximumDistance = s->FieldByName("MaximumDistance").GetDouble();

  AppInfo.ID       = s->FieldByName("AppInfo.ID").GetVarChar();
  AppInfo.Host     = s->FieldByName("AppInfo.Host").GetVarChar();
  AppInfo.Category = s->FieldByName("AppInfo.Category").GetVarChar();

  return true;
}
//---------------------------------------------------------------------------


void	TSphereCrossVisMapRequestPacket::AddPoint(double x, double y)
{
	Points.push_back(x);
  Points.push_back(y);
}
//---------------------------------------------------------------------------
