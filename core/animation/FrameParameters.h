/**
* Copyright(C) 2009-2012                
* @author Jing HUANG
* @file FrameParameters.h
* @brief 
* @date 1/2/2011
*/

#pragma once
#include <vector>
#include <string>
#include "math/Vec3.h"
#include "math/Quaternion.h"

namespace Etoile
{

	struct FrameParameters
	{
		Quaternionf _localRotation;
		Vec3f _translation;  
		Vec3f _scale;   
	};

}
