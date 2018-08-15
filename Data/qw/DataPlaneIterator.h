//---------------------------------------------------------------------------

#ifndef DataPlaneIteratorH
#define DataPlaneIteratorH
//---------------------------------------------------------------------------

class TDataPlaneIterator;

class TDataPlaneElement
{
public:
	double Left, Top, Right, Bottom;
  double Value;
};

class TDataPlane
{
	friend class TDataPlaneIterator;
  friend class TDataPlaneConverter;

  float *	DataBuffer;
  double	Left, Top, Right, Bottom;
  int			MaxI, MaxJ;

public:
	TDataPlane();
  ~TDataPlane();

  bool	Read(FILE * file);
  bool	Write(FILE * file);

	TDataPlaneIterator* CreateIterator() const;

  int							GetCount() 					 const;
  const float	&   GetValue(int index)  const;
};

class PACKAGE	TDataPlaneIterator
{
	friend class TDataPlane;
private:

  const TDataPlane * Data;

	TDataPlaneIterator(const TDataPlane * data);

  int		MaxIndex;
  int   CurIndex;

  double	CellSizeX, CellSizeY;

public:

  bool													 Next();

  const TDataPlaneElement    &   Current();
};

#endif

