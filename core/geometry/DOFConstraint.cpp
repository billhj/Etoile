/**
* Copyright(C) 2009-2012                
* @author Jing HUANG
* @file DOF.cpp
* @brief 
* @date 1/2/2011
*/

#include "DOFConstraint.h"

namespace Etoile
{
	DOFConstraint::DOFConstraint(const float& minValue, const float& maxValue)
		: _minValue(minValue), _maxValue(maxValue)
	{ }

	DOFConstraint::DOFConstraint(Vec3f axis, const float& minValue, const float& maxValue)
		: _axis(axis), _minValue(minValue), _maxValue(maxValue)
	{
	
	}

	DOFConstraint::DOFConstraint()
	{
		_minValue = -100000;
		_maxValue = 100000;
	}

	float DOFConstraint::maxValue() const 
	{ 
		return _maxValue; 
	}

	void DOFConstraint::maxValue(float val) 
	{ 
		_maxValue = val; 
	}

	float DOFConstraint::minValue() const 
	{
		return _minValue; 
	}

	void DOFConstraint::minValue(float val) 
	{
		_minValue = val; 
	}
}
