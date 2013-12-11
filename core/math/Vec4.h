/**
* Copyright(C) 2009-2012                
* @author Jing HUANG
* @file Vec4.h
* @brief 
* @date 1/2/2011
*/

#pragma once
#include <iostream>
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <assert.h>
#ifdef USING_BOOST
#include <boost/serialization/serialization.hpp>
#endif

using namespace std;

namespace Etoile
{
	template <class T>
	class Vec4 
	{

	public:
		// CONSTRUCTORS & DESTRUCTOR
		Vec4() 
		{ 
			_data[0] = _data[1] = _data[2] = _data[3] = 0; 
		}

		Vec4(T  f) 
		{ 
			_data[0] = _data[1] = _data[2] = _data[3] = f; 
		}

		Vec4(const Vec4 &V) 
		{
			_data[0] = V._data[0];
			_data[1] = V._data[1];
			_data[2] = V._data[2];
			_data[3] = V._data[3]; 
		}

		Vec4(T  d0, T  d1, T  d2, T  d3) 
		{
			_data[0] = d0;
			_data[1] = d1;
			_data[2] = d2;
			_data[3] = d3; 
		}

		/*Vec4(const Vec3<T> &V, T  w) 
		{
			_data[0] = V.x();
			_data[1] = V.y();
			_data[2] = V.z();
			_data[3] = w; 
		}*/


		~Vec4() {}

		// ACCESSORS
		void get(T  &d0, T  &d1, T  &d2, T  &d3) const 
		{
			d0 = _data[0];
			d1 = _data[1];
			d2 = _data[2];
			d3 = _data[3]; 
		}

		const T & operator[](int i) const 
		{
			assert (i >= 0 && i < 4);
			return _data[i]; 
		}

		T & operator[](int i) 
		{
			assert (i >= 0 && i < 4);
			return _data[i]; 
		}

		bool operator <(const Vec4& v) const
		{
			if(x() == v.x())
			{
				if(y() == v.y())
				{
					if(z() == v.z())
					{
						return w()< v.w();
					}
					else
					{
						return z() < v.z();
					}
				}
				else
				{
					return y() < v.y();
				}
			}
			else
			{
				return x() < v.x();
			}
		}

		bool operator >(const Vec4& v) const
		{
			if(x() == v.x())
			{
				if(y() == v.y())
				{
					if(z() == v.z())
					{
						return w() > v.w();
					}
					else
					{
						return z() > v.z();
					}
				}
				else
				{
					return y() > v.y();
				}
			}
			else
			{
				return x() > v.x();
			}
		}

		T* get_data()
		{
			return &_data[0];
		}

		T  x() const { return _data[0]; }
		T  y() const { return _data[1]; }
		T  z() const { return _data[2]; }
		T  w() const { return _data[3]; }
		T  r() const { return _data[0]; }
		T  g() const { return _data[1]; }
		T  b() const { return _data[2]; }
		T  a() const { return _data[3]; }

		T  length() const 
		{
			T  l = (T )sqrt( _data[0] * _data[0] +
				_data[1] * _data[1] +
				_data[2] * _data[2] +
				_data[3] * _data[3] );
			return l; 
		}

		// MODIFIERS
		void set(T  d0, T  d1, T  d2, T  d3) 
		{
			_data[0] = d0;
			_data[1] = d1;
			_data[2] = d2;
			_data[3] = d3; 
		}
		void scale(T  d0, T  d1, T  d2, T  d3) 
		{
			_data[0] *= d0;
			_data[1] *= d1;
			_data[2] *= d2;
			_data[3] *= d3; 
		}

		void divide(T  d0, T  d1, T  d2, T  d3) 
		{
			_data[0] /= d0;
			_data[1] /= d1;
			_data[2] /= d2;
			_data[3] /= d3;
		}

		void negate() 
		{
			_data[0] = -_data[0];
			_data[1] = -_data[1];
			_data[2] = -_data[2];
			_data[3] = -_data[3]; 
		}

		void normalize() 
		{
			T  l = length();
			if (l > 0) 
			{
				_data[0] /= l;
				_data[1] /= l;
				_data[2] /= l; 
				_data[3] /= l; 
			}
		}

