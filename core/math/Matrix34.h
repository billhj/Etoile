/**
* Copyright(C) 2009-2012                
* @author Jing HUANG
* @file Matrix34.h
* @brief 
* @date 1/2/2011
*/

#pragma once
#include "Vectors.h"

/************************************************************************
*  Homogeneous 3D matrix class
*
*  Rxx[0] Rxx[1] Rxx[2] P[0]	R is the rotation matrix
*  Ryx[0] Ryy[1] Ryy[2] P[1]	D is the position/translation vector
*  Rzx[0] Rzy[1] Rzz[2] P[2]
*  Dx[0]  Dy[1]  Dz[2]  P[3]   P (0, 0, 0, 1)T
*
************************************************************************/

namespace Etoile
{
	template <class T>
	class Matrix34 
	{

	public:
		Vec3<T> a; T pad0;
		Vec3<T> b; T pad1;
		Vec3<T> c; T pad2;
		Vec3<T> d; T pad3;


		/*static const Matrix34 Matrix34::IDENTITY( 1.0, 0.0, 0.0,
		0.0, 1.0, 0.0,
		0.0, 0.0, 1.0,
		0.0, 0.0, 0.0);*/
	public:
		inline Matrix34()
		{
			identity(); 
		}

		inline Matrix34( T ax,T ay,T az,T bx,T by,T bz,
			T cx,T cy,T cz,T dx,T dy,T dz )
		{
			a.x = ax; b.x = bx; c.x = cx; d.x = dx;
			a.y = ay; b.y = by; c.y = cy; d.y = dy;
			a.z = az; b.z = bz; c.z = cz; d.z = dz;
			pad0 = pad1 = pad2 = 0.0; 
			pad3 = 1.0;
		}

		inline Matrix34( const T* const m)
		{
			memcpy(&a, m, 16 * sizeof(T));
		}

		inline void identity()
		{
			a[0] = 1.0;  a[1] = a[2] = pad0 = 0; 
			b[1] = 1.0;  b[0] = b[2] = pad1 = 0;
			c[2] = 1.0;  c[0] = c[1] = pad2 = 0;
			pad3 = 1.0; d[0] = d[1] = d[2] = 0;
		}

		inline operator T*()
		{
			return (T*)this;
		}

		inline operator const T*() const
		{
			return (T*)this;
		}

		//Arithmetic operations
		inline const Matrix34 operator * ( const Matrix34& rhs ) const
		{
			Matrix34 lhs;

			lhs.a.x = a.x * rhs.a.x + a.y * rhs.b.x + a.z * rhs.c.x;
			lhs.a.y = a.x * rhs.a.y + a.y * rhs.b.y + a.z * rhs.c.y;
			lhs.a.z = a.x * rhs.a.z + a.y * rhs.b.z + a.z * rhs.c.z;

			lhs.b.x = b.x * rhs.a.x + b.y * rhs.b.x + b.z * rhs.c.x;
			lhs.b.y = b.x * rhs.a.y + b.y * rhs.b.y + b.z * rhs.c.y;
			lhs.b.z = b.x * rhs.a.z + b.y * rhs.b.z + b.z * rhs.c.z;

			lhs.c.x = c.x * rhs.a.x + c.y * rhs.b.x + c.z * rhs.c.x;
			lhs.c.y = c.x * rhs.a.y + c.y * rhs.b.y + c.z * rhs.c.y;
			lhs.c.z = c.x * rhs.a.z + c.y * rhs.b.z + c.z * rhs.c.z;

			lhs.d.x = d.x * rhs.a.x + d.y * rhs.b.x + d.z * rhs.c.x + rhs.d.x;
			lhs.d.y = d.x * rhs.a.y + d.y * rhs.b.y + d.z * rhs.c.y + rhs.d.y;
			lhs.d.z = d.x * rhs.a.z + d.y * rhs.b.z + d.z * rhs.c.z + rhs.d.z;

			return lhs;
		}

		inline const Matrix34 operator - ( const Matrix34& rhs ) const
		{
			Matrix34 lhs;

			lhs.a = a - rhs.a; lhs.pad0 -= rhs.pad0;
			lhs.b = b - rhs.b; lhs.pad1 -= rhs.pad1;
			lhs.c = c - rhs.c; lhs.pad2 -= rhs.pad2;
			lhs.d = d - rhs.d; lhs.pad3 -= rhs.pad3;

			return lhs;
		}

