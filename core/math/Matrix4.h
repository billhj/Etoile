/**
* Copyright(C) 2009-2012                
* @author Jing HUANG
* @file Matrix4.h
* @brief 
* @date 1/2/2011
*/

#pragma once
#include <math.h>
#include "Matrix3.h"

namespace Etoile
{
	template <class T>
	class Matrix4 
	{

		T det4x4(T a1, T a2, T a3, T a4, 
			T b1, T b2, T b3, T b4, 
			T c1, T c2, T c3, T c4, 
			T d1, T d2, T d3, T d4) const
		{
			return 
				a1 * det3x3( b2, b3, b4, c2, c3, c4, d2, d3, d4)
				- b1 * det3x3( a2, a3, a4, c2, c3, c4, d2, d3, d4)
				+ c1 * det3x3( a2, a3, a4, b2, b3, b4, d2, d3, d4)
				- d1 * det3x3( a2, a3, a4, b2, b3, b4, c2, c3, c4);
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

	public:
		//CONSTRUCTORS & DESTRUCTOR
		Matrix4() 
		{
			clear(); 
		}

		Matrix4(const Matrix4& m)
		{
			for (int y = 0; y < 4; y++) 
			{
				for (int x = 0; x < 4; x++) 
				{
					data[y][x] = m.data[y][x]; 
				}
			}
		}

		Matrix4(const T  *m)
		{
			for (int y = 0; y < 4; y++) 
			{
				for (int x = 0; x < 4; x++) 
				{
					data[y][x] = m[4*y+x];
				}
			}
		}

		Matrix4(T m0, T m1,T m2,T m3,T m4,T m5,T m6,T m7,T m8,T m9,T m10,T m11,T m12,T m13,T m14,T m15)
		{
			data[0][0] = m0;
			data[0][1] = m1;
			data[0][2] = m2;
			data[0][3] = m3;
			data[1][0] = m4;
			data[1][1] = m5;
			data[1][2] = m6;
			data[1][3] = m7;
			data[2][0] = m8;
			data[2][1] = m9;
			data[2][2] = m10;
			data[2][3] = m11;
			data[3][0] = m12;
			data[3][1] = m13;
			data[3][2] = m14;
			data[3][3] = m15;

		}

		~Matrix4() {}

		// ACCESSOR
		T * getGLMatrix(void) const 
		{
			T  *glMat = new T [16];
			glMat[0]=data[0][0];  glMat[1]=data[1][0];  glMat[2]=data[2][0];  glMat[3]=data[3][0];
			glMat[4]=data[0][1];  glMat[5]=data[1][1];  glMat[6]=data[2][1];  glMat[7]=data[3][1];
			glMat[8]=data[0][2];  glMat[9]=data[1][2]; glMat[10]=data[2][2]; glMat[11]=data[3][2];
			glMat[12]=data[0][3]; glMat[13]=data[1][3]; glMat[14]=data[2][3]; glMat[15]=data[3][3];
			return glMat;
		}

		void setByGLMatrix(T* glMat)
		{
			data[0][0] = glMat[0]; data[1][0] = glMat[1]; data[2][0] = glMat[2];  data[3][0] = glMat[3];
			data[0][1] = glMat[4]; data[1][1] = glMat[5]; data[2][1] = glMat[6];  data[3][1] = glMat[7];
			data[0][2] = glMat[8]; data[1][2] = glMat[9]; data[2][2] = glMat[10]; data[3][2] = glMat[11];
			data[0][3] = glMat[12]; data[1][3] = glMat[13]; data[2][3] = glMat[14]; data[3][3] = glMat[15];
		}

		T  get(int x, int y) const 
		{ 
			assert (x >= 0 && x < 4);
			assert (y >= 0 && y < 4);
			return data[y][x]; 
		}

		// MODIFIERS
		void set(int x, int y, T  v) 
		{
			assert (x >= 0 && x < 4);
			assert (y >= 0 && y < 4);
			data[y][x] = v; 
		}

		void setToIdentity()
		{
			for (int y = 0; y < 4; y++) 
			{
				for (int x = 0; x < 4; x++) 
				{
					data[y][x] = (x == y); 
				}
			}
		}

		void clear() 
		{
			for (int y = 0; y < 4; y++) 
			{
				for (int x = 0; x < 4; x++)
				{
					data[y][x] = 0; 
				}
			}
		}

		void transpose(Matrix4 &m) const
		{
			// be careful, <this> might be <m>
			Matrix4 tmp = Matrix4(*this);
			for (int y = 0; y < 4; y++) 
			{
				for (int x = 0; x < 4; x++)
				{
					m.data[y][x] = tmp.data[x][y];
				}
			}
		}

		void transpose() { transpose(*this); }

		int inverse(Matrix4 &m, T  epsilon = 1e-08) const 
		{
			m = *this;

			T a1, a2, a3, a4, b1, b2, b3, b4, c1, c2, c3, c4, d1, d2, d3, d4;
			a1 = m.data[0][0]; b1 = m.data[0][1]; c1 = m.data[0][2]; d1 = m.data[0][3];
			a2 = m.data[1][0]; b2 = m.data[1][1]; c2 = m.data[1][2]; d2 = m.data[1][3];
			a3 = m.data[2][0]; b3 = m.data[2][1]; c3 = m.data[2][2]; d3 = m.data[2][3];
			a4 = m.data[3][0]; b4 = m.data[3][1]; c4 = m.data[3][2]; d4 = m.data[3][3];

			T det = det4x4(a1,a2,a3,a4,b1,b2,b3,b4,c1,c2,c3,c4,d1,d2,d3,d4);

			if (fabs(det) < epsilon) {
				printf ("Matrix4::Inverse --- singular matrix, can't invert!\n");
				assert(0);
				return 0;
			}

			m.data[0][0] =   det3x3( b2, b3, b4, c2, c3, c4, d2, d3, d4);
			m.data[1][0] = - det3x3( a2, a3, a4, c2, c3, c4, d2, d3, d4);
			m.data[2][0] =   det3x3( a2, a3, a4, b2, b3, b4, d2, d3, d4);
			m.data[3][0] = - det3x3( a2, a3, a4, b2, b3, b4, c2, c3, c4);

			m.data[0][1] = - det3x3( b1, b3, b4, c1, c3, c4, d1, d3, d4);
			m.data[1][1] =   det3x3( a1, a3, a4, c1, c3, c4, d1, d3, d4);
			m.data[2][1] = - det3x3( a1, a3, a4, b1, b3, b4, d1, d3, d4);
			m.data[3][1] =   det3x3( a1, a3, a4, b1, b3, b4, c1, c3, c4);

			m.data[0][2] =   det3x3( b1, b2, b4, c1, c2, c4, d1, d2, d4);
			m.data[1][2] = - det3x3( a1, a2, a4, c1, c2, c4, d1, d2, d4);
			m.data[2][2] =   det3x3( a1, a2, a4, b1, b2, b4, d1, d2, d4);
			m.data[3][2] = - det3x3( a1, a2, a4, b1, b2, b4, c1, c2, c4);

			m.data[0][3] = - det3x3( b1, b2, b3, c1, c2, c3, d1, d2, d3);
			m.data[1][3] =   det3x3( a1, a2, a3, c1, c2, c3, d1, d2, d3);
			m.data[2][3] = - det3x3( a1, a2, a3, b1, b2, b3, d1, d2, d3);
			m.data[3][3] =   det3x3( a1, a2, a3, b1, b2, b3, c1, c2, c3);

			m *= 1/det;
			return 1;
		}

		int inverse(T  epsilon = 1e-08) { return inverse(*this,epsilon); }

		Matrix3<T> subMatrix(unsigned char i, unsigned char j) const
		{
			switch(i)
			{
			case 0: switch(j)
					{
			case 0: return Matrix3<T>(data[1][1],data[1][2],data[1][3], 
						data[2][1],data[2][2],data[2][3],
						data[3][1],data[3][2],data[3][3]);									   
			case 1: return Matrix3<T>(data[0][1],data[0][2],data[0][3], 
						data[2][1],data[2][2],data[2][3],
						data[3][1],data[3][2],data[3][3]);									   
			case 2: return Matrix3<T>(data[0][1],data[0][2],data[0][3], 
						data[1][1],data[1][2],data[1][3],
						data[3][1],data[3][2],data[3][3]);
			default:return Matrix3<T>(data[0][1],data[0][2],data[0][3], // case 3
						data[1][1],data[1][2],data[1][3], 
						data[2][1],data[2][2],data[2][3]);
					} 
			case 1: switch(j)
					{
			case 0: return Matrix3<T>(data[1][0],data[1][2],data[1][3], 
						data[2][0],data[2][2],data[2][3],
						data[3][0],data[3][2],data[3][3]);									   
			case 1: return Matrix3<T>(data[0][0],data[0][2],data[0][3], 
						data[2][0],data[2][2],data[2][3],
						data[3][0],data[3][2],data[3][3]);									   
			case 2: return Matrix3<T>(data[0][0],data[0][2],data[0][3], 
						data[1][0],data[1][2],data[1][3],
						data[3][0],data[3][2],data[3][3]);
			default:return Matrix3<T>(data[0][0],data[0][2],data[0][3], // case 3
						data[1][0],data[1][2],data[1][3], 
						data[2][0],data[2][2],data[2][3]);
					} 
			case 2: switch(j) 
					{
			case 0: return Matrix3<T>(data[1][0],data[1][1],data[1][3], 
						data[2][0],data[2][1],data[2][3],
						data[3][0],data[3][1],data[3][3]);									   
			case 1: return Matrix3<T>(data[0][0],data[0][1],data[0][3], 
						data[2][0],data[2][1],data[2][3],
						data[3][0],data[3][1],data[3][3]);									   
			case 2: return Matrix3<T>(data[0][0],data[0][1],data[0][3], 
						data[1][0],data[1][1],data[1][3],
						data[3][0],data[3][1],data[3][3]);
			default:return Matrix3<T>(data[0][0],data[0][1],data[0][3], // case 3
						data[1][0],data[1][1],data[1][3], 
						data[2][0],data[2][1],data[2][3]);
					} 
			default: switch(j) // case 3
					 {
			case 0: return Matrix3<T>(data[1][0],data[1][1],data[1][2], 
						data[2][0],data[2][1],data[2][2],
						data[3][0],data[3][1],data[3][2]);									   
			case 1: return Matrix3<T>(data[0][0],data[0][1],data[0][2], 
						data[2][0],data[2][1],data[2][2],
						data[3][0],data[3][1],data[3][2]);									   
			case 2: return Matrix3<T>(data[0][0],data[0][1],data[0][2], 
						data[1][0],data[1][1],data[1][2],
						data[3][0],data[3][1],data[3][2]);
			default:return Matrix3<T>(data[0][0],data[0][1],data[0][2], // case 3
						data[1][0],data[1][1],data[1][2], 
						data[2][0],data[2][1],data[2][2]);
					 } 
			}
		}

		//// OVERLOADED OPERATORS
		Matrix3<T> getMatrix3() const
		{
			return Matrix3<T>(data[0][0],data[0][1],data[0][2], // case 3
				data[1][0],data[1][1],data[1][2], 
				data[2][0],data[2][1],data[2][2]);
		}

		const Vec4<T>& operator[](int i) const 
		{ 
			assert (i >= 0 && i < 4);
			return data[i]; 
		}

		Vec4<T> & operator[](int i) { 
			assert (i >= 0 && i < 4);
			return data[i]; 
		}

		Matrix4& operator=(const Matrix4& m)
		{
			for (int y=0; y<4; y++) {
				for (int x=0; x<4; x++) {
					data[y][x] = m.data[y][x]; 
				}
			}
			return (*this); 
		}

		int operator==(const Matrix4& m) const
		{
			for (int y=0; y<4; y++) {
				for (int x=0; x<4; x++) {
					if (this->data[y][x] != m.data[y][x]) {
						return 0; 
					}
				}
			}
			return 1; 
		}

		int operator!=(const Matrix4 &m) const { return !(*this==m); }

		friend Matrix4 operator+(const Matrix4 &m1, const Matrix4 &m2) 
		{
			Matrix4 answer;
			for (int y=0; y<4; y++) 
			{
				for (int x=0; x<4; x++) 
				{
					answer.data[y][x] = m1.data[y][x] + m2.data[y][x];
				}
			}
			return answer; 
		}

		friend Matrix4 operator-(const Matrix4 &m1, const Matrix4 &m2)
		{
			Matrix4 answer;
			for (int y=0; y<4; y++) 
			{
				for (int x=0; x<4; x++) 
				{
					answer.data[y][x] = m1.data[y][x] - m2.data[y][x];
				}
			}
			return answer; 
		}

		friend Matrix4 operator*(const Matrix4 &m1, const Matrix4 &m2)
		{
			Matrix4 answer;
			for (int y=0; y<4; y++) 
			{
				for (int x=0; x<4; x++)
				{
					for (int i=0; i<4; i++) 
					{
						answer.data[y][x] 
						+= m1.data[y][i] * m2.data[i][x];
					}
				}
			}
			return answer;
		}

		friend Matrix4 operator*(const Matrix4 &m, T  f)
		{
			Matrix4 answer;
			for (int y=0; y<4; y++) 
			{
				for (int x=0; x<4; x++) 
				{
					answer.data[y][x] = m.data[y][x] * f;
				}
			}
			return answer;
		}

		friend Vec4<T> operator*(const Matrix4 &m1,const Vec4<T> &v)
		{
			Vec4<T> answer;
			for (int y=0; y<4; y++) 
			{
				answer[y] = m1.data[y][0] * v[0] + m1.data[y][1] * v[1] + m1.data[y][2] * v[2] + m1.data[y][3] * v[3];
			}
			return answer;
		}

		friend Matrix4 operator*(T  f, const Matrix4 &m) { return m * f; }
		Matrix4& operator+=(const Matrix4& m) { *this = *this + m; return *this; }
		Matrix4& operator-=(const Matrix4& m) { *this = *this - m; return *this; }
		Matrix4& operator*=(const T  f)   { *this = *this * f; return *this; }
		Matrix4& operator*=(const Matrix4& m) { *this = *this * m; return *this; }

		friend ostream& operator<<(ostream& o, const Matrix4& m)
		{
			return o << m[0] << '\t' << m[1] << '\t'<< m[2] << '\t'<< m[3] << '\n'
				<< m[4] << '\t' << m[5] << '\t'<< m[6] << '\t'<< m[7] << '\n'
				<< m[8] << '\t' << m[9] << '\t'<< m[10] << '\t'<< m[11] << '\n'
				<< m[12] << '\t' << m[13] << '\t'<< m[14] << '\t'<< m[15] << '\n';
		}

		// TRANSFORMATIONS
		static Matrix4 makeTranslation(const Vec3<T>  &v) 
		{
			Matrix4 t;
			t.setToIdentity();
			t.data[0][3] = v[0];
			t.data[1][3] = v[1];
			t.data[2][3] = v[2];
			return t;
		}

		static Matrix4 makeScale(const Vec3<T>  &v) 
		{
			Matrix4 s; 
			s.setToIdentity();
			s.data[0][0] = v[0];
			s.data[1][1] = v[1];
			s.data[2][2] = v[2];
			s.data[3][3] = 1;
			return s;
		}

		static Matrix4 makeScale(T  s) { return makeScale(Vec3<T> (s,s,s)); }
		static Matrix4 makeXRotation(T  theta) 
		{
			Matrix4 rx;
			rx.setToIdentity();
			rx.data[1][1]= (T)cos((T)theta);
			rx.data[1][2]=-(T)sin((T)theta);
			rx.data[2][1]= (T)sin((T)theta);
			rx.data[2][2]= (T)cos((T)theta);
			return rx;
		}

		static Matrix4 makeYRotation(T  theta)
		{
			Matrix4 ry;
			ry.setToIdentity();
			ry.data[0][0]= (T)cos((T)theta);
			ry.data[0][2]= (T)sin((T)theta);
			ry.data[2][0]=-(T)sin((T)theta);
			ry.data[2][2]= (T)cos((T)theta);
			return ry;
		}
		static Matrix4 makeZRotation(T  theta)
		{
			Matrix4 rz;
			rz.setToIdentity();
			rz.data[0][0]= (T)cos((T)theta);
			rz.data[0][1]=-(T)sin((T)theta);
			rz.data[1][0]= (T)sin((T)theta);
			rz.data[1][1]= (T)cos((T)theta);
			return rz;
		}
		static Matrix4 makeAxisRotation(const Vec3<T>  &v, T  theta)
		{
			Matrix4 r;
			r.setToIdentity();

			T x = v[0]; T y = v[1]; T z = v[2];

			T c = cosf(theta);
			T s = sinf(theta);
			T xx = x*x;
			T xy = x*y;
			T xz = x*z;
			T yy = y*y;
			T yz = y*z;
			T zz = z*z;

			r.set(0,0, (1-c)*xx + c);
			r.set(0,1, (1-c)*xy + z*s);
			r.set(0,2, (1-c)*xz - y*s);
			r.set(0,3, 0);

			r.set(1,0, (1-c)*xy - z*s);
			r.set(1,1, (1-c)*yy + c);
			r.set(1,2, (1-c)*yz + x*s);
			r.set(1,3, 0);

			r.set(2,0, (1-c)*xz + y*s);
			r.set(2,1, (1-c)*yz - x*s);
			r.set(2,2, (1-c)*zz + c);
			r.set(2,3, 0);

			r.set(3,0, 0);
			r.set(3,1, 0);
			r.set(3,2, 0);
			r.set(3,3, 1);

			return r;
		}

		// Use to transform a point with a matrix
		// that may include translation
		void transform(Vec4<T> &v) const 
		{
			Vec4<T> answer;
			for (int y=0; y<4; y++) {
				answer.data[y] = 0;
				for (int i=0; i<4; i++) {
					answer.data[y] += data[y][i] * v[i];
				}
			}
			v = answer;
		}

		void transform(Vec3<T>  &v) const 
		{
			Vec4f v2 = Vec4f(v.x(),v.y(),v.z(),1);
			transform(v2);
			v.set(v2.x(),v2.y(),v2.z()); 
		}
		void transform(Vec2f &v) const 
		{
			Vec4f v2 = Vec4f(v.x(),v.y(),1,1);
			transform(v2);
			v.set(v2.x(),v2.y()); 
		}

		// Use to transform the direction of the ray
		// (ignores any translation)
		void transformDirection(Vec3<T>  &v) const 
		{
			Vec4f v2 = Vec4f(v.x(),v.y(),v.z(),0);
			transform(v2);
			v.set(v2.x(),v2.y(),v2.z()); 
		}

		// INPUT / OUTPUT
		void write(FILE *F = stdout) const
		{
			assert (F != NULL);
			for (int y = 0; y < 4; y++) {
				for (int x = 0; x < 4; x++) {
					T tmp = data[y][x];
					if (fabs(tmp) < 0.00001) tmp = 0;
					fprintf (F, "%12.6f ", tmp);
				}
				fprintf (F,"\n"); 
			} 
		}

		void write3x3(FILE *F = stdout) const 
		{
			assert (F != NULL);
			for (int y = 0; y < 4; y++)
			{
				if (y == 2) continue;
				for (int x = 0; x < 4; x++) {
					if (x == 2) continue;
					T tmp = data[y][x];
					if (fabs(tmp) < 0.00001) tmp = 0;
					fprintf (F, "%12.6f ", tmp);
				}
				fprintf (F,"\n"); 
			} 
		}

		void read(FILE *F) 
		{
			assert (F != NULL);
			for (int y = 0; y < 4; y++)
			{
				for (int x = 0; x < 4; x++) 
				{
					int scanned = fscanf (F,"%f",&data[y][x]);
					assert (scanned == 1);
				}
			} 
		}

		void read3x3(FILE *F)
		{
			assert (F != NULL);
			clear();
			for (int y = 0; y < 4; y++)
			{
				if (y == 2) continue;
				for (int x = 0; x < 4; x++) 
				{
					if (x == 2) continue;
					int scanned = fscanf (F,"%f",&data[y][x]);
					assert (scanned == 1);
				} 
			} 
		}

	private:
		// REPRESENTATION
		Vec4<T> data[4];
	};

	typedef Matrix4<int> Matrix4i;
	typedef Matrix4<float> Matrix4f;
	typedef Matrix4<double> Matrix4d;
}