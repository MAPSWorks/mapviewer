//---------------------------------------------------------------------------

#ifndef DataPlaneIteratorH
#define DataPlaneIteratorH

#include <cstdio>
#include <vector>
//---------------------------------------------------------------------------

class TDataPlaneIterator;

// элемент матрицы.
class TDataPlaneElement
{
public:
	double Left, Top, Right, Bottom;
  double Value;

  TDataPlaneElement()
  	:Left(0.0)
    ,Top(0.0)
    ,Right(0.0)
    ,Bottom(0.0)
    ,Value(0.0)
  {
  }

  TDataPlaneElement(double left, double top, double right, double bottom, double value)
  	:Left(left)
    ,Top(top)
    ,Right(right)
    ,Bottom(bottom)
    ,Value(value)
  {
  }

  bool operator == (const TDataPlaneElement& other)
  {
  	return true;
  }

  bool Read(FILE * file);

  bool Write(FILE * file);
};


// базовая матрица.
class TDataPlane
{
public:
	TDataPlane()
  {
  }

  virtual ~TDataPlane()
  {
  }

  virtual bool	Read(FILE * file)
  {
  	return false;
  }

  virtual bool	Write(FILE * file)
  {
  	return false;
  }

	virtual	TDataPlaneIterator* CreateIterator() const
  {
  	return NULL;
  }

  virtual	TDataPlaneIterator* CreateBoundedIterator(double minValue, double maxValue) const
  {
  	return NULL;
  }
};


// базовый итератор.
class 	TDataPlaneIterator
{
protected:

	TDataPlaneIterator()
  {
  }

public:

  virtual	bool													 Next()    = 0;
  virtual	const TDataPlaneElement    &   Current() = 0;
};


// регулярная матрица (.dpl)
class TDataPlaneRegular : public TDataPlane
{
public:
  float *	DataBuffer;
  double	Left, Top, Right, Bottom;
  int			MaxI, MaxJ;

	TDataPlaneRegular();

  virtual ~TDataPlaneRegular();

  virtual bool	Read(FILE * file);
  virtual bool	Write(FILE * file);

  virtual	TDataPlaneIterator* CreateIterator() const;
  virtual	TDataPlaneIterator* CreateBoundedIterator(double minValue, double maxValue) const;

  int							GetCount() 					 const;
  const float	&   GetValue(int index)  const;
};


// итератор для регулярной матрицы.
class	TDataPlaneRegularIterator : public TDataPlaneIterator
{
	friend class TDataPlaneRegular;
private:
  int			MaxIndex;
  int   	CurIndex;

  bool		Bounded;
  double 	MinBoundValue;
  double	MaxBoundValue;

  double	CellSizeX, CellSizeY;

  const TDataPlaneRegular * Data;

	TDataPlaneRegularIterator(const TDataPlaneRegular * data);
  TDataPlaneRegularIterator(const TDataPlaneRegular * data, double minValue, double maxValue);

public:

  bool													 Next();
  const TDataPlaneElement    &   Current();
};


// это DataPlane внутри себя держит квадтри в виде списка.
class TDataPlaneQuadTree : public TDataPlane
{
	friend class TDataPlaneIterator;
  friend class TDataPlaneConverter;

public:
	std::vector<TDataPlaneElement>	Data;

	TDataPlaneQuadTree()
  {
  }

  virtual bool	Read(FILE * file);
  virtual bool	Write(FILE * file);

  virtual	TDataPlaneIterator* CreateIterator() const;
  virtual	TDataPlaneIterator* CreateBoundedIterator(double minValue, double maxValue) const;
};

// итератор для quadTree матрицы.
class	TDataPlaneQuadTreeIterator : public TDataPlaneIterator
{
	friend class TDataPlaneQuadTree;
private:
  int			MaxIndex;
  int   	CurIndex;

  bool		Bounded;
  double 	MinBoundValue;
  double	MaxBoundValue;

  const TDataPlaneQuadTree * Data;

	TDataPlaneQuadTreeIterator(const TDataPlaneQuadTree * data);
	TDataPlaneQuadTreeIterator(const TDataPlaneQuadTree * data, double minValue, double maxValue);

public:

  bool													 Next();

  const TDataPlaneElement    &   Current();
};

#endif

//*/
