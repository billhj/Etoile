/**
* Copyright(C) 2009-2012                
* @author Jing HUANG
* @file ContainmentTests.h
* @brief 
* @date 1/2/2011
*/

#pragma once
#include <vector>
#include "../math/Vectors.h"

namespace Etoile
{
	template <class T>
    class ContainmentTests
    {
	public:
		static bool pointInsideTriangle(Vec2<T> point, Vec2<T> p0, Vec2<T> p1, Vec2<T> p2)
        {
            //
            // Implementation based on http://www.blackpawn.com/texts/pointinpoly/default.html.
            //
            Vec2<T> v0 = (p1 - p0);
            Vec2<T> v1 = (p2 - p0);
            Vec2<T> v2 = (point - p0);

            T dot00 = v0.dot2(v0);
            T dot01 = v0.dot2(v1);
            T dot02 = v0.dot2(v2);
            T dot11 = v1.dot2(v1);
            T dot12 = v1.dot2(v2);

            T q = 1.0 / (dot00 * dot11 - dot01 * dot01);
            T u = (dot11 * dot02 - dot01 * dot12) * q;
            T v = (dot00 * dot12 - dot01 * dot02) * q;

            return (u > 0) && (v > 0) && (u + v < 1);
        }

        /// <summary>
        /// The pyramid's base points should be in counterclockwise winding order.
        /// </summary>
        static bool pointInsideThreeSidedInfinitePyramid(Vec3<T> point, Vec3<T> pyramidApex, Vec3<T> pyramidBase0, Vec3<T> pyramidBase1, Vec3<T> pyramidBase2)
        {
            Vec3<T> v0 = pyramidBase0 - pyramidApex;
            Vec3<T> v1 = pyramidBase1 - pyramidApex;
            Vec3<T> v2 = pyramidBase2 - pyramidApex;

            //
            // Face normals
            //
            Vec3<T> n0 = v1.cross3(v0);
            Vec3<T> n1 = v2.cross3(v1);
            Vec3<T> n2 = v0.cross3(v2);

            Vec3<T> planeToPoint = point - pyramidApex;

            return ((planeToPoint.dot3(n0) < 0) && (planeToPoint.dot3(n1) < 0) && (planeToPoint.dot3(n2) < 0));
        }
    };
}