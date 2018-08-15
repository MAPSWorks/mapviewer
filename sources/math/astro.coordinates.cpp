#include "astro.coordinates.hpp"
#include <math.h>
#include "math.utils.hpp"
#include "basic.utils.hpp"

using namespace algebra;
using namespace basic;

V3D & astro::FromECEFToGeodetic(const DATUMEX & dex, const algebra::V3D & vECEF, algebra::V3D & vGeo)
{
	if ((vECEF.dec.x == 0.0) && (vECEF.dec.y == 0.0))
	{
		vGeo.geo.l = 0.0;

		if (vECEF.dec.z < 0.0)
		{
			vGeo.geo.b = - math::dPiDiv2;
			vGeo.geo.h = - vECEF.dec.z - dex.sec.dRp;
		}
		else
		{
			vGeo.geo.b = + math::dPiDiv2;
			vGeo.geo.h = + vECEF.dec.z - dex.sec.dRp;
		}
	}
	else
	{
		vGeo.geo.l = atan2(vECEF.dec.y, vECEF.dec.x);

		double dTmp;
		double dS = Len(vECEF.v2);
		double dBeta = atan((vECEF.dec.z) / (dex.sec.d1mF * dS));
		double dSin = sin(dBeta);
		double dCos = cos(dBeta);

		vGeo.geo.b = atan((vECEF.dec.z + dex.sec.dESq * dex.sec.dRp * dSin * dSin * dSin / dex.sec.d1mESq) / (dS - dex.sec.dESq * dex.pri.dRe * dCos * dCos * dCos));

		do
		{
			dBeta = atan((dex.sec.d1mF * sin(vGeo.geo.b)) / (cos(vGeo.geo.b)));
			dSin = sin(dBeta);
			dCos = cos(dBeta);
			dTmp = vGeo.geo.b;
			vGeo.geo.b = atan((vECEF.dec.z + dex.sec.dESq * dex.sec.dRp * dSin * dSin * dSin / dex.sec.d1mESq) / (dS - dex.sec.dESq * dex.pri.dRe * dCos * dCos * dCos));

		} while (fabs(vGeo.geo.b - dTmp) > 1e-8);

		dSin = sin(vGeo.geo.b);
		dTmp = dex.pri.dRe / (sqrt(1.0 - dex.sec.dESq * dSin * dSin));
		vGeo.geo.h = dS * cos(vGeo.geo.b) + (vECEF.dec.z + dex.sec.dESq * dTmp * dSin) * dSin - dTmp;
	}

	return vGeo;
}

V3D & astro::FromGeodeticToECEF(const DATUMEX & dex, const algebra::V3D & vGeo, algebra::V3D & vECEF)
{
	double dSinFi = sin(vGeo.geo.b);
	double dN = dex.pri.dRe / (sqrt(1.0 - dex.sec.dESq * dSinFi * dSinFi));

	vECEF.dec.z = (dN * dex.sec.d1mESq + vGeo.geo.h) * dSinFi;
	dN = (dN + vGeo.geo.h) * cos(vGeo.geo.b);
	vECEF.dec.x = dN * cos(vGeo.geo.l);
	vECEF.dec.y = dN * sin(vGeo.geo.l);


	return vECEF;
}

astro::KEPLER & astro::FromECEFGSKToKeplerASK(const DATUMEX & dex, const double & dGAST, const V6D & c, KEPLER & k)
{
	k.r = Len(c.para.p);

	V3D va;

	VelocityFromGSKToASK(dex, c.para.p, c.para.v, va);

	k.v = Len(va);

	double dK = k.r * LenSq(va) / dex.pri.dMu;

	// dK < 2 Orbit is elliptic or circular
	// dK = 2 Orbit is porabolic
	// dK > 2 Orbit is hyperbolic

	k.a = k.r / (2.0 - dK);

	V3D vc;

	Cross(c.para.p, va, vc);

	double dOmegaVyGSK = atan2(vc.dec.x, -vc.dec.y);

	k.OmegaVy = fmod(dOmegaVyGSK + dGAST, math::d2Pi);

	k.i = acos(vc.dec.z / Len(vc));

	double dSinI = sin(k.i);

	k.p = LenSq(vc) / dex.pri.dMu;

	k.e = sqrt(1.0 - k.p / k.a);

	double dSinTeta = Dot(c.para.p, va) / (k.r * k.v);
	double dCosTeta = Len(vc) / (k.r * k.v);

	k.Teta = atan2(dSinTeta, dCosTeta);

	double dSinV = dK * dSinTeta * dCosTeta / k.e;
	double dCosV = (dK * dCosTeta * dCosTeta - 1.0) / k.e;

	k.Anomaly = atan2(dSinV, dCosV);

	double dCosU = (c.dec.x * cos(dOmegaVyGSK) + c.dec.y * sin(dOmegaVyGSK)) / k.r;
	double dSinU = c.dec.z / (k.r * dSinI);

	k.u = atan2(dSinU, dCosU);

	k.OmegaPi = k.u - k.Anomaly;

	return k;
}