		inline const Matrix34 operator + ( const Matrix34& rhs ) const
		{
			Matrix34 lhs;

			lhs.a = a + rhs.a; lhs.pad0 += rhs.pad0;
			lhs.b = b + rhs.b; lhs.pad1 += rhs.pad1;
			lhs.c = c + rhs.c; lhs.pad2 += rhs.pad2;
			lhs.d = d + rhs.d; lhs.pad3 += rhs.pad3;

			return lhs;
		}

		inline const Matrix34& operator *= ( const Matrix34& rhs) 
		{
			T ax = a.x * rhs.a.x + a.y * rhs.b.x + a.z * rhs.c.x;
			T ay = a.x * rhs.a.y + a.y * rhs.b.y + a.z * rhs.c.y;
			T az = a.x * rhs.a.z + a.y * rhs.b.z + a.z * rhs.c.z;

			T bx = b.x * rhs.a.x + b.y * rhs.b.x + b.z * rhs.c.x;
			T by = b.x * rhs.a.y + b.y * rhs.b.y + b.z * rhs.c.y;
			T bz = b.x * rhs.a.z + b.y * rhs.b.z + b.z * rhs.c.z;

			T cx = c.x * rhs.a.x + c.y * rhs.b.x + c.z * rhs.c.x;
			T cy = c.x * rhs.a.y + c.y * rhs.b.y + c.z * rhs.c.y;
			T cz = c.x * rhs.a.z + c.y * rhs.b.z + c.z * rhs.c.z;

			T dx = d.x * rhs.a.x + d.y * rhs.b.x + d.z * rhs.c.x + rhs.d.x;
			T dy = d.x * rhs.a.y + d.y * rhs.b.y + d.z * rhs.c.y + rhs.d.y;
			T dz = d.x * rhs.a.z + d.y * rhs.b.z + d.z * rhs.c.z + rhs.d.z;

			a.x = ax; a.y = ay; a.z = az;
			b.x = bx; b.y = by; b.z = bz;
			c.x = cx; c.y = cy; c.z = cz;
			d.x = dx; d.y = dy; d.z = dz;

			return *this;
		}

		inline Matrix34& operator += ( const Matrix34& rhs) 
		{
			a += rhs.a;  pad0 += rhs.pad0;
			b += rhs.b;  pad1 += rhs.pad1;
			c += rhs.c;  pad2 += rhs.pad2;
			d += rhs.d;  pad3 += rhs.pad3;

			return *this;
		}

		inline Matrix34& operator -= ( const Matrix34& rhs) 
		{
			a -= rhs.a;  pad0 -= rhs.pad0;
			b -= rhs.b;  pad1 -= rhs.pad1;
			c -= rhs.c;  pad2 -= rhs.pad2;
			d -= rhs.d;  pad3 -= rhs.pad3;

			return *this;
		}

		inline bool operator == ( const Matrix34& rhs )
		{
			return a == rhs.a && b == rhs.b && c == rhs.c && d == rhs.d && pad0 == rhs.pad0 && pad1 == rhs.pad1 && pad2 == rhs.pad2 && pad3 == rhs.pad3;				
		}

		inline bool operator != ( const Matrix34& rhs )
		{
			return a != rhs.a && b != rhs.b && c != rhs.c && d != rhs.d && pad0 != rhs.pad0 && pad1 != rhs.pad1 && pad2 != rhs.pad2 && pad3 != rhs.pad3;
		}

		/************************************************************************
		Rotation Transformation
		**********************************************************
		************/

		//Rotations (t is given in radians)
		inline static  Matrix34 getRotationX( T t )  
		{
			Matrix34 rx;

			T st = sin(t);
			T ct = cos(t);

			rx.a.x = 1.0; rx.a.y = 0.0; rx.a.z = 0.0;
			rx.b.x = 0.0; rx.b.y = ct;  rx.b.z = st;
			rx.c.x = 0.0; rx.c.y = -st;  rx.c.z = ct;
			rx.d.makeZero();

			return rx;
		}

		inline void makeRotationX( T t)
		{
			T st = sin(t);
			T ct = cos(t);

			a.x = 1.0;  a.y = 0.0;  a.z = 0.0;
			b.x = 0.0;  b.y = ct;   b.z = st;
			c.x = 0.0;  c.y = -st;   c.z = ct;
			d.makeZero();
		}

		inline static Matrix34 getRotationY(T t)  
		{
			Matrix34 ry;

			T st = sin(t);
			T ct = cos(t);

			ry.a.x = ct;  ry.a.y = 0.0; ry.a.z = -st;
			ry.b.x = 0.0; ry.b.y = 1.0; ry.b.z = 0.0;
			ry.c.x = st;  ry.c.y = 0.0; ry.c.z = ct;
			ry.d.makeZero();

			return ry;
		}

