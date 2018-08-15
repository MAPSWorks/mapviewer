//---------------------------------------------------------------------------

#ifndef SphereVisMapAnswerPacketH
#define SphereVisMapAnswerPacketH
//---------------------------------------------------------------------------
#pragma once

#pragma once

#include "DataPlaneIterator.h"
#include "message/msgDataSet.h"
#include "message/msgType.h"
#include "port/portImpl.h"

// это ответ.
class PACKAGE	TSphereVisMapAnswerPacket
{
public:
	static	GUID										Type;

  int													RequestID;

  std::string											OutputMatrixFileName;
  std::string											OutputQuadTreeFileName;
  std::vector<TDataPlaneElement>                        Data;

  std::string                                           Warnings;

    sphere::IApplicationInfo                            AppInfo;

	TSphereVisMapAnswerPacket();

  bool operator == (const TSphereVisMapAnswerPacket& s);

  virtual bool Write(msg::TDataSetPtr storage);
  virtual bool Read(msg::TDataSetPtr storage);
};

MT_DECL(TSphereVisMapAnswerPacket)


// это ответ.
class PACKAGE	TSphereCrossVisMapAnswerPacket
{
public:
	static	GUID										Type;

  int															RequestID;

  // количество точек из которых видим боец с таким индексом.
  // надо не количество, а индексы, это очень важно.
  // индексы бойцов которые видят данного бойца?
  // если они видят его, то и он видит их.
  std::vector<std::vector<int> > 	Data;

  std::string											Warnings;

	sphere::IApplicationInfo				AppInfo;

	TSphereCrossVisMapAnswerPacket();

  bool operator == (const TSphereCrossVisMapAnswerPacket& s);

  virtual bool Write(msg::TDataSetPtr storage);
  virtual bool Read(msg::TDataSetPtr storage);
};

MT_DECL(TSphereCrossVisMapAnswerPacket)

MT_DECL(TSeqInt)

#endif