void astro::GetQEQABCD(const DATUMEX & dex, const V3D & p, const V3D & n, double & a, double & b, double & c, double & d)
{
	double semia = 1.0 / dex.pri.dRe;
	double semib = 1.0 / dex.pri.dRe;
	double semic = 1.0 / (dex.pri.dRe * (1.0 - dex.pri.dF));

	d = n.dec.x * semia;
	a = d * d;
	d = n.dec.y * semib;
	a+= d * d;
	d = n.dec.z * semic;
	a+= d * d;

	semia *= semia;
	semib *= semib;
	semic *= semic;

	d = n.dec.x * p.dec.x * semia;
	b = d;
	d = n.dec.y * p.dec.y * semib;
	b+= d;
	d = n.dec.z * p.dec.z * semic;
	b+= d;
	b*= 2.0;

	c = p.dec.x * p.dec.x * semia + p.dec.y * p.dec.y * semib + p.dec.z * p.dec.z * semic - 1.0;

	d = b * b - 4 * a * c;
}

V3D & astro::SphereIntersect(V3D p, V3D n, V3D & e)
{
	const double dReAvg = 6371.0;

	double a = n.dec.x * n.dec.x + n.dec.y * n.dec.y + n.dec.z * n.dec.z;
	double b = 2.0 * (n.dec.x * p.dec.x + n.dec.y * p.dec.y + n.dec.z * p.dec.z);
	double c = p.dec.x * p.dec.x + p.dec.y * p.dec.y + p.dec.z * p.dec.z - dReAvg * dReAvg;
	double d = b * b - 4 * a * c;

	if (d < 0)
	{
		/* ???
		V3D m;

		Cross(p, n, m);
		Cross(m, p, m);

		double sina = dReAvg / Len(p);
		e = Norm(p, sina);
		Add(e, Norm(m, sqrt(1.0 - sina * sina)));
		Mul(e, dReAvg);
		*/
		e.dec.x = 0; // временно, пока не найду решение
		e.dec.y = 0;
		e.dec.z = 0;
	}
	else if (d == 0)
	{
		double t = - b / (2.0 * a);

		e.dec.x = p.dec.x + n.dec.x * t;
		e.dec.y = p.dec.y + n.dec.y * t;
		e.dec.z = p.dec.z + n.dec.z * t;

	}
	else if (d > 0)
	{
		d = sqrt(d);

		double t1 = (- b - d) / (2.0 * a);
		double t2 = (- b + d) / (2.0 * a);

		double t = basic::mini<double &>(t1, t2);

		e.dec.x = p.dec.x + n.dec.x * t;
		e.dec.y = p.dec.y + n.dec.y * t;
		e.dec.z = p.dec.z + n.dec.z * t;
	}

	return e;
}

V3D & astro::EllipsoidIntersect(const DATUMEX & dex, const V3D & p, const V3D & n, V3D & e)
{
	double a, b, c, d;

	GetQEQABCD(dex, p, n, a, b, c, d);

	if (d < 0)
	{
		// ??? don't have solution yet !!!
		throw 777;
	}

	if (d == 0)
	{
		double t = - b / (2.0 * a);

		e.dec.x = p.dec.x + n.dec.x * t;
		e.dec.y = p.dec.y + n.dec.y * t;
		e.dec.z = p.dec.z + n.dec.z * t;

	}
	else if (d > 0)
	{
		d = sqrt(d);

		double t1 = (- b - d) / (2.0 * a);
		double t2 = (- b + d) / (2.0 * a);

		double t = basic::mini<double &>(t1, t2);

		e.dec.x = p.dec.x + n.dec.x * t;
		e.dec.y = p.dec.y + n.dec.y * t;
		e.dec.z = p.dec.z + n.dec.z * t;
	}

	return e;
}

