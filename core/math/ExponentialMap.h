/**
* Copyright(C) 2009-2012                
* @author Jing HUANG
* @file ExponentialMap.h
* @brief 
* @date 1/2/2011
*/

#pragma once
#include "Vec3.h"
#include "Quaternion.h"

namespace Etoile
{
	template<class T>
	class ExponentialMap
	{
	public:
		ExponentialMap()
		{

		}

		ExponentialMap(Vec3<T> axis, T angle)
		{
			_axis = axis.normalized();
			_angle = angle;
		}

		ExponentialMap(Quaternion value)
		{
			_axis = value.axis().normalized();
			_angle = value.angle();
		}

		Quaternion getQuaternion()
		{
			Quaternion q;
			q.setAxisAngle(_axis, _angle);
			return q;
		}

		Vec3<T> operator *(Vec3<T> v)
		{
			return rotate(v);
		}

		ExponentialMap inverse() const
		{
			return ExponentialMap(_axis, -_angle);
		}

		Vec3<T> rotate(const Vec3<T> & v) const
		{
			Vec3<T> r = v * cos(_angle)
				+ _axis.cross3(v) * sin(_angle)
				+ _axis * _axis.dot3(v) * (1 - cos(_angle));
			return r;
		}

		Vec3<T> inverseRotate(const Vec3<T> & v) const
		{
			return inverse().rotate(v);
		}

		void invert()
		{
			_angle = -_angle;
		}
	private:
		Vec3<T> _axis;
		T _angle;

	};
}