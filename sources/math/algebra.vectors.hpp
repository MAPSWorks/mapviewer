#ifndef __VECTORS_HPP__
#define __VECTORS_HPP__

#include <math.h>

namespace algebra
{

////////////////////////////////////////////////////////////////////////////////
// Misc

	#define	TINY		1e-27

////////////////////////////////////////////////////////////////////////////////
// Vector2D

	#pragma pack(4)

	template<class float_t> struct VEC2
	{
		union
		{
			struct
			{
				float_t x;
				float_t y;
			}
				dec;

			struct
			{
				float_t s;
				float_t t;
			}
				tex;

			struct
			{
				float_t _11;
				float_t _12;
			}
				mat;

			struct
			{
				float_t b;
				float_t l;
			}
				geo;

			float_t a[2];
		};

		operator float_t*() { return a;}
		operator const float_t*() const { return a;}
	};

	#pragma pack()

////////////////////////////////////////////////////////////////////////////////
// Vector3D
	#pragma pack(16)

	template<class float_t> struct VEC3
	{
		union
		{
			struct
			{
				float_t x;
				float_t y;
				float_t z;
			}
				dec;

			struct
			{
				float_t s;
				float_t t;
				float_t q;
			}
				tex;

			struct
			{
				float_t _11;
				float_t _12;
				float_t _13;
			}
				mat;

			struct
			{
				float_t l;
				float_t b;
				float_t h;
			}
				geo;

            struct
			{
				float_t r;
				float_t a;
				float_t e;
			}
				rae;

            struct
			{
				float_t n;
				float_t e;
				float_t d;
			}
				ned;

			float_t a[3];

			VEC2<float_t> v2;
		};

		operator float_t*() { return a;}
		operator const float_t*() const { return a;}
	};

	#pragma pack()

////////////////////////////////////////////////////////////////////////////////
// Vector4D

	#pragma pack(4)

	template<class float_t> struct VEC4
	{
		union
		{
			struct
			{
				float_t x;
				float_t y;
				float_t z;
				float_t w;
			}
				dec;

			struct
			{
				float_t s;
				float_t t;
				float_t q;
				float_t r;
			}
				tex;

			struct
			{
				float_t _11;
				float_t _12;
				float_t _13;
				float_t _14;
			}
				mat;

			float_t a[4];

			VEC3<float_t> v3;
		};

		operator float_t*() { return a;}
		operator const float_t*() const { return a;}
	};

	#pragma pack()

////////////////////////////////////////////////////////////////////////////////
// Vector6D

	#pragma pack(4)

	template<class float_t> struct VEC6
	{
		union
		{
			struct
			{
				float_t x;
				float_t y;
				float_t z;
				float_t vx;
				float_t vy;
				float_t vz;
			}
				dec;

			struct
			{
				float_t _11;
				float_t _12;
				float_t _13;
				float_t _14;
				float_t _15;
				float_t _16;
			}
				mat;

			float_t a[6];

			struct
			{
				VEC3<float_t> p;
				VEC3<float_t> v;
			}
				para;

			struct
			{
				float_t r;
				float_t v;
				float_t t;
				float_t i;
				float_t o;
				float_t u;
			}
				kep;
		};

		operator float_t*() { return a;}
		operator const float_t*() const { return a;}
	};

	#pragma pack()

////////////////////////////////////////////////////////////////////////////////
// Matrix3x3

	#pragma pack(4)

	template<class float_t> struct MAT3
	{
		union
		{
			struct
			{
				float_t _11, _12, _13;
				float_t _21, _22, _23;
				float_t _31, _32, _33;
			}
				mat;

			struct
			{
				VEC3<float_t> left;
				VEC3<float_t> up;
				VEC3<float_t> front;
			}
				v3;

			float_t m[3][3];
			float_t a[9];
		};

		operator float_t*() { return a;}
		operator const float_t*() const { return a;}
	};

	#pragma pack()

////////////////////////////////////////////////////////////////////////////////
// Matrix4x4

	#pragma pack(4)

	template<class float_t> struct MAT4
	{
		union
		{
			struct
			{
				float_t _11, _12, _13, _14;
				float_t _21, _22, _23, _24;
				float_t _31, _32, _33, _34;
				float_t _41, _42, _43, _44;
			}
				mat;

			struct
			{
				VEC3<float_t> left;
				float_t w1;
				VEC3<float_t> up;
				float_t w2;
				VEC3<float_t> front;
				float_t w3;
				VEC3<float_t> pos;
				float_t w4;
			}
				v3;

			struct
			{
				VEC4<float_t> _1;
				VEC4<float_t> _2;
				VEC4<float_t> _3;
				VEC4<float_t> _4;
			}
				v4;

			float_t m[4][4];
			float_t a[16];
		};

		operator float_t*() { return a;}
		operator const float_t*() const { return a;}
	};

	#pragma pack()

////////////////////////////////////////////////////////////////////////////////
// Vector2D functions

	template<class float_t> VEC2<float_t> & Set
	(
		VEC2<float_t> & a,
		float_t x,
		float_t y
	)
	{
		a.dec.x = x;
		a.dec.y = y;

		return a;
	}

