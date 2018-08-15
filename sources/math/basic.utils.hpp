#ifndef __BASIC_HPP__
#define __BASIC_HPP__

#include <iostream>
#include <exception>

#ifndef REPORT_EXPECTED_EXCEPTION
#define REPORT_EXPECTED_EXCEPTION(e) \
	std::cerr \
		<< "E: " << (e).what() \
		<< " F: " << __FILE__ \
		<< " L: " << __LINE__		
#endif

namespace basic
{
	template<class T> T mini(T a,  T b)
	{
		return (a < b) ? a : b;
	}

	template<class T> T mini(T a,  T b,  T c,  T d)
	{
		return mini<T>(mini<T>(a, b), mini<T>(c, d));
	}

	template<class T> T maxi(T a,  T b)
	{
		return (a > b) ? a : b;
	}

	template<class T> T maxi(T a,  T b,  T c,  T d)
	{
		return maxi<T>(maxi<T>(a, b), maxi<T>(c, d));
	}

	template<class T> bool bwn(const T a, const T b, const T c)
	{
		return ((a <= b) && (b <= c));
	}
}

#endif //__BASIC_HPP__
