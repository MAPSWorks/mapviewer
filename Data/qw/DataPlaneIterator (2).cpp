//---------------------------------------------------------------------------


//#pragma hdrstop

#include "DataPlaneIterator.h"

//----------------------------------------------------------------------------------------

//#pragma package(smart_init)


//#define EXCLUDE_READWRITE

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


TDataPlane::TDataPlane()
	:DataBuffer(NULL)
{
}
//----------------------------------------------------------------------------------------

TDataPlane::~TDataPlane()
{
	if (DataBuffer)
		delete[] DataBuffer;
  DataBuffer = NULL;
}
//----------------------------------------------------------------------------------------

bool	TDataPlane::Read(FILE * file)
{
	if (!file)
  	return false;

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

bool	TDataPlane::Write(FILE * file)
{
	if (!file)
  	return false;

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

TDataPlaneIterator* TDataPlane::CreateIterator() const
{
	return new TDataPlaneIterator(this);
}
//----------------------------------------------------------------------------------------

const float	&   TDataPlane::GetValue(int index)  const
{
	return DataBuffer[index];
}

int							TDataPlane::GetCount() 										 const
{
	return MaxI*MaxJ;
}
//----------------------------------------------------------------------------------------

TDataPlaneIterator::TDataPlaneIterator(const TDataPlane * data)
	:CurIndex(-1)
  ,MaxIndex(0)
  ,Data(data)
  ,CellSizeX(0)
  ,CellSizeY(0)
{
	if (Data)
  {
		MaxIndex = Data->GetCount();

    CellSizeX = (Data->Right - Data->Left)/(double)Data->MaxI;
    CellSizeY = (Data->Bottom - Data->Top)/(double)Data->MaxJ;
  }
}
//----------------------------------------------------------------------------------------

bool											TDataPlaneIterator::Next()
{
	CurIndex++;
  if (CurIndex >= MaxIndex)
  	return false;
}
//----------------------------------------------------------------------------------------


/*sim::TPoint2I SET_DIM2::GetPosition(int index) const
{
  return sim::TPoint2I(index / MaxJ, index % MaxJ);
}*/

const TDataPlaneElement    &   TDataPlaneIterator::Current()
{
	/*static TDataPlaneElement elem;
  if (!Data)
  	return elem;

  int x = CurIndex / Data->MaxJ;
  int y = CurIndex % Data->MaxJ;

  elem.Left    = Data->Left + CellSizeX*x;
  elem.Top     = Data->Top  + CellSizeY*y;
  elem.Right   = elem.Left + CellSizeX;
  elem.Bottom  = elem.Top  + CellSizeY;
  elem.Value   = Data->GetValue(CurIndex);

  return elem;*/
	static TDataPlaneElement elem;
	if (!Data)
		return elem;

	int x = CurIndex / Data->MaxJ;
	int y = CurIndex % Data->MaxJ;

	elem.Left    = Data->Left - CellSizeX*x;
	elem.Top     = Data->Top  + CellSizeY*y;
	elem.Right   = elem.Left - CellSizeX;
	elem.Bottom  = elem.Top  + CellSizeY;
	elem.Value   = Data->GetValue(CurIndex);

	return elem;
}
//----------------------------------------------------------------------------------------