	template<class float_t> float_t Len(const VEC2<float_t> & a)
	{
		return sqrt(a.dec.x * a.dec.x + a.dec.y * a.dec.y);
	}

	template<class float_t> float_t LenSq(const VEC2<float_t> & a)
	{
		return (a.dec.x * a.dec.x + a.dec.y * a.dec.y);
	}

	template<class float_t> VEC2<float_t> & Norm(VEC2<float_t> & a)
	{
		float_t f(LenSq(a));

		if (f <= TINY)
		{
			a.dec.x = 0;
			a.dec.y = 0;

			return a;
		}
		else
		{
			f = 1/sqrt(f);

			a.dec.x *= f;
			a.dec.y *= f;
		}

		return a;
	}

	template<class float_t> float_t Dot
	(
		const VEC2<float_t> & a,
		const VEC2<float_t> & b
	)
	{
		return a.dec.x * b.dec.x + a.dec.y * b.dec.y;
	}

	template<class float_t> VEC2<float_t> & Cross
	(
		const VEC2<float_t> & a,
		const VEC2<float_t> & b,
		VEC2<float_t> & r
	)
	{
		r.dec.x = a.dec.x * b.dec.y - a.dec.y * b.dec.x;
		r.dec.y = r.dec.x;

		return r;
	}

	template<class float_t> float_t Ang(const VEC2<float_t> & a)
	{
		return (float_t) atan2(a.dec.y, a.dec.x);
	}

	template<class float_t> VEC2<float_t> & Rot(VEC2<float_t> & a, float_t ang)
	{
		float_t s = (float_t) sin(ang);
		float_t c = (float_t) cos(ang);
		float_t x(a.dec.x);
		float_t y(a.dec.y);

		a.dec.x = (c * x) - (s * y);
		a.dec.y = (c * y) + (s * x);

		return a;
	}

	template<class float_t> VEC2<float_t> & Add
	(
		const VEC2<float_t> & a,
		const VEC2<float_t> & b,
		VEC2<float_t> & r
	)
	{
		r.dec.x = a.dec.x + b.dec.x;
		r.dec.y = a.dec.y + b.dec.y;

		return r;
	}

	template<class float_t> VEC2<float_t> & Sub
	(
		const VEC2<float_t> & a,
		const VEC2<float_t> & b,
		VEC2<float_t> & r
	)
	{
		r.dec.x = a.dec.x - b.dec.x;
		r.dec.y = a.dec.y - b.dec.y;

		return r;
	}

	template<class float_t> VEC2<float_t> & Mul
	(
		VEC2<float_t> & a,
		float_t b
	)
	{
		a.dec.x *= b;
		a.dec.y *= b;

		return a;
	}

	template<class float_t> VEC2<float_t> & Mul
	(
		VEC2<float_t> & a,
		float_t b,
		VEC2<float_t> & r
	)
	{
		r.dec.x = a.dec.x * b;
		r.dec.y = a.dec.y * b;

		return r;
	}

	template<class float_t> VEC2<float_t> & Mul
	(
		const VEC2<float_t> & a,
		const VEC2<float_t> & b,
		VEC2<float_t> & r
	)
	{
		r.dec.x = a.dec.x * b.dec.x;
		r.dec.y = a.dec.y * b.dec.y;

		return r;
	}

	template<class float_t> VEC2<float_t> & Div
	(
		VEC2<float_t> & a,
		float_t b
	)
	{
		float_t f1_b(1/b);

		a.dec.x *= f1_b;
		a.dec.y *= f1_b;

		return a;
	}

	template<class float_t> VEC2<float_t> & Div
	(
		const VEC2<float_t> & a,
		float_t b,
		VEC2<float_t> & r
	)
	{
		float_t f1_b(1/b);

		r.dec.x = a.dec.x * b;
		r.dec.y = a.dec.y * b;

		return r;
	}

	template<class float_t> VEC2<float_t> & Neg
	(
		VEC2<float_t> & a
	)
	{
		a.dec.x = -a.dec.x;
		a.dec.y = -a.dec.y;

		return a;
	}

	template<class float_t> float_t Ang
	(
		const VEC2<float_t> & a,
		const VEC2<float_t> & b
	)
	{
		float_t d(Len(a) * Len(b));

		if (d == 0)
		{
			return 0;
		}

		return (float_t) acos(Dot(a, b) / d);
	}

////////////////////////////////////////////////////////////////////////////////
// Vector3D functions

	template<class float_t> VEC3<float_t> & Set
	(
		VEC3<float_t> & a,
		float_t x,
		float_t y,
		float_t z
	)
	{
		a.dec.x = x;
		a.dec.y = y;
		a.dec.z = z;

		return a;
	}

	template<class float_t> float_t Len(const VEC3<float_t> & a)
	{
		return sqrt(a.dec.x * a.dec.x + a.dec.y * a.dec.y + a.dec.z * a.dec.z);
	}

	template<class float_t> float_t LenSq(const VEC3<float_t> & a)
	{
		return (a.dec.x * a.dec.x + a.dec.y * a.dec.y + a.dec.z * a.dec.z);
	}

