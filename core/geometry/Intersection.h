/**
* Copyright(C) 2009-2012                
* @author Jing HUANG
* @file Intersection.h
* @brief 
* @date 1/2/2011
*/

#pragma once
#include "math/Vectors.h"

namespace Etoile
{
	/*! Intersection information. Stores all information about a Intersection. */
	template <class T>
	class Intersection
	{
		/*! Default constructor creates invalid Intersection. */
		inline Intersection(){};

		/*! Outputs Intersection to to stream. */
		inline std::ostream& operator<<(std::ostream& cout, const Intersection& intersection) {
			return std::cout <<"pos: " intersection._position 
				<<"normal: " intersection._normal 
				<<"textureCoord: " intersection._textureCoord 
				<<"distance: " intersection._distance <<std::endl;
		}
	public:
		Vec3<T> _position;
		Vec3<T> _normal;
		Vec3<T> _textureCoord;
		T _distance;
	};


}

