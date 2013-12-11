/**
* Copyright(C) 2009-2012                
* @author Jing HUANG
* @file IntersectionTests.h
* @brief 
* @date 1/2/2011
*/

#pragma once
#include <vector>
#include <cmath>
#include "../math/Vec3.h"

namespace Etoile
{
	template <class T>
	class IntersectionTests
	{
	public:
		static bool tryRayPlane(Vec3<T> rayOrigin, Vec3<T> rayDirection, Vec3<T> planeNormal, T planeD, Vec3<T>& intersectionPoint)
		{
			T denominator = planeNormal.dot3(rayDirection);
			if (abs(denominator) < 0.00000000000000000001)
			{
				//
				// Ray is parallel to plane.  The ray may be in the polygon's plane.
				//
				//intersectionPoint = NULL;
				return false;
			}

			T t = (-planeD - planeNormal.dot3(rayOrigin)) / denominator;

			if (t < 0)
			{
				//intersectionPoint = NULL;
				return false;
			}

			Vec3<T> p = rayOrigin + (rayDirection * t);
			intersectionPoint.set(p.x(), p.y(), p.z());
			return true;
		}
	};
}