	template<class float_t> VEC3<float_t> & Norm(VEC3<float_t> & a)
	{
		float_t f(LenSq(a));

		if (f <= TINY)
		{
			a.dec.x = 0;
			a.dec.y = 0;
			a.dec.z = 0;

			return a;
		}
		else
		{
			f = 1/sqrt(f);

			a.dec.x *= f;
			a.dec.y *= f;
			a.dec.z *= f;
		}

		return a;
	}

	template<class float_t> VEC3<float_t> & Norm(VEC3<float_t> & a, float_t len)
	{
		float_t f(LenSq(a));

		if (f <= TINY)
		{
			a.dec.x = 0;
			a.dec.y = 0;
			a.dec.z = 0;

			return a;
		}
		else
		{
			f = len/sqrt(f);

			a.dec.x *= f;
			a.dec.y *= f;
			a.dec.z *= f;
		}

		return a;
	}

	template<class float_t> VEC3<float_t> & Add
	(
		const VEC3<float_t> & a,
		const VEC3<float_t> & b,
		VEC3<float_t> & r
	)
	{
		r.dec.x = a.dec.x + b.dec.x;
		r.dec.y = a.dec.y + b.dec.y;
		r.dec.z = a.dec.z + b.dec.z;

		return r;
	}

	template<class float_t> VEC3<float_t> & Add
	(
		VEC3<float_t> & a,
		const VEC3<float_t> & b
	)
	{
		a.dec.x += b.dec.x;
		a.dec.y += b.dec.y;
		a.dec.z += b.dec.z;

		return a;
	}

	template<class float_t> VEC3<float_t> & Sub
	(
		const VEC3<float_t> & a,
		const VEC3<float_t> & b,
		VEC3<float_t> & r
	)
	{
		r.dec.x = a.dec.x - b.dec.x;
		r.dec.y = a.dec.y - b.dec.y;
		r.dec.z = a.dec.z - b.dec.z;

		return r;
	}

	template<class float_t> VEC3<float_t> & Sub
	(
		VEC3<float_t> & a,
		const VEC3<float_t> & b
	)
	{
		a.dec.x -= b.dec.x;
		a.dec.y -= b.dec.y;
		a.dec.z -= b.dec.z;

		return a;
	}

	template<class float_t> VEC3<float_t> & Mul
	(
		VEC3<float_t> & a,
		float_t b
	)
	{
		a.dec.x *= b;
		a.dec.y *= b;
		a.dec.z *= b;

		return a;
	}

	template<class float_t> VEC3<float_t> & Mul
	(
		const VEC3<float_t> & a,
		float_t b,
		VEC3<float_t> & r
	)
	{
		r.dec.x = a.dec.x * b;
		r.dec.y = a.dec.y * b;
		r.dec.z = a.dec.z * b;

		return r;
	}

	template<class float_t> VEC3<float_t> & Div
	(
		VEC3<float_t> & a,
		float_t b
	)
	{
		float_t f1_b(1/b);

		a.dec.x *= b;
		a.dec.y *= b;
		a.dec.z *= b;

		return a;
	}

	template<class float_t> VEC3<float_t> & Neg
	(
		VEC3<float_t> & a
	)
	{
		a.dec.x = -a.dec.x;
		a.dec.y = -a.dec.y;
		a.dec.z = -a.dec.z;

		return a;
	}

	template<class float_t> float_t Dot
	(
		const VEC3<float_t> & a,
		const VEC3<float_t> & b
	)
	{
		return a.dec.x * b.dec.x + a.dec.y * b.dec.y + a.dec.z * b.dec.z;
	}

	template<class float_t> VEC3<float_t> & Cross
	(
		const VEC3<float_t> & a,
		const VEC3<float_t> & b,
		VEC3<float_t> & r
	)
	{
		float_t x(a.dec.y * b.dec.z - a.dec.z * b.dec.y);
		float_t y(a.dec.z * b.dec.x - a.dec.x * b.dec.z);
		float_t z(a.dec.x * b.dec.y - a.dec.y * b.dec.x);

		r.dec.x = x;
		r.dec.y = y;
		r.dec.z = z;

		return r;
	}

	template<class float_t> VEC3<float_t> & Mul
	(
		const VEC3<float_t> & a,
		const MAT3<float_t> & b,
		VEC3<float_t> & r
	)
	{
		float_t x(a.mat._11*b.mat._11 + a.mat._12*b.mat._21 + a.mat._13*b.mat._31);
		float_t y(a.mat._11*b.mat._12 + a.mat._12*b.mat._22 + a.mat._13*b.mat._32);
		float_t z(a.mat._11*b.mat._13 + a.mat._12*b.mat._23 + a.mat._13*b.mat._33);

		r.dec.x = x;
		r.dec.y = y;
		r.dec.z = z;

		return r;
	}

	template<class float_t> VEC3<float_t> & Mul3
	(
		const VEC3<float_t> & a,
		const MAT4<float_t> & b,
		VEC3<float_t> & r
	)
	{
		float_t x(a.mat_11*b.mat_11 + a.mat_12*b.mat_21 + a.mat_13*b.mat_31);
		float_t y(a.mat_11*b.mat_12 + a.mat_12*b.mat_22 + a.mat_13*b.mat_32);
		float_t z(a.mat_11*b.mat_13 + a.mat_12*b.mat_23 + a.mat_13*b.mat_33);

		r.dec.x = x;
		r.dec.y = y;
		r.dec.z = z;

		return r;
	}