		inline void makeRotationY( T t)
		{
			T st = sin(t);
			T ct = cos(t);

			a.x = ct;	 a.y = 0.0;  a.z = -st;
			b.x = 0.0;  b.y = 1.0;  b.z = 0.0;
			c.x = st;   c.y = 0.0;  c.z = ct;
			d.makeZero();
		}

		inline static Matrix34 getRotationZ( T t )  
		{
			Matrix34 rz;

			T st = sin(t);
			T ct = cos(t);

			rz.a.x = ct;   rz.a.y = st;  rz.a.z = 0.0;
			rz.b.x = -st;  rz.b.y = ct;  rz.b.z = 0.0;
			rz.c.x = 0.0;  rz.c.y = 0.0;  rz.c.z = 1.0;
			rz.d.makeZero();

			return rz;
		}

		inline void makeRotationZ( T t )
		{
			T st = sin(t);
			T ct = cos(t);

			a.x = ct;   a.y = st;  a.z = 0.0;
			b.x = -st;  b.y = ct;  b.z = 0.0;
			c.x = 0.0;  c.y = 0.0; c.z = 1.0;
			d.makeZero();
		}

		inline static Matrix34 getRotation( const Vec3<T>& r, T t )
		{
			Matrix34 mr;

			Vec3<T> v = r.copyNormalize();

			T ct   = cos(t);
			T st   = sin(t);
			T omct = 1.0 - ct;

			mr.a.x = omct * v.x * v.x + ct;
			mr.a.y = omct * v.x * v.y + st * v.z;
			mr.a.z = omct * v.x * v.z - st * v.y;
			mr.b.x = omct * v.x * v.y - st * v.z;
			mr.b.y = omct * v.y * v.y + ct;
			mr.b.z = omct * v.y * v.z + st * v.x;
			mr.c.x = omct * v.x * v.z + st * v.y;
			mr.c.y = omct * v.y * v.z - st * v.x;
			mr.c.z = omct * v.z * v.z + ct;
			mr.d.makeZero();

			return mr;
		}

		inline void makeRotation( const Vec3<T>& r, T t )
		{
			Vec3<T> v = r.CopyNormalize();

			T ct   = cos(t);
			T st   = sin(t);
			T omct = 1.0 - ct;

			a.x = omct * v.x * v.x + ct;
			b.y = omct * v.y * v.y + ct;
			c.z = omct * v.z * v.z + ct;
			a.y = omct * v.x * v.y + st * v.z;
			b.x = omct * v.x * v.y - st * v.z;
			a.z = omct * v.x * v.z - st * v.y;
			c.x = omct * v.x * v.z + st * v.y;
			b.z = omct * v.y * v.z + st * v.x;
			c.y = omct * v.y * v.z - st * v.x;
			d.makeZero();
		}

		inline static Matrix34 getRotation( T x, T y, T z, T t )
		{
			Vec3<T> v(x, y, z);

			return GetRotation(v, t);
		}

		inline void makeRotation( T x, T y, T z, T t )
		{
			Vec3<T> v(x, y, z);

			makeRotation(v, t);
		}

		inline Vec3<T> getRotationAxeX()
		{
			return a;
		}

		inline Vec3<T> getRotationAxeY()
		{
			return b;
		}

		inline Vec3<T> getRotationAxeZ()
		{
			return c;
		}

		inline void setRotation(const Vec3<T>& xAxis, const Vec3<T>& yAxis, const Vec3<T>& zAxis)
		{
			a = xAxis; a.normalize();
			b = yAxis; b.normalize();
			c = zAxis; c.normalize();
		}

		//will set only the rotation part
		inline void setRotation(const Matrix34& m)
		{
			a = m.a;
			b = m.b;
			c = m.c;
		}

		/************************************************************************
		*  Translation Transformation
		************************************************************************/

		/* Sets the Translation transformations
		*/
		inline void setTranslation(const Vec3<T>& t)
		{
			d = t;
		}

		inline void setTranslation( T x, T y, T z )
		{
			d.x = x; d.y = y; d.z = z;
		}

		inline Vec3<T> getTranslation()
		{
			return d;
		}

		inline static Matrix34 getTranslation( T x, T y, T z )
		{
			Matrix34 t;

			t.a.makeZero();
			t.b.makeZero();
			t.c.makeZero();
			t.d.x = x; t.d.y = y; t.d.z = z;

			return t;
		}