// WoH = without height
V3D & astro::FromECEFToGeodeticWoH(const DATUMEX & dex, const V3D & e, V3D & g)
{
	g.geo.h = 0.0;

	if ((e.dec.x == 0.0) && (e.dec.y == 0.0))
	{
		g.geo.l = 0.0;

		if (e.dec.z < 0.0)
		{
			g.geo.b = - math::dPiDiv2;
		}
		else
		{
			g.geo.b = + math::dPiDiv2;
		}
	}
	else
	{
		g.geo.l = atan2(e.dec.y, e.dec.x);

		g.geo.b = atan2(e.dec.z, Len(e.v2) * dex.sec.d1mF * dex.sec.d1mF);
	}

	return g;
}

double astro::PeriodDraconicFromECEFGSK(const DATUMEX & dex, const V6D & c)
{
	double r = Len(c.para.p);

	V3D va;
	VelocityFromGSKToASK(dex, c.para.p, c.para.v, va);
	double v = Len(va);

	double a = (dex.pri.dMu * r) / (2 * dex.pri.dMu - r * v * v);

	V3D vc;
	Cross(c.para.p, va, vc);

	double dSinI2 = sin(acos(vc.dec.z / Len(vc)));
	dSinI2 *= dSinI2;

	double dReDivA2 = dex.pri.dRe / a;
	dReDivA2 *= dReDivA2;

	return math::d2Pi*sqrt(a*a*a/dex.pri.dMu)*(1+1.5*dex.sec.dC20Un*dReDivA2*(3-2.5*dSinI2));
}

V3D & astro::From3DtoGeographic85(const V3D & e, V3D & g, double minlon, double maxlon) // on sphere
{
	static const double d85 = 85.0 * math::dDegToRad;

	g.geo.h = 0;

	if ((e.dec.x == 0.0) && (e.dec.y == 0.0))
	{
		g.geo.l = 0.0;

		if (e.dec.z < 0.0)
		{
			g.geo.b = - d85;
		}
		else
		{
			g.geo.b = + d85;
		}
	}
	else
	{
		g.geo.l = atan2(e.dec.y, e.dec.x);

		g.geo.b = atan2(e.dec.z, Len(e.v2));

		if (g.geo.b > + d85)
		{
			g.geo.b = + d85;
		}
		if (g.geo.b < - d85)
		{
			g.geo.b = - d85;
		}
	}

	if (g.geo.l < minlon)
	{
		if (basic::bwn(minlon, g.geo.l + math::d2Pi, maxlon))
		{
			g.geo.l += math::d2Pi;
		}
	}

	if (g.geo.l > maxlon)
	{
		if (basic::bwn(minlon, g.geo.l - math::d2Pi, maxlon))
		{
			g.geo.l -= math::d2Pi;
		}
	}

	return g;
}

algebra::V3D & astro::VelocityECEFToNED(const algebra::V3D geopos, const algebra::V3D vecef, algebra::V3D & vned)
{
    algebra::M3D m;

    double sf = sin(geopos.geo.b);
    double sl = sin(geopos.geo.l);
    double cf = cos(geopos.geo.b);
    double cl = cos(geopos.geo.l);

    algebra::Set(m.v3.left  , -sf*cl    , -sl   , -cf*cl);
    algebra::Set(m.v3.up    , -sf*sl    , +cl   , -cf*sl);
    algebra::Set(m.v3.front , +cf       , .0    , -sf);

    algebra::Mul(vecef, m, vned);

    return vned;
}

algebra::V3D & astro::RAEToNED(const algebra::V3D rae, algebra::V3D & ned)
{
    double se = sin(rae.rae.e);
    double sd = sin(rae.rae.a);
    double ce = cos(rae.rae.e);
    double cd = cos(rae.rae.a);

    ned.ned.n = +rae.rae.r*ce*cd;
    ned.ned.e = +rae.rae.r*ce*sd;
    ned.ned.d = -rae.rae.r*se;

    return ned;
}

algebra::V3D & astro::NEDtoECEF(const algebra::V3D geopos, const algebra::V3D ned, algebra::V3D & ecef)
{
    algebra::M3D m;

    double sf = sin(geopos.geo.b);
    double sl = sin(geopos.geo.l);
    double cf = cos(geopos.geo.b);
    double cl = cos(geopos.geo.l);

    algebra::Set(m.v3.left  , -sf*cl    , -sf*sl   , +cf);
    algebra::Set(m.v3.up    , -sl       , +cl      , .0);
    algebra::Set(m.v3.front , -cf*cl    , -cf*sl   , -sf);

    algebra::Mul(ned, m, ecef);

    return ecef;
}