	template<class float_t> VEC3<float_t> & Mul4
	(
		const VEC3<float_t> & a,
		const MAT4<float_t> & b,
		VEC3<float_t> & r
	)
	{
		float_t x(a.mat_11*b.mat_11 + a.mat_12*b.mat_21 + a.mat_13*b.mat_31 + b.mat_41);
		float_t y(a.mat_11*b.mat_12 + a.mat_12*b.mat_22 + a.mat_13*b.mat_32 + b.mat_42);
		float_t z(a.mat_11*b.mat_13 + a.mat_12*b.mat_23 + a.mat_13*b.mat_33 + b.mat_43);

		r.dec.x = x;
		r.dec.y = y;
		r.dec.z = z;

		return r;
	}

	// просто перпендикул€рный вектор дл€ заданного
	template<class float_t> VEC3<float_t> & Ortogonal
	(
		const VEC3<float_t> & a,
		VEC3<float_t> & r
	)
	{
		if (a.dec.x < a.dec.y)
		{
			if (a.dec.x < a.dec.z)
			{
				r.dec.x = 0;

				r.dec.y = a.dec.z;
				r.dec.z =-a.dec.y;
			}
			else
			{
				r.dec.z = 0;

				r.dec.x = a.dec.y;
				r.dec.y =-a.dec.x;
			}
		}
		else
		{
			if (a.dec.y < a.dec.z)
			{
				r.dec.y = 0;

				r.dec.x = a.dec.z;
				r.dec.z =-a.dec.x;
			}
			else
			{
				r.dec.z = 0;

				r.dec.x = a.dec.y;
				r.dec.y =-a.dec.x;
			}
		}

		return r;
	}

////////////////////////////////////////////////////////////////////////////////
// Vector4D functions

template<class float_t> VEC4<float_t> & Set
	(
		VEC4<float_t> & a,
		float_t x,
		float_t y,
		float_t z,
		float_t w
	)
	{
		a.dec.x = x;
		a.dec.y = y;
		a.dec.z = z;
		a.dec.w = w;

		return a;
	}

	template<class float_t> float_t Len(const VEC4<float_t> & a)
	{
		return sqrt(a.dec.x * a.dec.x + a.dec.y * a.dec.y + a.dec.z * a.dec.z + a.dec.w * a.dec.w);
	}

	template<class float_t> float_t LenSq(const VEC4<float_t> & a)
	{
		return (a.dec.x * a.dec.x + a.dec.y * a.dec.y + a.dec.z * a.dec.z + a.dec.w * a.dec.w);
	}

	template<class float_t> VEC4<float_t> & Norm(VEC4<float_t> & a)
	{
		float_t f(LenSq(a));

		if (f <= TINY)
		{
			a.dec.x = 0;
			a.dec.y = 0;
			a.dec.z = 0;
			a.dec.w = 1;

			return a;
		}
		else
		{
			f = 1/sqrt(f);

			a.dec.x *= f;
			a.dec.y *= f;
			a.dec.z *= f;
			a.dec.w *= f;
		}

		return a;
	}

	template<class float_t> VEC4<float_t> & Add
	(
		const VEC4<float_t> & a,
		const VEC4<float_t> & b,
		VEC4<float_t> & r
	)
	{
		r.dec.x = a.dec.x + b.dec.x;
		r.dec.y = a.dec.y + b.dec.y;
		r.dec.z = a.dec.z + b.dec.z;
		r.dec.w = a.dec.w + b.dec.w;

		return r;
	}

	template<class float_t> VEC4<float_t> & Sub
	(
		const VEC4<float_t> & a,
		const VEC4<float_t> & b,
		VEC4<float_t> & r
	)
	{
		r.dec.x = a.dec.x - b.dec.x;
		r.dec.y = a.dec.y - b.dec.y;
		r.dec.z = a.dec.z - b.dec.z;
		r.dec.w = a.dec.w - b.dec.w;

		return r;
	}

	template<class float_t> VEC4<float_t> & Mul
	(
		VEC4<float_t> & a,
		float_t b
	)
	{
		a.dec.x *= b;
		a.dec.y *= b;
		a.dec.z *= b;
		a.dec.w *= b;

		return a;
	}

	template<class float_t> VEC4<float_t> & Div
	(
		VEC4<float_t> & a,
		float_t b
	)
	{
		float_t f1_b(1/b);

		a.dec.x *= b;
		a.dec.y *= b;
		a.dec.z *= b;
		a.dec.w *= b;

		return a;
	}

	template<class float_t> VEC4<float_t> & Neg(VEC4<float_t> & a)
	{
		a.dec.x = -a.dec.x;
		a.dec.y = -a.dec.y;
		a.dec.z = -a.dec.z;
		a.dec.w = -a.dec.w;

		return a;
	}