		inline void makeTranslation( T x, T y, T z )
		{
			a.makeZero();
			b.makeZero();
			c.makeZero();
			d.x = x;  d.y = y;  d.z = z;
		}

		inline static Matrix34 getTranslation( const Vec3<T>& t )
		{
			Matrix34 vt;

			vt.a.makeZero();
			vt.b.makeZero();
			vt.c.makeZero();
			vt.d = t;

			return vt;
		}

		inline void makeTranslation( const Vec3<T>& t  )
		{
			a.makeZero();
			b.makeZero();
			c.makeZero();
			d = t;
		}

		inline Vec3<T> getTranslation() const
		{
			return d;
		}

		/************************************************************************
		*  Scale Transformation
		************************************************************************/

		inline static Matrix34 getScale( T x, T y, T z )
		{
			Matrix34 s;

			s.a.x = x;   s.a.y = 0.0; s.a.z = 0.0;
			s.b.x = 0.0; s.b.y = y;   s.b.z = 0.0;
			s.c.x = 0.0; s.c.y = 0.0; s.c.z = z;
			s.d.makeZero();

			return s;
		}

		inline void makeScale( T x, T y, T z )
		{
			a.x = x;    a.y = 0.0;  a.z = 0.0;
			b.x = 0.0;  b.y = y;    b.z = 0.0;
			c.x = 0.0;  c.y = 0.0;  c.z = z;
			d.makeZero();
		}

		inline static Matrix34 getScale(T s)
		{					
			Matrix34 ms;

			ms.a.x = s;   ms.a.y = 0.0; ms.a.z = 0.0;
			ms.b.x = 0.0; ms.b.y = s;   ms.b.z = 0.0;
			ms.c.x = 0.0; ms.c.y = 0.0; ms.c.z = s;
			ms.d.makeZero();

			return ms;
		}

		inline void makeScale( T s )
		{
			a.x = s;   a.y = 0.0; a.z = 0.0;
			b.x = 0.0; b.y = s;   b.z = 0.0;
			c.x = 0.0; c.y = 0.0; c.z = s;
			d.makeZero();
		}

		inline void setScale( T x, T y, T z )
		{
			a.x = x; b.y = y; c.z = z;
		}

		inline void setScale( T s )
		{
			a.x = b.y = c.z = s;
		}

		/************************************************************************
		* Vector Transform
		************************************************************************/

		Vec3<T> Transform(Vec3<T> in)
		{
			T x = in.x * a.x + in.y * a.y + in.z * a.z;
			T y = in.x * b.x + in.y * b.y + in.z * b.z;
			T z = in.x * c.x + in.y * c.y + in.z * c.z;

			return Vec3<T>(x, y, z);
		}

		friend Vec3<T> operator* (const Vec3<T>& v, const Matrix34& m)
		{
			T x = v.x * m.a.x + v.y * m.a.y + v.z * m.a.z;
			T y = v.x * m.b.x + v.y * m.b.y + v.z * m.b.z;
			T z = v.x * m.c.x + v.y * m.c.y + v.z * m.c.z;

			return Vec3<T>(x, y, z);
		}

		//inline Matrix34 Transpose3x3()
		//{
		//	return	Matrix34 (a.x, b.x, c.x, a.y, b.y, c.y, a.z, b.z, c.z, d.x, d.y, d.z);		
		//}

		/************************************************************************
		*  Inverse
		************************************************************************/

		//work only on orthonormal matrices
		inline void fastInverse() 
		{
			MxAssert(isOrthonormal());

			Matrix34 inv;

			//the rotation matrix becomes the transpose of the current rotation matrix
			inv.a.x = a.x; inv.a.y = b.x; inv.a.z = c.x; 
			inv.b.x = a.y; inv.b.y = b.y; inv.b.z = c.y;
			inv.c.x = a.z; inv.c.y = b.z; inv.c.z = c.z;	

			inv.d.x = -d.dotProduct(a);
			inv.d.y = -d.dotProduct(b);
			inv.d.z = -d.dotProduct(c);

			(*this) = inv;
		}

		inline Matrix34 copyFastInverse() const
		{
			MxAssert(isOrthonormal());

			Matrix34 i;

			//the rotation matrix becomes the transpose of the current rotation matrix
			i.a.x = a.x; i.a.y = b.x; i.a.z = c.x; 
			i.b.x = a.y; i.b.y = b.y; i.b.z = c.y;
			i.c.x = a.z; i.c.y = b.z; i.c.z = c.z;

			i.d.x = -d.dotProduct(a);
			i.d.y = -d.dotProduct(b);
			i.d.z = -d.dotProduct(c);

			return i;
		}

