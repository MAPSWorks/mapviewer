#ifndef __COORDINATES_HPP__
#define __COORDINATES_HPP__

#include "algebra.vectors.hpp"
#include "astro.datum.hpp"

namespace astro
{
	// coordinates in km !!!
	algebra::V3D & FromECEFToGeodetic(const DATUMEX & dex, const algebra::V3D & vECEF, algebra::V3D & vGeo);

	// coordinates in km !!!
	algebra::V3D & FromGeodeticToECEF(const DATUMEX & dex, const algebra::V3D & vGeo, algebra::V3D & vECEF);


	inline double EarthHeightFromDir
	(
		const DATUMEX & dex,
		const algebra::V3D & vDir
	)
	{
		return dex.pri.dRe * (1.0 - dex.pri.dF * vDir.dec.z * vDir.dec.z / LenSq(vDir));
	}

	inline algebra::V3D & VelocityFromGSKToASK
	(
		const DATUMEX & dex,
		const algebra::V3D & vPos,
		const algebra::V3D & vGSK,
		algebra::V3D & vASK
	)
	{
		vASK.dec.x = vGSK.dec.x - dex.pri.dOmega * vPos.dec.y;
		vASK.dec.y = vGSK.dec.y + dex.pri.dOmega * vPos.dec.x;
		vASK.dec.z = vGSK.dec.z;

		return vASK;
	}

	typedef struct
	{
		double r;
		double v;
		double a;
		double e;
		double p;
		double i;
		double OmegaVy;
		double OmegaPi;
		double u;
		double Teta;
		double Anomaly;
	}
	KEPLER;

	KEPLER & FromECEFGSKToKeplerASK(const DATUMEX & dex, const double & dGAST, const algebra::V6D & c, KEPLER & k);

	algebra::V3D & SphereIntersect(algebra::V3D p, algebra::V3D n, algebra::V3D & e);

	void GetQEQABCD(const DATUMEX & dex, const algebra::V3D & p, const algebra::V3D & n, double & a, double & b, double & c, double & d);

	algebra::V3D & EllipsoidIntersect(const DATUMEX & dex, const algebra::V3D & p, const algebra::V3D & n, algebra::V3D & e);
	algebra::V3D & FromECEFToGeodeticWoH(const DATUMEX & dex, const algebra::V3D & e, algebra::V3D & g);
	double PeriodDraconicFromECEFGSK(const DATUMEX & dex, const algebra::V6D & c);

	algebra::V3D & From3DtoGeographic85(const algebra::V3D & e, algebra::V3D & g, double minlon, double maxlon);

    ///	Из документа Datum Transformations of GPS Positions Application Note
	/// Converting ECEF Velocities to Local Tangent Plane Velocities
	algebra::V3D & VelocityECEFToNED(const algebra::V3D geopos, const algebra::V3D vecef, algebra::V3D & vned);

    /// /Docs/Geodetic/tspi.pdf
    /// NED - North East Down
    /// RAE - Range Azimuth Elevation
    /// rae - радианы и метры
    /// vned- метры
	algebra::V3D & RAEToNED(const algebra::V3D rae, algebra::V3D & ned);

    /// На будущее, может понадобится =>
    /// Rotation matrix to convert from ENU to NED coordinates
    /// rotation = [0 1 0; 1 0 0; 0 0 -1];


    /// geopos - геодезические координаты точки отсчета(относительно которой заданы NED) (fi(рад), la(рад), h(м))
    /// ned - прямоугольные, метры
    /// ecef- прямоугольные, метры
    algebra::V3D & NEDtoECEF(const algebra::V3D geopos, const algebra::V3D ned, algebra::V3D & ecef);

}

#endif // __COORDINATES_HPP__
