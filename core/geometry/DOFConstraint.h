/**
* Copyright(C) 2009-2012                
* @author Jing HUANG
* @file DOFConstraint.h
* @brief 
* @date 1/2/2011
*/

#pragma once
#include <vector>
#include "math/MathHead.h"

namespace Etoile
{
	enum DOFConstraintType
	{
		ROTATION_X    = 0,
		ROTATION_Y    = 1,
		ROTATION_Z	  = 2,
		TRANSLATION_X = 3,
		TRANSLATION_Y = 4,
		TRANSLATION_Z = 5
	};

	class DOFConstraint 
	{
	public:
		DOFConstraint(const float&, const float&);
		DOFConstraint(Vec3f axis, const float&, const float&);
		DOFConstraint();
		float maxValue() const;
		void maxValue(float val);
		float minValue() const;
		void minValue(float val);
		Vec3f getAxis()
		{
			return _axis;
		}
	private:
		float _minValue;
		float _maxValue;
		Vec3f _axis;
	};
	typedef std::vector<DOFConstraint> DOFConstraints;
}