	template<class float_t> float_t Dot
	(
		const VEC4<float_t> & a,
		const VEC4<float_t> & b
	)
	{
		return a.dec.x * b.dec.x + a.dec.y * b.dec.y + a.dec.z * b.dec.z + a.dec.w * b.dec.w;
	}

	template<class float_t> VEC4<float_t> & QuatConj(VEC4<float_t> & a)
	{
		a.dec.x = -a.dec.x;
		a.dec.y = -a.dec.y;
		a.dec.z = -a.dec.z;

		return a;
	}

	template<class float_t> VEC4<float_t> & QuatId(VEC4<float_t> & a)
	{
		a.dec.x = 0;
		a.dec.y = 0;
		a.dec.z = 0;
		a.dec.w = 1;

		return a;
	}

	template<class float_t> VEC4<float_t> & QuatMul
	(
		const VEC4<float_t> & a,
		const VEC4<float_t> & b,
		VEC4<float_t> & r
	)
	{
		float_t f_0((a.dec.z - a.dec.y) * (b.dec.y - b.dec.z));
		float_t f_1((a.dec.w + a.dec.x) * (b.dec.w + b.dec.x));
		float_t f_2((a.dec.w - a.dec.x) * (b.dec.y + b.dec.z));
		float_t f_3((a.dec.y + a.dec.z) * (b.dec.w - b.dec.x));
		float_t f_4((a.dec.z - a.dec.x) * (b.dec.x - b.dec.y));
		float_t f_5((a.dec.z + a.dec.x) * (b.dec.x + b.dec.y));
		float_t f_6((a.dec.w + a.dec.y) * (b.dec.w - b.dec.z));
		float_t f_7((a.dec.w - a.dec.y) * (b.dec.w + b.dec.z));
		float_t f_8(f_5 + f_6 + f_7);
		float_t f_9((float_t)0.5 * (f_4 + f_8));

		r.dec.x = f_1 + f_9 - f_8;
		r.dec.y = f_2 + f_9 - f_7;
		r.dec.z = f_3 + f_9 - f_6;
		r.dec.w = f_0 + f_9 - f_5;

		return r;
	}

	template<class float_t> VEC4<float_t> & QuatFromVecAng
	(
		const VEC3<float_t> & vDir,
		float_t fAng,
		VEC4<float_t> & qRes
	)
	{
		fAng *= (float_t) 0.5;

		float_t fSin_2 = sin(fAng);

		qRes.dec.x = vDir.dec.x * fSin_2;
		qRes.dec.y = vDir.dec.y * fSin_2;
		qRes.dec.z = vDir.dec.z * fSin_2;

		qRes.dec.w = cos(fAng);

		return qRes;
	}

	template<class float_t> VEC4<float_t> & QuatFromVecVec
	(
		const VEC3<float_t> & vFrom,
		const VEC3<float_t> & vTo,
		VEC4<float_t> & qRes
	)
	{

		qRes.dec.x = (vFrom.dec.y * vTo.dec.z - vFrom.dec.z * vTo.dec.y);
		qRes.dec.y = (vFrom.dec.z * vTo.dec.x - vFrom.dec.x * vTo.dec.z);
		qRes.dec.z = (vFrom.dec.x * vTo.dec.y - vFrom.dec.y * vTo.dec.x);
		qRes.dec.w = vFrom.dec.x * vTo.dec.x + vFrom.dec.y * vTo.dec.y + vFrom.dec.z * vTo.dec.z;

		Norm(qRes);

		qRes.dec.w += 1;

		if( fabs(qRes.dec.w) <= TINY ) // angle close to PI
		{
			if( fabs(vFrom.dec.z) > fabs(vFrom.dec.x))
			{
				qRes.dec.x = 0;
				qRes.dec.y = vFrom.dec.z;
				qRes.dec.z =-vFrom.dec.y;
			}
			else
			{
				qRes.dec.x = vFrom.dec.y;
				qRes.dec.y =-vFrom.dec.x;
				qRes.dec.z = 0;
			}
		}

		return qRes;
	}

////////////////////////////////////////////////////////////////////////////////
// Vector6D functions

	template<class float_t> VEC6<float_t> & Add
	(
		const VEC6<float_t> & a,
		const VEC6<float_t> & b,
		VEC6<float_t> & r
	)
	{
		r.dec.x = a.dec.x + b.dec.x;
		r.dec.y = a.dec.y + b.dec.y;
		r.dec.z = a.dec.z + b.dec.z;
		r.dec.vx = a.dec.vx + b.dec.vx;
		r.dec.vy = a.dec.vy + b.dec.vy;
		r.dec.vz = a.dec.vz + b.dec.vz;

		return r;
	}

	template<class float_t> VEC6<float_t> & Add
	(
		VEC6<float_t> & a,
		const VEC6<float_t> & b
	)
	{
		a.dec.x += b.dec.x;
		a.dec.y += b.dec.y;
		a.dec.z += b.dec.z;
		a.dec.vx += b.dec.vx;
		a.dec.vy += b.dec.vy;
		a.dec.vz += b.dec.vz;

		return a;
	}

