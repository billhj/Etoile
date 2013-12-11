/**
* Copyright(C) 2009-2012                
* @author Jing HUANG
* @file VecN.h
* @brief 
* @date 1/2/2011
*/

#pragma once
#include <cstring>

namespace Etoile
{
	template <class T>
	class VecN
	{
	public:
		VecN(): _size(0),
			elements(NULL)
		{

		}

		VecN(unsigned long size)
			: elements(NULL)
		{
			setSize(size);
			makeZero();
		}

		VecN(unsigned long size, T* values)
			: elements(NULL)
		{
			setSize(size);
			makeZero();
			setElements(_size, values);
		}

		VecN(const VecN& v)
		{
			elements = NULL;
			setSize(v.getSize());
			makeZero();
			setElements(_size, v.elements);
		}

		~VecN()
		{
			if(elements != NULL)
			{
				delete elements;
			}

			elements = NULL;
		}

		VecN& operator=(const VecN& v)
		{
			if(elements != NULL)
			{
				delete []elements;
			}

			setSize(v.getSize());
			makeZero();
			setElements(_size, v.elements);

			return *this;
		}

		T& operator[](unsigned long pos)
		{
			assert(pos < _size);
			return elements[pos];
		}
		T operator[](unsigned long pos) const
		{
			assert(pos < _size);
			return elements[pos];
		}

		//VecN operator+(const VecN&);
		//VecN operator+(T scalar);
		//VecN operator-(const VecN&);
		//VecN operator-(T scalar);

		//VecN& operator+=(const VecN&);
		//VecN& operator+=(T scalar);
		//VecN& operator-=(const VecN&);
		//VecN& operator-=(T scalar);

		//VecN& operator*=(T scalar);
		//VecN operator*(T scalar);

		unsigned long getSize() const
		{
			return _size;
		}
		void setSize(unsigned long size)
		{
			this->_size = size;

			if(elements)
			{
				delete []elements;
			}

			this->elements = new T[size];
		}
		void setElements(unsigned long size, T* values)
		{
			memcpy(elements, values, size * sizeof(T));
		}
		void makeZero()
		{
			assert(elements != NULL);
			for(unsigned long i = 0; i < _size; i++)
			{
				elements[i] = 0.0;
			}
		}

		friend std::ostream& operator <<( std::ostream& o, const VecN& v )
		{
			for(unsigned long i = 0; i < v.getSize(); i++)
			{
				o <</* std::right << std::setw(12) <<*/ v.elements[i] << " ";
			}

			return o;
		}

	private:
		unsigned long _size;
		T* elements;
	};

	typedef VecN<int> VecNi;
	typedef VecN<float> VecNf;
	typedef VecN<double> VecNd;
}