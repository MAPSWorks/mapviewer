//---------------------------------------------------------------------------

#ifndef SphereVisMapRequestPacketH
#define SphereVisMapRequestPacketH
//---------------------------------------------------------------------------
#pragma once

#include "message/msgDataSet.h"
#include "message/msgType.h"
#include "port/portImpl.h"

// это запрос на расчёт карты прямой видимости.
class PACKAGE	TSphereVisMapRequestPacket
{
public:
	static	GUID							Type;

  std::vector<double>				Points;

  double										HeightOverMap;

  int												RequestID; // идентификатор запроса приходит снаружи.

  std::string								MatrixName;
  std::string								OutputPathName;

  double										MaximumDistance;
  bool											SmoothProbability;

  sphere::IApplicationInfo	AppInfo;

	TSphereVisMapRequestPacket();

  bool operator == (const TSphereVisMapRequestPacket& s);

  virtual bool Write(msg::TDataSetPtr storage);
  virtual bool Read(msg::TDataSetPtr storage);

  void	AddPoint(double x, double y);
};

MT_DECL(TSphereVisMapRequestPacket)

class PACKAGE	TSphereCrossVisMapRequestPacket
{
public:
	static	GUID							Type;

  std::vector<double>				Points;

  double										HeightOverMap;

  int												RequestID; // идентификатор запроса приходит снаружи.

  std::string								MatrixName;
  double										MaximumDistance;

  sphere::IApplicationInfo	AppInfo;

	TSphereCrossVisMapRequestPacket();

  bool operator == (const TSphereCrossVisMapRequestPacket& s);

  virtual bool Write(msg::TDataSetPtr storage);
  virtual bool Read(msg::TDataSetPtr storage);

  void	AddPoint(double x, double y);
};

MT_DECL(TSphereCrossVisMapRequestPacket)

#endif
