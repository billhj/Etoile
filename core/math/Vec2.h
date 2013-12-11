/**
* Copyright(C) 2009-2012                
* @author Jing HUANG
* @file Vec2.h
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
	class Vec2
	{

	public:
		// CONSTRUCTORS & DESTRUCTOR
		Vec2() 
		{ 
			_data[0] = _data[1] = 0; 
		}

		Vec2(T f) 
		{ 
			_data[0] = _data[1] = f; 
		}

		Vec2(const Vec2 &V) 
		{
			_data[0] = V._data[0];
			_data[1] = V._data[1]; 
		}

		Vec2(T d0, T d1) 
		{
			_data[0] = d0;
			_data[1] = d1; 
		}

		~Vec2() { }

		// ACCESSORS
		void get(T &d0, T &d1) const
		{
			d0 = _data[0];
			d1 = _data[1];
		}

		const T& operator[](int i) const 
		{ 
			assert (i >= 0 && i < 2);
			return _data[i]; 
		}

		T& operator[](int i) 
		{ 
			assert (i >= 0 && i < 2);
			return _data[i]; 
		}

		bool operator <(const Vec2& v) const
		{
			if(x() == v.x())
			{
				return y() < v.y();
			}
			else
			{
				return x() < v.x();
			}
		}

		bool operator >(const Vec2& v) const
		{
			if(x() == v.x())
			{
				return y() > v.y();
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

		T x() const { return _data[0]; }
		T y() const { return _data[1]; }

		T length() const 
		{
			T l = (double)sqrt( _data[0] * _data[0] + _data[1] * _data[1] );
			return l;
		}

		// MODIFIERS
		void set(T d0, T d1) 
		{
			_data[0] = d0;
			_data[1] = d1;
		}

		void scale(T d0, T d1) 
		{
			_data[0] *= d0;
			_data[1] *= d1;
		}

		void divide(T d0, T d1)
		{
			_data[0] /= d0;
			_data[1] /= d1;
		}

		void negate() 
		{
			_data[0] = -_data[0];
			_data[1] = -_data[1]; 
		}

		void normalize() 
		{
			T l = length();
			if (l > 0) 
			{
				_data[0] /= l;
				_data[1] /= l;
			}
		}

		Vec2 normalized() 
		{
			T l = length();
			if (l > 0) 
			{
				Vec2 res(_data[0] / l, _data[1] / l); 
				return res;
			}
			return Vec2();
		}

		Vec2 precise(int n)
		{
			Vec2 out;
			int cut = pow(double(10),n);
			int a = _data[0] * cut + 0.5;
			int b = _data[1] * cut + 0.5;
			out.set(double(a)/double(cut), double(b)/double(cut));
			return out;
		}

		// OVERLOADED OPERATORS
		Vec2 operator-() const 
		{
			return Vec2(-_data[0], -_data[1]); 
		}

		Vec2 operator*(T f) const
		{
			return Vec2(_data[0] * f, _data[1] * f); 
		}

		Vec2 operator/(T f) const
		{
			return Vec2(_data[0]/f, _data[1]/f);
		}

		Vec2& operator*=(T f) 
		{
			_data[0] *= f;
			_data[1] *= f;
			return (*this); 
		}

		Vec2& operator/=(T f) 
		{
			_data[0] /= f;
			_data[1] /= f;
			return *this; 
		}

		Vec2& operator=(const Vec2 &V) 
		{
			_data[0] = V._data[0];
			_data[1] = V._data[1];
			return *this; 
		}

		Vec2 operator+(const Vec2 &f) 
		{
			return Vec2(_data[0] + f.x(), _data[1] + f.y());
		} 

		Vec2 operator-(const Vec2 &f) 
		{
			return Vec2(_data[0] - f.x(), _data[1] - f.y());
		} 

		T operator*(const Vec2 &V)const 
		{
			return _data[0] * V._data[0] +
				_data[1] * V._data[1]; 
		}

		friend Vec2 operator+(const Vec2 &f1, const Vec2 &f2) 
		{
			return Vec3(f1.x() + f2.x(), f1.y() + f2.y());
		} 

		friend Vec2 operator-(const Vec2 &f1, const Vec2 &f2) 
		{
			return Vec3(f1.x() - f2.x(), f1.y() - f2.y());
		} 

		friend T operator*(const Vec2 &f1, const Vec2 &f2) 
		{
			return f1.x() * f2.x() +
				f1.y() * f2.y(); 
		}

		int operator==(const Vec2 &V) const 
		{
			return ((_data[0] == V._data[0]) &&
				(_data[1] == V._data[1]));
		}

		int operator!=(const Vec2 &V) 
		{
			return ((_data[0] != V._data[0]) ||
				(_data[1] != V._data[1])); 
		}

		Vec2& operator+=(const Vec2 &V) 
		{
			_data[0] += V._data[0];
			_data[1] += V._data[1];
			return *this;
		}

		Vec2& operator-=(const Vec2 &V) 
		{
			_data[0] -= V._data[0];
			_data[1] -= V._data[1];
			return *this; 
		}

		// OPERATIONS
		T dot2(const Vec2 &V) const 
		{
			return _data[0] * V._data[0] + _data[1] * V._data[1];
		}
		
		// STATIC OPERATIONS
		static void add(Vec2 &a, const Vec2 &b, const Vec2 &c ) 
		{
			a._data[0] = b._data[0] + c._data[0];
			a._data[1] = b._data[1] + c._data[1]; 
		}

		static void sub(Vec2 &a, const Vec2 &b, const Vec2 &c ) 
		{
			a._data[0] = b._data[0] - c._data[0];
			a._data[1] = b._data[1] - c._data[1];
		}

		static void copyScale(Vec2 &a, const Vec2 &b, T c ) 
		{
			a._data[0] = b._data[0] * c;
			a._data[1] = b._data[1] * c;
		}

		static void addScale(Vec2 &a, const Vec2 &b, const Vec2 &c, T d )
		{
			a._data[0] = b._data[0] + c._data[0] * d;
			a._data[1] = b._data[1] + c._data[1] * d; 
		}

		static void average(Vec2 &a, const Vec2 &b, const Vec2 &c )
		{
			a._data[0] = (b._data[0] + c._data[0]) * 0.5f;
			a._data[1] = (b._data[1] + c._data[1]) * 0.5f; 
		}

		static void weightedSum(Vec2 &a, const Vec2 &b, T c, const Vec2 &d, T e ) 
		{
			a._data[0] = b._data[0] * c + d._data[0] * e;
			a._data[1] = b._data[1] * c + d._data[1] * e; 
		}

		friend ostream& operator<<(ostream& o, const Vec2& v)
		{
			return o << v[0] << '\t' << v[1] << '\t';
		}

		// INPUT / OUTPUT
		void write(FILE *F = stdout) 
		{
			fprintf (F, "%f %f\n",_data[0],_data[1]);
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
		T _data[2];

	};

	typedef Vec2<int> Vec2i;
	typedef Vec2<float> Vec2f;
	typedef Vec2<double> Vec2d;
}

