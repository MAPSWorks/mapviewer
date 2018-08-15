#include "astro.datum.hpp"
#include <math.h>

astro::DATUMEX astro::DatumEx(const astro::DATUM & datum)
{
	astro::DATUMEX dex;

	dex.pri = datum;

	dex.sec.dRp = datum.dRe * (1.0 - datum.dF);
	dex.sec.dESq = datum.dF * (2.0 - datum.dF);
	dex.sec.d1mF = 1.0 - datum.dF;
	dex.sec.d1mESq = 1.0 - dex.sec.dESq;
	dex.sec.dC20Un = dex.pri.dC20 * sqrt(5.0);

	return dex;
}

const astro::DATUMEX & astro::WGS84(void)
{
	static const astro::DATUMEX dexWGS84 = DatumEx(dWGS84);

	return dexWGS84;	
}

const astro::DATUMEX & astro::PZ90(void)
{
	static const astro::DATUMEX dexPZ90 = DatumEx(dPZ90);

	return dexPZ90;	
}
