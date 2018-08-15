#ifndef __DATUM_HPP__
#define __DATUM_HPP__

namespace astro
{
	typedef struct 
	{
		double dF; // flattening
		double dOmega; // rad/s
		double dMu; // km3/s2
		double dRe; // a-equatorial  km		
		double dC20;		
	}
	DATUM;

	typedef struct 
	{		
		DATUM pri;

		struct 
		{
			double dRp; // b-polar  km
			double dESq;
			double d1mF;
			double d1mESq;
			double dC20Un;
		} sec;

		operator const DATUM & ()
		{
			return pri;
		}
	}
	DATUMEX;

	DATUMEX DatumEx(const DATUM &);

////////////////////////////////////////////////////////////////////////////////
// WGS-84 //////////////////////////////////////////////////////////////////////

	const DATUM dWGS84 = 
	{
		1.0 / 298.257223563,
		7.292115e-5,
		398600.5,
		6378.137,	
		-0.00048416685000		
	};

	const DATUM dPZ90 = 
	{
		1.0 / 298.257839303,
		7.292115e-5,
		398600.44,
		6378.136,				
		-0.000484164953				
	};
	
	const DATUMEX & WGS84(void);
	const DATUMEX & PZ90(void);
}


#endif //  __DATUM_HPP__