	template<class float_t> VEC6<float_t> & Sub
	(
		const VEC6<float_t> & a,
		const VEC6<float_t> & b,
		VEC6<float_t> & r
	)
	{
		r.dec.x = a.dec.x - b.dec.x;
		r.dec.y = a.dec.y - b.dec.y;
		r.dec.z = a.dec.z - b.dec.z;
		r.dec.vx = a.dec.vx - b.dec.vx;
		r.dec.vy = a.dec.vy - b.dec.vy;
		r.dec.vz = a.dec.vz - b.dec.vz;

		return r;
	}

	template<class float_t> VEC6<float_t> & Mul
	(
		VEC6<float_t> & a,
		float_t b
	)
	{
		a.dec.x *= b;
		a.dec.y *= b;
		a.dec.z *= b;
		a.dec.vx *= b;
		a.dec.vy *= b;
		a.dec.vz *= b;

		return a;
	}

	template<class float_t> VEC6<float_t> & Mul
	(
		const VEC6<float_t> & a,
		float_t b,
		VEC6<float_t> & r
	)
	{
		r.dec.x = a.dec.x * b;
		r.dec.y = a.dec.y * b;
		r.dec.z = a.dec.z * b;
		r.dec.vx = a.dec.vx * b;
		r.dec.vy = a.dec.vy * b;
		r.dec.vz = a.dec.vz * b;

		return r;
	}

	template<class float_t> VEC6<float_t> & MulAdd
	(
		const VEC6<float_t> & a,
		float_t b,
		VEC6<float_t> & r
	)
	{
		r.dec.x += a.dec.x * b;
		r.dec.y += a.dec.y * b;
		r.dec.z += a.dec.z * b;
		r.dec.vx += a.dec.vx * b;
		r.dec.vy += a.dec.vy * b;
		r.dec.vz += a.dec.vz * b;

		return r;
	}

////////////////////////////////////////////////////////////////////////////////
// Matrix3x3 functions

	template<class float_t> MAT3<float_t> & Tp(MAT3<float_t> & a)
	{
		float_t f;

		f		= a.mat._12;
		a.mat._12	= a.mat._21;
		a.mat._21	= f;

		f		= a.mat._13;
		a.mat._13	= a.mat._31;
		a.mat._31	= f;

		f		= a.mat._23;
		a.mat._23	= a.mat._32;
		a.mat._32	= f;

		return a;
	}

	template<class float_t> MAT3<float_t> & Id(MAT3<float_t> & a)
	{
		a.mat._11 = a.mat._22 = a.mat._33 = 1;
		a.mat._12 = a.mat._13 = a.mat._21 = a.mat._23 = a.mat._31 = a.mat._32 = 0;

		return a;
	}

	template<class float_t> MAT3<float_t> & Rx(MAT3<float_t> & a, float_t fAng)
	{
		a.mat._11 = 1;
		a.mat._12 = 0;
		a.mat._13 = 0;

		a.mat._21 = 0;
		a.mat._22 = (float_t)cos(fAng);
		a.mat._23 = (float_t)sin(fAng);

		a.mat._31 = 0;
		a.mat._32 =-a.mat._23;
		a.mat._33 = a.mat._22;

		return a;
	}

	template<class float_t> MAT3<float_t> & Ry(MAT3<float_t> & a, float_t fAng)
	{
		a.mat._11 = cos(fAng);
		a.mat._12 = 0;
		a.mat._13 =-sin(fAng);

		a.mat._21 = 0;
		a.mat._22 = 1;
		a.mat._23 = 0;

		a.mat._31 =-a.mat._13;
		a.mat._32 = 0;
		a.mat._33 = a.mat._11;

		return a;
	}

	template<class float_t> MAT3<float_t> & Rz(MAT3<float_t> & a, float_t fAng)
	{
		a.mat._11 = (float_t)cos(fAng);
		a.mat._12 = (float_t)sin(fAng);
		a.mat._13 = 0;

		a.mat._21 =-a.mat._12;
		a.mat._22 = a.mat._11;
		a.mat._23 = 0;

		a.mat._31 = 0;
		a.mat._32 = 0;
		a.mat._33 = 1;

		return a;
	}

	template<class float_t> MAT3<float_t> & Mul
	(
		const MAT3<float_t> & a,
		const MAT3<float_t> & b,
		MAT3<float_t> & r
	)
	{
		float_t _11(a.mat._11 * b.mat._11 + a.mat._12 * b.mat._21 + a.mat._13 * b.mat._31);
		float_t _12(a.mat._11 * b.mat._12 + a.mat._12 * b.mat._22 + a.mat._13 * b.mat._32);
		float_t _13(a.mat._11 * b.mat._13 + a.mat._12 * b.mat._23 + a.mat._13 * b.mat._33);

		float_t _21(a.mat._21 * b.mat._11 + a.mat._22 * b.mat._21 + a.mat._23 * b.mat._31);
		float_t _22(a.mat._21 * b.mat._12 + a.mat._22 * b.mat._22 + a.mat._23 * b.mat._32);
		float_t _23(a.mat._21 * b.mat._13 + a.mat._22 * b.mat._23 + a.mat._23 * b.mat._33);

		float_t _31(a.mat._31 * b.mat._11 + a.mat._32 * b.mat._21 + a.mat._33 * b.mat._31);
		float_t _32(a.mat._31 * b.mat._12 + a.mat._32 * b.mat._22 + a.mat._33 * b.mat._32);
		float_t _33(a.mat._31 * b.mat._13 + a.mat._32 * b.mat._23 + a.mat._33 * b.mat._33);

		r.mat._11 = _11; r.mat._12 = _12; r.mat._13 = _13;
		r.mat._21 = _21; r.mat._22 = _22; r.mat._23 = _23;
		r.mat._31 = _31; r.mat._32 = _32; r.mat._33 = _33;

		return r;
	}


