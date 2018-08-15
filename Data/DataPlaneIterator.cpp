//---------------------------------------------------------------------------


#pragma hdrstop

#include "DataPlaneIterator.h"
#include "core/sphereDefs.h"

#include <float.h>
#include <math.h>

//----------------------------------------------------------------------------------------

#ifndef _MSC_VER
#pragma package(smart_init)
#endif

#define EXCLUDE_READWRITE
//#define EXCLUDE_SMOOTHCOMPARE

#ifndef EXCLUDE_SMOOTHCOMPARE

#define EPSILON 0.0001

bool IsEqual(double a, double b, double eps)
{
  return fabs(a - b) < eps;
}
//---------------------------------------------------------------------------

bool IsGreaterEqual(double a, double b, double eps)
{
  return a > b || IsEqual(a, b, eps);
}
//---------------------------------------------------------------------------

bool IsGreater(double a, double b, double eps)
{
  return a > (b + eps);
}
//---------------------------------------------------------------------------

bool IsLess(double a, double b, double eps)
{
  return a < (b - eps);
}
//---------------------------------------------------------------------------

bool IsLessEqual(double a, double b, double eps)
{
  return a < b || IsEqual(a, b, eps);
}
//---------------------------------------------------------------------------

#endif



//#ifndef EXCLUDE_READWRITE

template <typename T>
int _READFILE(T &t,FILE *file)
{
  return (fread(&t,sizeof(T),1,file)==1);
}
//----------------------------------------------------------------------------------------

template <typename T>
int _WRITEFILE(const T &t,FILE *file)
{
  return (fwrite(&t,sizeof(T),1,file)==1);
}
//----------------------------------------------------------------------------------------
//#endif


bool TDataPlaneElement::Read(FILE * file)
{
	if (!file)
  	return false;

  _READFILE(Left, file);
  _READFILE(Top, file);
  _READFILE(Right, file);
  _READFILE(Bottom, file);
  _READFILE(Value, file);

	return true;
}

bool TDataPlaneElement::Write(FILE * file)
{
	if (!file)
  	return false;

  _WRITEFILE(Left, file);
  _WRITEFILE(Top, file);
  _WRITEFILE(Right, file);
  _WRITEFILE(Bottom, file);
  _WRITEFILE(Value, file);

	return true;
}


////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////

TDataPlaneRegular::TDataPlaneRegular()
	:DataBuffer(NULL)
  ,Left(0)
  ,Top(0)
  ,Right(0)
  ,Bottom(0)
  ,MaxI(0)
  ,MaxJ(0)
{
}
//----------------------------------------------------------------------------------------

TDataPlaneRegular::~TDataPlaneRegular()
{
	if (DataBuffer)
		delete[] DataBuffer;
  DataBuffer = NULL;
}
//----------------------------------------------------------------------------------------

bool	TDataPlaneRegular::Read(FILE * file)
{
	if (!file)
  	return false;

  int version = 0;
  _READFILE(version, file);

 	_READFILE(Left, file);
  _READFILE(Top, file);
  _READFILE(Right, file);
  _READFILE(Bottom, file);
  _READFILE(MaxI, file);
  _READFILE(MaxJ, file);

  int count = MaxI*MaxJ;
  if (count > 0)
  {
  	DataBuffer = new float[count];
    fread(&DataBuffer[0], sizeof(float)*count, 1, file);
  }
	return true;
}
//----------------------------------------------------------------------------------------

bool	TDataPlaneRegular::Write(FILE * file)
{
	if (!file)
  	return false;

  int version = 1;
  _WRITEFILE(version, file);

 	_WRITEFILE(Left, file);
  _WRITEFILE(Top, file);
  _WRITEFILE(Right, file);
  _WRITEFILE(Bottom, file);
  _WRITEFILE(MaxI, file);
  _WRITEFILE(MaxJ, file);

  int count = MaxI*MaxJ;
  if (count > 0)
  {
  	fwrite(&DataBuffer[0], sizeof(float)*count, 1, file);
  }

  return true;
}
//----------------------------------------------------------------------------------------

TDataPlaneIterator* TDataPlaneRegular::CreateIterator() const
{
	return new TDataPlaneRegularIterator(this);
}
//----------------------------------------------------------------------------------------

TDataPlaneIterator* TDataPlaneRegular::CreateBoundedIterator(double minValue, double maxValue) const
{
	return new TDataPlaneRegularIterator(this, minValue, maxValue);
}
//----------------------------------------------------------------------------------------

const float	&   TDataPlaneRegular::GetValue(int index)  const
{
	return DataBuffer[index];
}

int							TDataPlaneRegular::GetCount() 										 const
{
	return MaxI*MaxJ;
}
//----------------------------------------------------------------------------------------




////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////
TDataPlaneRegularIterator::TDataPlaneRegularIterator(const TDataPlaneRegular * data)
	:CurIndex(-1)
  ,MaxIndex(0)
  ,Data(data)
  ,CellSizeX(0)
  ,CellSizeY(0)
  ,Bounded(false)
  ,MinBoundValue(-DBL_MAX)
  ,MaxBoundValue(DBL_MAX)
{
	if (Data)
  {
		MaxIndex = Data->GetCount();

    CellSizeX = (Data->Right - Data->Left)/(double)Data->MaxI;
    CellSizeY = (Data->Bottom - Data->Top)/(double)Data->MaxJ;
  }
}
//----------------------------------------------------------------------------------------