		Vec4 normalized() 
		{
			T l = length();
			if (l > 0) 
			{
				Vec4 res(_data[0] / l, _data[1] / l, _data[2] / l, _data[3] / l ); 
				return res;
			}
			return Vec4();
		}

		void divideByW() 
		{
			if (_data[3] != 0) 
			{
				_data[0] /= _data[3];
				_data[1] /= _data[3];
				_data[2] /= _data[3];
			} else 
			{
				_data[0] = _data[1] = _data[2] = 0; 
			}
			_data[3] = 1;
		}

		Vec4 precise(int n)
		{
			Vec4 out;
			int cut = pow(double(10),n);
			int a = _data[0] * cut + 0.5;
			int b = _data[1] * cut + 0.5;
			int c = _data[2] * cut + 0.5;
			int d = _data[4] * cut + 0.5;
			out.set(double(a)/double(cut), double(b)/double(cut), double(c)/double(cut) , double(d)/double(cut));
			return out;
		}

		// OVERLOADED OPERATORS

		Vec4 operator-() const 
		{
			return Vec4(-_data[0], -_data[1], -_data[2], -_data[3]); 
		}
		
		Vec4 operator*(T  f) const
		{
			return Vec4(_data[0] * f, _data[1] * f, _data[2] * f, _data[3] * f);
		}

		Vec4 operator/(T f) const
		{
			Vec4 v(_data[0]/f, _data[1]/f, _data[2]/f, _data[3]/f);
			return v;
		}

		Vec4& operator*=(T  f) 
		{
			_data[0] *= f;
			_data[1] *= f;
			_data[2] *= f;
			_data[3] *= f;
			return *this; 
		}

		Vec4& operator/=(T  f) 
		{
			_data[0] /= f;
			_data[1] /= f;
			_data[2] /= f;
			_data[3] /= f;
			return *this; 
		}

		Vec4& operator=(const Vec4 &V) 
		{
			_data[0] = V._data[0];
			_data[1] = V._data[1];
			_data[2] = V._data[2];
			_data[3] = V._data[3];
			return *this; 
		}

		Vec4 operator+(Vec4 f) 
		{
			return Vec4(_data[0] + f.x(), _data[1] + f.y(), _data[2] + f.z(), _data[3] + f.w());
		}

		Vec4 operator-(Vec4 f) 
		{
			return Vec4(_data[0] - f.x(), _data[1] - f.y(), _data[2] - f.z(), _data[3] - f.w());
		}

		T operator*(const Vec4 &V)const 
		{
			return _data[0] * V._data[0] +
				_data[1] * V._data[1] +
				_data[2] * V._data[2] +
				_data[3] * V._data[3]; 
		}

		friend Vec4 operator+(const Vec4 &f1, const Vec4 &f2) 
		{
			return Vec4(f1.x() + f2.x(), f1.y() + f2.y(), f1.z() + f2.z(),  f1.w() + f2.w());
		} 

		friend Vec4 operator-(const Vec4 &f1, const Vec4 &f2) 
		{
			return Vec4(f1.x() - f2.x(), f1.y() - f2.y(), f1.z() - f2.z(),  f1.w() - f2.w());
		} 

		friend T operator*(const Vec4 &f1, const Vec4 &f2) 
		{
			return f1.x() * f2.x() +
				f1.y() * f2.y() +
				f1.z() * f2.z() + f1.w() * f2.w(); 
		}

		int operator==(const Vec4 &V) const 
		{
			return ((_data[0] == V._data[0]) &&
				(_data[1] == V._data[1]) &&
				(_data[2] == V._data[2]) &&
				(_data[3] == V._data[3])); 
		}

		int operator!=(const Vec4 &V) const 
		{
			return ((_data[0] != V._data[0]) ||
				(_data[1] != V._data[1]) ||
				(_data[2] != V._data[2]) ||
				(_data[3] != V._data[3]));
		}

		Vec4& operator+=(const Vec4 &V) 
		{
			_data[0] += V._data[0];
			_data[1] += V._data[1];
			_data[2] += V._data[2];
			_data[3] += V._data[3];
			return *this; 
		}

		Vec4& operator-=(const Vec4 &V) 
		{
			_data[0] -= V._data[0];
			_data[1] -= V._data[1];
			_data[2] -= V._data[2];
			_data[3] -= V._data[3];
			return *this; 
		}