	template<class float_t> MAT3<float_t> & Rq // from quaternion
	(
		const VEC4<float_t> & a,
		MAT3<float_t> & r
	)
	{
		float_t dx(2. * a.dec.x);
		float_t dy(2. * a.dec.y);
		float_t dz(2. * a.dec.z);
		float_t dwx(dx * a.dec.w);
		float_t dwy(dy * a.dec.w);
		float_t dwz(dz * a.dec.w);
		float_t dxx(dx * a.dec.x);
		float_t dxy(dy * a.dec.x);
		float_t dxz(dz * a.dec.x);
		float_t dyy(dy * a.dec.y);
		float_t dyz(dz * a.dec.y);
		float_t dzz(dz * a.dec.z);

		r.mat._11 = 1.0-(dyy+dzz);
		r.mat._21 = dxy-dwz;
		r.mat._31 = dxz+dwy;
		r.mat._12 = dxy+dwz;
		r.mat._22 = 1.0-(dxx+dzz);
		r.mat._32 = dyz-dwx;
		r.mat._13 = dxz-dwy;
		r.mat._23 = dyz+dwx;
		r.mat._33 = 1.0-(dxx+dyy);

		return r;
	}

////////////////////////////////////////////////////////////////////////////////
// Matrix4x4 functions


	template<class float_t> MAT4<float_t> & Tp3(MAT4<float_t> & a)
	{
		float_t f;

		f		= a.mat._12;
		a.mat._12	= a.mat._21;
		a.mat._21	= f;

		f		= a.mat._13;
		a.mat._13	= a.mat._31;
		a.mat._31	= f;

		f		= a.mat._23;
		a.mat._23	= a.mat._32;
		a.mat._32	= f;

		return a;
	}

	template<class float_t> MAT4<float_t> & Tp4(MAT4<float_t> & a)
	{
		float_t f;

		f		= a.mat._12;
		a.mat._12	= a.mat._21;
		a.mat._21	= f;

		f		= a.mat._13;
		a.mat._13	= a.mat._31;
		a.mat._31	= f;

		f		= a.mat._14;
		a.mat._14	= a.mat._41;
		a.mat._41	= f;

		f		= a.mat._23;
		a.mat._23	= a.mat._32;
		a.mat._32	= f;

		f		= a.mat._24;
		a.mat._24	= a.mat._42;
		a.mat._42	= f;

		f		= a.mat._34;
		a.mat._34	= a.mat._43;
		a.mat._43	= f;

		return a;
	}

	template<class float_t> MAT4<float_t> & Id(MAT4<float_t> & a)
	{
		a.mat._12 = a.mat._13 = a.mat._14 =
		a.mat._21 = a.mat._23 = a.mat._24 =
		a.mat._31 = a.mat._32 = a.mat._34 =
		a.mat._41 = a.mat._42 = a.mat._43 = 0;

		a.mat._11 = a.mat._22 = a.mat._33 = a.mat._44 = 1;

		return a;
	}

	template<class float_t> MAT4<float_t> & Rx3(MAT4<float_t> & a, float_t fAng)
	{
		a.mat._11 = 1;
		a.mat._12 = 0;
		a.mat._13 = 0;

		a.mat._21 = 0;
		a.mat._22 = (float_t)cos(fAng);
		a.mat._23 = (float_t)sin(fAng);

		a.mat._31 = 0;
		a.mat._32 =-a.mat._23;
		a.mat._33 = a.mat._22;

		return a;
	}

	template<class float_t> MAT4<float_t> & Ry3(MAT4<float_t> & a, float_t fAng)
	{
		a.mat._11 = cos(fAng);
		a.mat._12 = 0;
		a.mat._13 =-sin(fAng);

		a.mat._21 = 0;
		a.mat._22 = 1;
		a.mat._23 = 0;

		a.mat._31 =-a.mat._13;
		a.mat._32 = 0;
		a.mat._33 = a.mat._11;

		return a;
	}

	template<class float_t> MAT4<float_t> & Rz3(MAT4<float_t> & a, float_t fAng)
	{
		a.mat._11 = (float_t)cos(fAng);
		a.mat._12 = (float_t)sin(fAng);
		a.mat._13 = 0;

		a.mat._21 =-a.mat._12;
		a.mat._22 = a.mat._11;
		a.mat._23 = 0;

		a.mat._31 = 0;
		a.mat._32 = 0;
		a.mat._33 = 1;

		return a;
	}