TDataPlaneRegularIterator::TDataPlaneRegularIterator(const TDataPlaneRegular * data, double minValue, double maxValue)
	:CurIndex(-1)
  ,MaxIndex(0)
  ,Data(data)
  ,CellSizeX(0)
  ,CellSizeY(0)
  ,Bounded(true)
  ,MinBoundValue(minValue)
  ,MaxBoundValue(maxValue)
{
	if (Data)
  {
		MaxIndex = Data->GetCount();

    CellSizeX = (Data->Right - Data->Left)/(double)Data->MaxI;
    CellSizeY = (Data->Bottom - Data->Top)/(double)Data->MaxJ;
  }
}
//----------------------------------------------------------------------------------------

bool											TDataPlaneRegularIterator::Next()
{
	if (!Bounded)
  {
		CurIndex++;
	  if (CurIndex >= MaxIndex)
  		return false;
    return true;
  } else
  {
    while (1)
    {
    	CurIndex++;
    	if (CurIndex >= MaxIndex)
  			return false;

	  	const float & value = Data->GetValue(CurIndex);
      if (IsGreaterEqual(value, MinBoundValue, EPSILON) && IsLessEqual(value, MaxBoundValue, EPSILON))
      	return true;
    }
    return true;
  }
}
//----------------------------------------------------------------------------------------


const TDataPlaneElement    &   TDataPlaneRegularIterator::Current()
{
	static TDataPlaneElement elem;
  if (!Data)
  	return elem;

  int x = CurIndex / Data->MaxJ;
  int y = CurIndex % Data->MaxJ;

  elem.Left    = Data->Left + CellSizeX*x;
  elem.Top     = Data->Top  + CellSizeY*y;
  elem.Right   = elem.Left + CellSizeX;
  elem.Bottom  = elem.Top  + CellSizeY;
  elem.Value   = Data->GetValue(CurIndex);

  return elem;
}
//----------------------------------------------------------------------------------------




////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////

bool	TDataPlaneQuadTree::Read(FILE * file)
{
	if (!file)
  	return false;
  int version = 0;
  _READFILE(version, file);

  int count = 0;
  _READFILE(count, file);
  Data.resize(count);
  for (COUNT i = 0, in = count; i < in; i++)
    Data[i].Read(file);

	return true;
}

bool	TDataPlaneQuadTree::Write(FILE * file)
{
	if (!file)
  	return false;
  int version = 2;
  _WRITEFILE(version, file);

  int count = Data.size();
  _WRITEFILE(count, file);

  for (COUNT i = 0, in = count; i < in; i++)
    Data[i].Write(file);

	return true;
}

TDataPlaneIterator* TDataPlaneQuadTree::CreateIterator() const
{
	return new TDataPlaneQuadTreeIterator(this);
}

TDataPlaneIterator* TDataPlaneQuadTree::CreateBoundedIterator(double minValue, double maxValue) const
{
	return new TDataPlaneQuadTreeIterator(this, minValue, maxValue);
}


////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////
TDataPlaneQuadTreeIterator::TDataPlaneQuadTreeIterator(const TDataPlaneQuadTree * data)
	:CurIndex(-1)
  ,MaxIndex(0)
  ,Data(data)
  ,Bounded(false)
  ,MinBoundValue(-DBL_MAX)
  ,MaxBoundValue(DBL_MAX)
{
	if (Data)
  {
		MaxIndex = Data->Data.size();
  }
}
//----------------------------------------------------------------------------------------

TDataPlaneQuadTreeIterator::TDataPlaneQuadTreeIterator(const TDataPlaneQuadTree * data, double minValue, double maxValue)
	:CurIndex(-1)
  ,MaxIndex(0)
  ,Data(data)
  ,Bounded(true)
  ,MinBoundValue(minValue)
  ,MaxBoundValue(maxValue)
{
	if (Data)
  {
		MaxIndex = Data->Data.size();
  }
}
//----------------------------------------------------------------------------------------


bool											TDataPlaneQuadTreeIterator::Next()
{
	if (!Bounded)
  {
		CurIndex++;
	  if (CurIndex >= MaxIndex)
  		return false;
    return true;
  } else
  {
    while (1)
    {
    	CurIndex++;
    	if (CurIndex >= MaxIndex)
  			return false;

      double value = Data->Data[CurIndex].Value;
      if (IsGreaterEqual(value, MinBoundValue, EPSILON) && IsLessEqual(value, MaxBoundValue, EPSILON))
      	return true;
    }
    return true;
  }
}
//----------------------------------------------------------------------------------------


const TDataPlaneElement    &   TDataPlaneQuadTreeIterator::Current()
{
	static TDataPlaneElement elem;
  if (!Data)
  	return elem;

  if (CurIndex >= MaxIndex)
  	elem;

  return Data->Data[CurIndex];
}
//----------------------------------------------------------------------------------------

//*/