		void inverse()
		{
			if(isOrthonormal())
			{
				fastInverse();
				return;
			}

			T m10 = b.x, m11 = b.y, m12 = b.z;
			T m20 = c.x, m21 = c.y, m22 = c.z;

			T t00 = m22 * m11 - m21 * m12;
			T t10 = m20 * m12 - m22 * m10;
			T t20 = m21 * m10 - m20 * m11;

			T m00 = a.x, m01 = a.y, m02 = a.z;

			T invDet = 1 / (m00 * t00 + m01 * t10 + m02 * t20);

			t00 *= invDet; t10 *= invDet; t20 *= invDet;

			m00 *= invDet; m01 *= invDet; m02 *= invDet;

			T r00 = t00;
			T r01 = m02 * m21 - m01 * m22;
			T r02 = m01 * m12 - m02 * m11;

			T r10 = t10;
			T r11 = m00 * m22 - m02 * m20;
			T r12 = m02 * m10 - m00 * m12;

			T r20 = t20;
			T r21 = m01 * m20 - m00 * m21;
			T r22 = m00 * m11 - m01 * m10;

			T m03 = d.x, m13 = d.y, m23 = d.z;

			T r30 = - (r00 * m03 + r01 * m13 + r02 * m23);
			T r31 = - (r10 * m03 + r11 * m13 + r12 * m23);
			T r32 = - (r20 * m03 + r21 * m13 + r22 * m23);

			a.x = r00; a.y = r01; a.z = r02; 
			b.x = r10; b.y = r11; b.z = r12;
			c.x = r20; c.y = r21; c.z = r22;
			d.x = r30; d.y = r31; d.z = r32;
		}


		Matrix34 copyInverse() const
		{
			if(isOrthonormal())
			{
				return copyFastInverse();
			}

			T m10 = b.x, m11 = b.y, m12 = b.z;
			T m20 = c.x, m21 = c.y, m22 = c.z;

			T t00 = m22 * m11 - m21 * m12;
			T t10 = m20 * m12 - m22 * m10;
			T t20 = m21 * m10 - m20 * m11;

			T m00 = a.x, m01 = a.y, m02 = a.z;

			T invDet = 1 / (m00 * t00 + m01 * t10 + m02 * t20);

			t00 *= invDet; t10 *= invDet; t20 *= invDet;

			m00 *= invDet; m01 *= invDet; m02 *= invDet;

			T r00 = t00;
			T r01 = m02 * m21 - m01 * m22;
			T r02 = m01 * m12 - m02 * m11;

			T r10 = t10;
			T r11 = m00 * m22 - m02 * m20;
			T r12 = m02 * m10 - m00 * m12;

			T r20 = t20;
			T r21 = m01 * m20 - m00 * m21;
			T r22 = m00 * m11 - m01 * m10;

			T m03 = d.x, m13 = d.y, m23 = d.z;

			T r30 = - (r00 * m03 + r01 * m13 + r02 * m23);
			T r31 = - (r10 * m03 + r11 * m13 + r12 * m23);
			T r32 = - (r20 * m03 + r21 * m13 + r22 * m23);

			return Matrix34 ( r00, r01, r02, 
				r10, r11, r12, 
				r20, r21, r22, 
				r30, r31, r32 );
		}

		/************************************************************************
		* Orthonormality
		************************************************************************/

		inline bool isOrthonormal() const
		{
			if(a.magnitudeSquared() != 1 || b.magnitudeSquared()!= 1 || c.magnitudeSquared() != 1)
			{
				return false;
			}

			if(a.dotProduct(b) != 0 || b.dotProduct(c) != 0 || c.dotProduct(a) != 0)
			{
				return false;
			}

			return true;
		}


		inline friend std::ostream& operator <<
			( std::ostream& o, const Matrix34& v )
		{				
			o << "\n " << v.a.x << " " << v.a.y << " " << v.a.z << " " << v.pad0 << "\n";
			o << " " << v.b.x << " " << v.b.y << " " << v.b.z << " " << v.pad1 << "\n";
			o << " " << v.c.x << " " << v.c.y << " " << v.c.z << " " << v.pad2 << "\n";
			o << " " << v.d.x << " " << v.d.y << " " << v.d.z << " " << v.pad3 << "\n";
			return o;
		}
	};

	typedef Matrix34<int> Matrix34i;
	typedef Matrix34<float> Matrix34f;
	typedef Matrix34<double> Matrix34d;
}