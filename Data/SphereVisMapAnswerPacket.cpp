//---------------------------------------------------------------------------


#pragma hdrstop

#define  _USE_MATH_DEFINES
#include "math.h"

#include "SphereVisMapAnswerPacket.h"
#include "message/msgField.h"

//---------------------------------------------------------------------------

#define EPSILON 0.0001

#ifdef _CC_BOR_
	#pragma argsused
  #pragma package(smart_init)
#endif

MT_IMPL(TSphereVisMapAnswerPacket)
	MT_FIELD("Type",					 "Type",  					msg::sfGuid,  0, "")
  MT_FIELD("RequestID", 		 "RequestID", 			msg::sfInt32,  0, "")

  MT_FIELD("OutputMatrixFileName", "OutputMatrixFileName",  msg::sfVarChar,   0, "")
  MT_FIELD("OutputQuadTreeFileName", "OutputQuadTreeFileName",  msg::sfVarChar,   0, "")

  MT_FIELD("Data",      				"Data", 							msg::sfBlob,   0, "")

  MT_FIELD("Warnings", "Warnings",  msg::sfVarChar,   0, "")

  MT_FIELD("AppInfo.ID",    "AppInfo.ID",  msg::sfVarChar,   0, "")
  MT_FIELD("AppInfo.Host",    "AppInfo.Host",  msg::sfVarChar,   0, "")
  MT_FIELD("AppInfo.Category",    "AppInfo.Category",  msg::sfVarChar,   0, "")
MT_END(TSphereVisMapAnswerPacket)

GUID	TSphereVisMapAnswerPacket::Type = {0x62ef1112, 0xba84, 0x4bbb, 0x8d, 0x60, 0x6e, 0x36, 0x0b, 0x7a, 0xfa, 0x27};

TSphereVisMapAnswerPacket::TSphereVisMapAnswerPacket()
	:RequestID(0)
{
}

bool TSphereVisMapAnswerPacket::operator == (const TSphereVisMapAnswerPacket& s)
{
  return false;
}

bool TSphereVisMapAnswerPacket::Write(msg::TDataSetPtr s)
{
  s->Append();
  s->FieldByName("Type").PutGuid(Type);

  s->FieldByName("RequestID").PutInt32(RequestID);

  s->FieldByName("OutputMatrixFileName").PutVarChar(OutputMatrixFileName);
  s->FieldByName("OutputQuadTreeFileName").PutVarChar(OutputQuadTreeFileName);

	s->FieldByName("Data").PutVector(Data);

  s->FieldByName("Warnings").PutVarChar(Warnings);

  s->FieldByName("AppInfo.ID").PutVarChar(AppInfo.ID);
  s->FieldByName("AppInfo.Host").PutVarChar(AppInfo.Host);
  s->FieldByName("AppInfo.Category").PutVarChar(AppInfo.Category);
  return true;
}

bool TSphereVisMapAnswerPacket::Read(msg::TDataSetPtr s)
{
  s->FieldByName("Type").GetGuid();

  RequestID = s->FieldByName("RequestID").GetInt32();

  OutputMatrixFileName 	= s->FieldByName("OutputMatrixFileName").GetVarChar();
  OutputQuadTreeFileName 	= s->FieldByName("OutputQuadTreeFileName").GetVarChar();

  s->FieldByName("Data").GetVector(Data);

  Warnings 							= s->FieldByName("Warnings").GetVarChar();

  AppInfo.ID       = s->FieldByName("AppInfo.ID").GetVarChar();
  AppInfo.Host     = s->FieldByName("AppInfo.Host").GetVarChar();
  AppInfo.Category = s->FieldByName("AppInfo.Category").GetVarChar();

  return true;
}


//----------------------------------------------------------------------------------------------------------------

MT_IMPL(TSphereCrossVisMapAnswerPacket)
	MT_FIELD("Type",					 "Type",  					msg::sfGuid,  0, "")
  MT_FIELD("RequestID", 		 "RequestID", 			msg::sfInt32,  0, "")

  MT_FIELD("Data",      				"Data", 				msg::sfDataSet,   0, "TSeqInt")

  MT_FIELD("Warnings", "Warnings",  msg::sfVarChar,   0, "")

  MT_FIELD("AppInfo.ID",    "AppInfo.ID",  msg::sfVarChar,   0, "")
  MT_FIELD("AppInfo.Host",    "AppInfo.Host",  msg::sfVarChar,   0, "")
  MT_FIELD("AppInfo.Category",    "AppInfo.Category",  msg::sfVarChar,   0, "")
MT_END(TSphereCrossVisMapAnswerPacket)

MT_IMPL(TSeqInt)
	MT_FIELD("Data",      				"Data", 							msg::sfBlob,   0, "")
MT_END(TSeqInt)


GUID	TSphereCrossVisMapAnswerPacket::Type = {0x62ef1113, 0xba84, 0x4bbb, 0x8d, 0x60, 0x6e, 0x36, 0x0b, 0x7a, 0xfa, 0x27};

TSphereCrossVisMapAnswerPacket::TSphereCrossVisMapAnswerPacket()
	:RequestID(0)
{
}
//-------------------------------------------------------------------------------------------------------

bool TSphereCrossVisMapAnswerPacket::operator == (const TSphereCrossVisMapAnswerPacket& s)
{
  return false;
}
//-------------------------------------------------------------------------------------------------------

bool TSphereCrossVisMapAnswerPacket::Write(msg::TDataSetPtr s)
{
  s->Append();
  s->FieldByName("Type").PutGuid(Type);

  s->FieldByName("RequestID").PutInt32(RequestID);

  msg::TDataSetPtr data = s->FieldByName("Data").PutDataSet(MT_ID(TSeqInt));
  for (COUNT i = 0, in = Data.size(); i < in; i++)
  {
  	data->Append();
  	data->FieldByName("Data").PutVector(Data[i]);
  }

	//s->FieldByName("Data").PutVector(Data);

  s->FieldByName("Warnings").PutVarChar(Warnings);

  s->FieldByName("AppInfo.ID").PutVarChar(AppInfo.ID);
  s->FieldByName("AppInfo.Host").PutVarChar(AppInfo.Host);
  s->FieldByName("AppInfo.Category").PutVarChar(AppInfo.Category);
  return true;
}
//-------------------------------------------------------------------------------------------------------

bool TSphereCrossVisMapAnswerPacket::Read(msg::TDataSetPtr s)
{
  s->FieldByName("Type").GetGuid();

  RequestID = s->FieldByName("RequestID").GetInt32();

  //s->FieldByName("Data").GetVector(Data);
  msg::TDataSetPtr data = s->FieldByName("Data").GetDataSet();

  for(data->First(); !data->Eof(); data->Next())
  {
  	std::vector<int> temp;
    data->FieldByName("Data").GetVector(temp);
    Data.push_back(temp);
  }

  Warnings 				 = s->FieldByName("Warnings").GetVarChar();

  AppInfo.ID       = s->FieldByName("AppInfo.ID").GetVarChar();
  AppInfo.Host     = s->FieldByName("AppInfo.Host").GetVarChar();
  AppInfo.Category = s->FieldByName("AppInfo.Category").GetVarChar();

  return true;
}
//-------------------------------------------------------------------------------------------------------
