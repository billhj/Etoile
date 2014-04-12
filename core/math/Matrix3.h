/**
* Copyright(C) 2009-2012                
* @author Jing HUANG
* @file Matrix3.h
* @brief 
* @date 1/2/2011
*/

#pragma once
#include <math.h>
#include "Vectors.h"

namespace Etoile
{
	template <class T>
	class Matrix3
	{
	public:	//! casting from a Vec3<T>  to a matrix is the tilde operator
		Matrix3(const Vec3<T>  &v)
		{
			data[0][0]= 0,   data[0][1]= -v[2], data[0][2]= v[1];
			data[1][0]= v[2], data[1][1]=  0,   data[1][2]=-v[0]; 
			data[2][0]= -v[1],data[2][1]=  v[0], data[2][2]= 0;
		}

		Matrix3(const T * v)
		{
			data[0][0]= v[0],   data[0][1]= v[1], data[0][2]= v[2];
			data[1][0]= v[3], data[1][1]=  v[4],   data[1][2]=v[5]; 
			data[2][0]= v[6],data[2][1]=  v[7], data[2][2]= v[8];
		}

		Matrix3(const Matrix3& m)
		{
			for (int y = 0; y < 3; y++)
			{
				for (int x = 0; x < 3; x++) 
				{
					data[y][x] = m.data[y][x];
				}
			}
		}

		Matrix3()
		{

		}

		Matrix3(const Vec3<T>  &v0, const Vec3<T>  &v1, const Vec3<T>  &v2)
		{	data[0]= v0, data[1]= v1, data[2]= v2;  }

		Matrix3(T  m00, T  m01, T  m02,
			T  m10, T  m11, T  m12,
			T  m20, T  m21, T  m22)
		{  
			data[0][0]= m00, data[0][1]= m01, data[0][2]= m02;
			data[1][0]= m10, data[1][1]= m11, data[1][2]= m12;
			data[2][0]= m20, data[2][1]= m21, data[2][2]= m22;
		}

		//skew-symmetric matrix
		inline void skewMatrix(Vec3<T> v)
		{
			data[0][0]= 0; data[0][1]= -v.z(); data[0][2]= v.y();
			data[1][0]= v.z(); data[1][1]= 0; data[1][2]= -v.x();
			data[2][0]= -v.y(); data[2][1]= v.x(); data[2][2]= 0;
		}

		//! construct an x axis rotation matrix
		inline static void rotate_x(Matrix3 &m, T  angle)
		{
			m.data[0][0]= 1; m.data[0][1]= 0; m.data[0][2]= 0;
			m.data[1][0]= 0; m.data[1][1]= cosf(angle); m.data[1][2]= -sinf(angle);
			m.data[2][0]= 0; m.data[2][1]= sinf(angle); m.data[2][2]= cosf(angle);
		}

		//! construct an y axis rotation matrix
		inline static void rotate_y(Matrix3 &m, T  angle)
		{
			m.data[0][0]= cosf(angle); m.data[0][1]= 0; m.data[0][2]= sinf(angle);
			m.data[1][0]= 0; m.data[1][1]= 0; m.data[1][2]= 0;
			m.data[2][0]= -sinf(angle); m.data[2][1]= 0; m.data[2][2]= cosf(angle);
		}

		//! construct an z axis rotation matrix
		inline static void rotate_z(Matrix3 &m, T  angle)
		{
			m.data[0][0]= cosf(angle); m.data[0][1]= -sinf(angle); m.data[0][2]= 0;
			m.data[1][0]= sinf(angle); m.data[1][1]= cosf(angle);  m.data[1][2]= 0;
			m.data[2][0]= 0;           m.data[2][1]= 0;            m.data[2][2]= 1;
		}

		//! constructs a rotation matrix around an arbitrary vector
		inline static void rotate(Matrix3 &m, const Vec3<T>  &v, const T  angle)
		{
			T  c= cosf(angle), s= sinf(angle), ci = 1-c; 
			T xxci = v[0]*v[0] * ci, xyci = v[0]*v[1] * ci, xzci = v[0]*v[2]*ci, yyci = v[1]*v[1]*ci, yzci = v[1]*v[2]*ci, zzci = v[2]*v[2]*ci;
			T ys = v[1]*s, zs = v[2]*s, xs = v[0]*s;
			m.data[0][0]= xxci + c;		m.data[0][1]= xyci - zs;	m.data[0][2]= xzci + ys; 
			m.data[1][0]= xyci + zs; m.data[1][1]= yyci + c;	m.data[1][2]= yzci - xs; 
			m.data[2][0]= xzci - ys; m.data[2][1]= yzci + xs;  m.data[2][2]= zzci + c; 
		}

