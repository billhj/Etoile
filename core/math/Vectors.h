/**
* Copyright(C) 2009-2012                
* @author Jing HUANG
* @file Vectors.h
* @brief 
* @date 1/2/2011
*/

#pragma once
#include "Vec2.h"
#include "Vec3.h"
#include "Vec4.h"
#include "VecN.h"
#include <vector>

namespace Etoile
{
	class VecConverter
	{
	public:
		static Vec4f convertFromDoubleToFloat(const Vec4d& v)
		{
			return Vec4f((float)v[0], (float)v[1], (float)v[2], (float)v[3]);
		}

		static std::vector<Vec4f> convertFromDoubleToFloat(const std::vector<Vec4d>& v)
		{
			std::vector<Vec4f> vr;
			for(unsigned int i = 0; i < v.size(); ++i)
			{
				vr.push_back(convertFromDoubleToFloat(v[i]));
			}
			return vr;
		}
	};
}