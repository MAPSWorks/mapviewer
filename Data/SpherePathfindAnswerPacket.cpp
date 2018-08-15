//---------------------------------------------------------------------------


#pragma hdrstop

#define  _USE_MATH_DEFINES
#include "math.h"

#include "SpherePathfindAnswerPacket.h"
#include "message/simple/tfieldholder.h"

//---------------------------------------------------------------------------

#define EPSILON 0.0001

#ifdef _CC_BOR_
	#pragma argsused
  #pragma package(smart_init)
#endif

MT_IMPL(TSpherePathfindAnswerPacket)
	MT_FIELD("Type",					 "Type",  					msg::sfGuid,  0, "")
  MT_FIELD("RequestID", 		 "RequestID", 			msg::sfInt32,  0, "")

  MT_FIELD("PointsInResult", "PointsInResult",	msg::sfInt32,  0, "")
  MT_FIELD("ResultBuffer",   "ResultBuffer", 		msg::sfBlob,  0, "")

  MT_FIELD("Warnings", 			  "Warnings", 		msg::sfVarChar,  0, "")


  MT_FIELD("AppInfo.ID",    "AppInfo.ID",  msg::sfVarChar,   0, "")
  MT_FIELD("AppInfo.Host",    "AppInfo.Host",  msg::sfVarChar,   0, "")
  MT_FIELD("AppInfo.Category",    "AppInfo.Category",  msg::sfVarChar,   0, "")
MT_END(TSpherePathfindAnswerPacket)

GUID	TSpherePathfindAnswerPacket::Type = {0x62ef1112, 0xba84, 0x4bbb, 0x8d, 0x60, 0x6e, 0x36, 0x0b, 0x7a, 0xfa, 0x27};

TSpherePathfindAnswerPacket::TSpherePathfindAnswerPacket()
	:RequestID(0)
	,Result(500)
{
}

bool TSpherePathfindAnswerPacket::operator == (const TSpherePathfindAnswerPacket& s)
{
  return false;
}

bool TSpherePathfindAnswerPacket::Write(msg::TDataSetPtr s)
{
  s->Append();
  s->FieldByName("Type").PutGuid(Type);

  s->FieldByName("RequestID").PutInt32(RequestID);

  s->FieldByName("PointsInResult").PutInt32(Result.size());
  {
  	int count = Result.size();
  	TRawPoint * data = new TRawPoint[count];
    for (COUNT i = 0, in = count; i < in; i++)
    {
    	TRawPoint & cur = data[i];
      cur = Result[i];
      int n = 0;
      //Result.push_back(cur);
    }

  	s->FieldByName("ResultBuffer").PutBlob(&data[0], sizeof(TRawPoint)*count);
    delete[] data;
  }

  s->FieldByName("Warnings").PutVarChar(Warnings);

  s->FieldByName("AppInfo.ID").PutVarChar(AppInfo.ID);
  s->FieldByName("AppInfo.Host").PutVarChar(AppInfo.Host);
  s->FieldByName("AppInfo.Category").PutVarChar(AppInfo.Category);
  return true;
}

bool TSpherePathfindAnswerPacket::Read(msg::TDataSetPtr s)
{
  s->FieldByName("Type").GetGuid();

  RequestID = s->FieldByName("RequestID").GetInt32();

  Result.clear();
  int count = s->FieldByName("PointsInResult").GetInt32();
  TRawPoint * data = new TRawPoint[count];
  s->FieldByName("ResultBuffer").GetBlob(&data[0], count*sizeof(TRawPoint));
  for (COUNT i = 0, in = count; i < in; i++)
  	Result.push_back(data[i]);
  delete[] data;

  Warnings = s->FieldByName("Warnings").GetVarChar();

  AppInfo.ID       = s->FieldByName("AppInfo.ID").GetVarChar();
  AppInfo.Host     = s->FieldByName("AppInfo.Host").GetVarChar();
  AppInfo.Category = s->FieldByName("AppInfo.Category").GetVarChar();

  return true;
}
//*/