		//! sets a rotation matrix up by euler angles
		//! uses pitch, roll, yaw (xyz) form
		//may have pb forget the order
		inline static void rotate_euler(Matrix3 &m, T  x, T  y, T  z)
		{
			double cx = cos(x);
			double sx = sin(x);
			double cy = cos(y);
			double sy = sin(y);
			double cz = cos(z);
			double sz = sin(z);

			m.data[0][0] = (T )(cy*cz); 
			m.data[0][1] = (T )(cy*sz);
			m.data[0][2] = (T )(-sy);

			double sxsy = sx*sy;
			double cxsy = cx*sy;

			m.data[1][0] = (T )(sxsy*cz-cx*sz);
			m.data[1][1] = (T )(sxsy*sz+cx*cz);
			m.data[1][2] = (T )(sx*cy);

			m.data[2][0] = (T )(cxsy*cz+sx*sz);
			m.data[2][1] = (T )(cxsy*sz-sx*cz);
			m.data[2][2] = (T )(cx*cy);
		}

		//! sets a rotation matrix up by euler angles
		//! uses pitch, roll, yaw (xyz) form
		inline static void rotate_euler(Matrix3 &m, const Vec3<T>  &angles)
		{	
			rotate_euler(m, angles[0], angles[1], angles[2]);	
		}

		Matrix3& operator=(const Matrix3& m)
		{
			for (int y=0; y<3; y++) {
				for (int x=0; x<3; x++) {
					data[y][x] = m.data[y][x]; 
				}
			}
			return (*this); 
		}

		//! multiplication by a matrix
		const Matrix3 &operator *=(const Matrix3 &m)
		{
			Matrix3 temp(Vec3<T> (m.data[0][0]*data[0][0] + m.data[1][0]*m.data[0][1] + m.data[2][0]*data[0][2],
				m.data[0][1]*data[0][0] + m.data[1][1]*data[0][1] + m.data[2][1]*data[0][2],
				m.data[0][2]*data[0][0] + m.data[1][2]*data[0][1] + m.data[2][2]*data[0][2]),
				Vec3<T> (m.data[0][0]*data[1][0] + m.data[1][0]*data[1][1] + m.data[2][0]*data[1][2],
				m.data[0][1]*data[1][0] + m.data[1][1]*data[1][1] + m.data[2][1]*data[1][2],
				m.data[0][2]*data[1][0] + m.data[1][2]*data[1][1] + m.data[2][2]*data[1][2]),
				Vec3<T> (m.data[0][0]*data[2][0] + m.data[1][0]*data[2][1] + m.data[2][0]*data[2][2],
				m.data[0][1]*data[2][0] + m.data[1][1]*data[2][1] + m.data[2][1]*data[2][2],
				m.data[0][2]*data[2][0] + m.data[1][2]*data[2][1] + m.data[2][2]*data[2][2]));		
			return *this= temp;
		}


		friend Matrix3 operator*(const Matrix3 &m1, const Matrix3 &m2)
		{
			Matrix3 answer;
			for (int y=0; y<3; y++) 
			{
				for (int x=0; x<3; x++)
				{
					for (int i=0; i<3; i++) 
					{
						answer.data[y][x] 
						+= m1.data[y][i] * m2.data[i][x];
					}
				}
			}
			return answer;
		}

		//! multiplication by a vector
		const Vec3<T>  operator *(const Vec3<T>  &v) const
		{	
			return Vec3<T> (v[0]*data[0][0] + v[1]*data[0][1] + v[2]*data[0][2], 
				v[0]*data[1][0] + v[1]*data[1][1] + v[2]*data[1][2], 
				v[0]*data[2][0] + v[1]*data[2][1] + v[2]*data[2][2]);	
		}			

		friend Matrix3 operator*(const Matrix3 &m, T  f)
		{
			Matrix3 answer;
			for (int y=0; y<3; y++) 
			{
				for (int x=0; x<3; x++) 
				{
					answer.data[y][x] = m.data[y][x] * f;
				}
			}
			return answer;
		}

		//! multiplication by a scalar
		const Matrix3 &operator *=(const T  s)
		{
			data[0]*=s; data[1]*=s; data[2]*=s;		
			return *this;
		}

		//! division by a scalar
		const Matrix3 &operator /=(const T  s)
		{
			data[0]/=s; data[1]/=s; data[2]/=s;		
			return *this;
		}

		//! addition	
		const Matrix3 operator +(const Matrix3 &m) const
		{   
			return Matrix3(data[0]+m.data[0], data[1]+m.data[1], data[2]+m.data[2]);	
		}	