	template<class float_t> MAT4<float_t> & Mul
	(
		const MAT4<float_t> & a,
		const MAT4<float_t> & b,
		MAT4<float_t> & r
	)
	{
		r.mat._11 = a.mat._11 * b.mat._11 + a.mat._12 * b.mat._21 + a.mat._13 * b.mat._31 + a.mat._14 * b.mat._41;
		r.mat._12 = a.mat._11 * b.mat._12 + a.mat._12 * b.mat._22 + a.mat._13 * b.mat._32 + a.mat._14 * b.mat._42;
		r.mat._13 = a.mat._11 * b.mat._13 + a.mat._12 * b.mat._23 + a.mat._13 * b.mat._33 + a.mat._14 * b.mat._43;
		r.mat._14 = a.mat._11 * b.mat._14 + a.mat._12 * b.mat._24 + a.mat._13 * b.mat._34 + a.mat._14 * b.mat._44;

		r.mat._21 = a.mat._21 * b.mat._11 + a.mat._22 * b.mat._21 + a.mat._23 * b.mat._31 + a.mat._24 * b.mat._41;
		r.mat._22 = a.mat._21 * b.mat._12 + a.mat._22 * b.mat._22 + a.mat._23 * b.mat._32 + a.mat._24 * b.mat._42;
		r.mat._23 = a.mat._21 * b.mat._13 + a.mat._22 * b.mat._23 + a.mat._23 * b.mat._33 + a.mat._24 * b.mat._43;
		r.mat._24 = a.mat._21 * b.mat._14 + a.mat._22 * b.mat._24 + a.mat._23 * b.mat._34 + a.mat._24 * b.mat._44;

		r.mat._31 = a.mat._31 * b.mat._11 + a.mat._32 * b.mat._21 + a.mat._33 * b.mat._31 + a.mat._34 * b.mat._41;
		r.mat._32 = a.mat._31 * b.mat._12 + a.mat._32 * b.mat._22 + a.mat._33 * b.mat._32 + a.mat._34 * b.mat._42;
		r.mat._33 = a.mat._31 * b.mat._13 + a.mat._32 * b.mat._23 + a.mat._33 * b.mat._33 + a.mat._34 * b.mat._43;
		r.mat._34 = a.mat._31 * b.mat._14 + a.mat._32 * b.mat._24 + a.mat._33 * b.mat._34 + a.mat._34 * b.mat._44;

		r.mat._41 = a.mat._41 * b.mat._11 + a.mat._42 * b.mat._21 + a.mat._43 * b.mat._31 + a.mat._44 * b.mat._41;
		r.mat._42 = a.mat._41 * b.mat._12 + a.mat._42 * b.mat._22 + a.mat._43 * b.mat._32 + a.mat._44 * b.mat._42;
		r.mat._43 = a.mat._41 * b.mat._13 + a.mat._42 * b.mat._23 + a.mat._43 * b.mat._33 + a.mat._44 * b.mat._43;
		r.mat._44 = a.mat._41 * b.mat._14 + a.mat._42 * b.mat._24 + a.mat._43 * b.mat._34 + a.mat._44 * b.mat._44;

		return r;
	}

	template<class float_t> MAT4<float_t> & Rq3 // from quaternion
	(
		const VEC4<float_t> & a,
		MAT4<float_t> & r
	)
	{
		float_t dx(2. * a.dec.x);
		float_t dy(2. * a.dec.y);
		float_t dz(2. * a.dec.z);
		float_t dwx(dx * a.dec.w);
		float_t dwy(dy * a.dec.w);
		float_t dwz(dz * a.dec.w);
		float_t dxx(dx * a.dec.x);
		float_t dxy(dy * a.dec.x);
		float_t dxz(dz * a.dec.x);
		float_t dyy(dy * a.dec.y);
		float_t dyz(dz * a.dec.y);
		float_t dzz(dz * a.dec.z);

		r.mat._11 = 1.0-(dyy+dzz);
		r.mat._21 = dxy-dwz;
		r.mat._31 = dxz+dwy;
		r.mat._12 = dxy+dwz;
		r.mat._22 = 1.0-(dxx+dzz);
		r.mat._32 = dyz-dwx;
		r.mat._13 = dxz-dwy;
		r.mat._23 = dyz+dwx;
		r.mat._33 = 1.0-(dxx+dyy);

		return r;
	}

	template<class float_t> MAT4<float_t> & Iv(MAT4<float_t> & a)
	{
		Tp3(a);

		Mul3(a.v3.pos, a, a.v3.pos);

		Neg(a.v3.pos);

		return a;
	}

////////////////////////////////////////////////////////////////////////////////
//

	typedef VEC2<float> V2F;
	typedef VEC2<double> V2D;

	typedef VEC3<float> V3F;
	typedef VEC3<double> V3D;

	typedef VEC4<float> V4F;
	typedef VEC4<double> V4D;

	typedef VEC6<float> V6F;
	typedef VEC6<double> V6D;

	typedef MAT3<float> M3F;
	typedef MAT3<double> M3D;

	typedef MAT4<float> M4F;
	typedef MAT4<double> M4D;
}

#endif //__VECTORS_HPP__
