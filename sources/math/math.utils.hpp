// Библиотека математических функций и констант ///////////////////////////////

#ifndef __MATH_HPP__
#define __MATH_HPP__

#include <math.h>

namespace math
{

///////////////////////////////////////////////////////////////////////////////
// Константы
	
	///////////////////////////////////////////////////////////////////////////
	// Пи

	const double dPi = 3.1415926535897932384626433832795;

	const double dPiDiv2 = dPi / 2.0;

	const double d2Pi = 2.0 * dPi;

	///////////////////////////////////////////////////////////////////////////
	// Угловые градусы в радианы

	const double dDegToRad = (dPi / 180.0);

	///////////////////////////////////////////////////////////////////////////
	// Угловые минуты в радианы

	const double dMinToRad = (dDegToRad / 60.0);

	///////////////////////////////////////////////////////////////////////////
	// Угловые секунды в радианы

	const double dSecToRad = (dMinToRad / 60.0);

	///////////////////////////////////////////////////////////////////////////
	// Временные секунды в радианы

	const double dTSecToRad = (dPi / 43200.0);
	
///////////////////////////////////////////////////////////////////////////////
// Функции

	///////////////////////////////////////////////////////////////////////////
	// Расчет значения полинома в точке dX с коэффициентами adK

	template<class T> T Polynom(const T * adK, int iKNum, const T & dX)
	{
		T dY(0);
		
		for(--iKNum; iKNum > 0; iKNum--)
		{
			dY = (dY + adK[iKNum]) * dX;
		}

		return (dY + adK[0]);
	}

	template<class T, class U> T lerp(const T & a, const T & b, const U & t) 
	{ 
		return a + (b - a) * t; 
	}

	template<class T> T copysign(const T & val, const T & sign)
	{
		if (sign < 0)
		{
			return -fabs(val);
		}
		else
		{
			return +fabs(val);
		}
	}
}

#endif //__MATH_HPP__