		// OPERATIONS
		T  dot2(const Vec4 &V) const 
		{
			return _data[0] * V._data[0] +
				_data[1] * V._data[1];
		}

		T  dot3(const Vec4 &V) const 
		{
			return _data[0] * V._data[0] +
				_data[1] * V._data[1] +
				_data[2] * V._data[2];
		}

		T  dot4(const Vec4 &V) const 
		{
			return _data[0] * V._data[0] +
				_data[1] * V._data[1] +
				_data[2] * V._data[2] +
				_data[3] * V._data[3]; 
		}




		// STATIC OPERATIONS
		static void add(Vec4 &a, const Vec4 &b, const Vec4 &c ) 
		{
			a._data[0] = b._data[0] + c._data[0];
			a._data[1] = b._data[1] + c._data[1];
			a._data[2] = b._data[2] + c._data[2];
			a._data[3] = b._data[3] + c._data[3]; 
		}

		static void sub(Vec4 &a, const Vec4 &b, const Vec4 &c ) 
		{
			a._data[0] = b._data[0] - c._data[0];
			a._data[1] = b._data[1] - c._data[1];
			a._data[2] = b._data[2] - c._data[2];
			a._data[3] = b._data[3] - c._data[3]; 
		}

		static void copyScale(Vec4 &a, const Vec4 &b, T  c ) 
		{
			a._data[0] = b._data[0] * c;
			a._data[1] = b._data[1] * c;
			a._data[2] = b._data[2] * c;
			a._data[3] = b._data[3] * c;
		}

		static void addScale(Vec4 &a, const Vec4 &b, const Vec4 &c, T  d ) 
		{
			a._data[0] = b._data[0] + c._data[0] * d;
			a._data[1] = b._data[1] + c._data[1] * d;
			a._data[2] = b._data[2] + c._data[2] * d;
			a._data[3] = b._data[3] + c._data[3] * d;
		}

		static void average(Vec4 &a, const Vec4 &b, const Vec4 &c ) 
		{
			a._data[0] = (b._data[0] + c._data[0]) * 0.5f;
			a._data[1] = (b._data[1] + c._data[1]) * 0.5f;
			a._data[2] = (b._data[2] + c._data[2]) * 0.5f;
			a._data[3] = (b._data[3] + c._data[3]) * 0.5f; 
		}

		static void weightedSum(Vec4 &a, const Vec4 &b, T  c, const Vec4 &d, T  e ) 
		{
			a._data[0] = b._data[0] * c + d._data[0] * e;
			a._data[1] = b._data[1] * c + d._data[1] * e;
			a._data[2] = b._data[2] * c + d._data[2] * e;
			a._data[3] = b._data[3] * c + d._data[3] * e; 
		}

		static void cross3(Vec4 &c, const Vec4 &v1, const Vec4 &v2) 
		{
			T  x = v1._data[1]*v2._data[2] - v1._data[2]*v2._data[1];
			T  y = v1._data[2]*v2._data[0] - v1._data[0]*v2._data[2];
			T  z = v1._data[0]*v2._data[1] - v1._data[1]*v2._data[0];
			c._data[0] = x; c._data[1] = y; c._data[2] = z; 
		}

		friend ostream& operator<<(ostream& o, const Vec4& v)
		{
			return o << v[0] << '\t' << v[1] << '\t'<< v[2] << '\t'<< v[3] << '\t';
		}

		// INPUT / OUTPUT
		void write(FILE *F = stdout) 
		{
			fprintf (F, "%f %f %f %f\n",_data[0],_data[1],_data[2],_data[3]); 
		}

#ifdef USING_BOOST
		friend class boost::serialization::access;
		template<class Archive>
		void serialize(Archive & ar, const unsigned int version)
		{
			ar & _data;
		}
#endif

	private:

		// REPRESENTATION
		T _data[4];

	};

	typedef Vec4<int> Vec4i;
	typedef Vec4<float> Vec4f;
	typedef Vec4<double> Vec4d;
	typedef Vec4<int> Color4i;
	typedef Vec4<float> Color4f;
	typedef Vec4<double> Color4d;
	typedef Vec4<int> Rgbai;
	typedef Vec4<float> Rgbaf;
	typedef Vec4<double> Rgbad;
}