		//! addition
		const Matrix3 &operator +=(const Matrix3 &m)
		{
			data[0]+= m.data[0];
			data[1]+= m.data[1];
			data[2]+= m.data[2];
			return *this;
		}

		const Vec3<T>& operator[](int i) const 
		{ 
			assert (i >= 0 && i < 3);
			return data[i]; 
		}

		Vec3<T> & operator[](int i) { 
			assert (i >= 0 && i < 3);
			return data[i]; 
		}


		//! subtraction	
		const Matrix3 operator -(const Matrix3 &m) const
		{   
			return Matrix3(data[0]-m.data[0], data[1]-m.data[1], data[2]-m.data[2]);	 
		}	

		//! subtraction
		const Matrix3 &operator -=(const Matrix3 &m)
		{
			data[0]-= m.data[0];
			data[1]-= m.data[1];
			data[2]-= m.data[2];
			return *this;
		}

		//! compute the trace
		T  trace() const
		{   return data[0][0] + data[1][1] + data[2][2];  }



		//! compute the determinant
		T  determinant() const
		{	
			return det3x3(data[0][0], data[0][1],data[0][2],
				data[1][0],data[1][1],data[1][2],
				data[2][0],data[2][1],data[2][2]);
		}

		void transpose(Matrix3 &m) const
		{
			// be careful, <this> might be <m>
			Matrix3 tmp = Matrix3(*this);
			for (int y = 0; y < 3; y++) 
			{
				for (int x = 0; x < 3; x++)
				{
					m.data[y][x] = tmp.data[x][y];
				}
			}
		}

		void transpose() { transpose(*this); }

		void inverse()
		{
			*this = inverseMatrix();
		}

		Matrix3 inverseMatrix()
		{
			Matrix3 ans;
			float det = this->determinant();

			ans.data[0][0] = det2x2(data[1][1], data[1][2], data[2][1], data[2][2]) / det;
			ans.data[0][1] = det2x2(data[0][2], data[0][1], data[2][2], data[2][1]) / det;
			ans.data[0][2] = det2x2(data[0][1], data[0][2], data[1][1], data[1][2]) / det;
			ans.data[1][0] = det2x2(data[1][2], data[1][0], data[2][2], data[2][0]) / det;
			ans.data[1][1] = det2x2(data[0][0], data[0][2], data[2][0], data[2][2]) / det;
			ans.data[1][2] = det2x2(data[0][2], data[0][0], data[1][2], data[1][0]) / det;
			ans.data[2][0] = det2x2(data[1][0], data[1][1], data[2][0], data[2][1]) / det;
			ans.data[2][1] = det2x2(data[0][1], data[0][0], data[2][1], data[2][0]) / det;
			ans.data[2][2] = det2x2(data[0][0], data[0][1], data[1][0], data[1][1]) / det;

			return	ans;
		}

		void setToIdentity()
		{
			for (int y = 0; y < 3; y++) 
			{
				for (int x = 0; x < 3; x++) 
				{
					data[y][x] = (x == y); 
				}
			}
		}

		friend ostream& operator<<(ostream& o, const Matrix3& m)
		{
			return o << m[0] << '\t' << m[1] << '\t'<< m[2] << '\n'   << m[3] << '\t' << m[4] << '\t'<< m[5] << '\n'  << m[6] << '\t' << m[7] << '\t'<< m[8] << '\n';
		}

		T det3x3(T a1,T a2,T a3,
			T b1,T b2,T b3,
			T c1,T c2,T c3) const
		{
			return
				a1 * det2x2( b2, b3, c2, c3 )
				- b1 * det2x2( a2, a3, c2, c3 )
				+ c1 * det2x2( a2, a3, b2, b3 );
		}

		T det2x2(T a, T b,
			T c, T d) const
		{
			return a * d - b * c;
		}

		bool checkRotation()
		{
			T l1 = (double)sqrt( data[0][0] * data[0][0] +
				data[1][0] * data[1][0] +
				data[2][0] * data[2][0] );
			T l2 = (double)sqrt( data[0][1] * data[0][1] +
				data[1][1] * data[1][1] +
				data[2][1] * data[2][1] );
			T l3 = (double)sqrt( data[0][2] * data[0][2] +
				data[1][2] * data[1][2] +
				data[2][2] * data[2][2] );

			if( abs(l1 - 1) > 0.000001 || abs(l2 - 1) > 0.000001 || abs(l3 - 1) > 0.000001)
			{
				return false;
			}
			return true;
		}

		void rotationNormalize()
		{
			*this /= determinant(); 
		}
		
	private:
		Vec3<T> data[3];
	};

	typedef Matrix3<int> Matrix3i;
	typedef Matrix3<float> Matrix3f;
	typedef Matrix3<double> Matrix3d;
}