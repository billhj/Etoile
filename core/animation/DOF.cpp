/**
* Copyright(C) 2009-2012                
* @author Jing HUANG
* @file DOF.cpp
* @brief 
* @date 1/2/2011
*/

#include "DOF.h"

namespace Etoile
{
	DOF::DOF(const float& minValue, const float& maxValue)
		: _minValue(minValue), _maxValue(maxValue)
	{ }

	DOF::DOF()
	{
		_minValue = -100000;
		_maxValue = 100000;
	}

	float DOF::maxValue() const 
	{ 
		return _maxValue; 
	}

	void DOF::maxValue(float val) 
	{ 
		_maxValue = val; 
	}

	float DOF::minValue() const 
	{
		return _minValue; 
	}

	void DOF::minValue(float val) 
	{
		_minValue = val; 
	}
}
