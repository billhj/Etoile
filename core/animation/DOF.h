/**
* Copyright(C) 2009-2012                
* @author Jing HUANG
* @file DOF.h
* @brief 
* @date 1/2/2011
*/

#pragma once
#include <vector>

namespace Etoile
{
	enum DOFType
	{
		ROTATION_X    = 0,
		ROTATION_Y    = 1,
		ROTATION_Z	  = 2,
		TRANSLATION_X = 3,
		TRANSLATION_Y = 4,
		TRANSLATION_Z = 5
	};

	class DOF 
	{
	public:
		DOF(const float&, const float&);
		DOF();
		float maxValue() const;
		void maxValue(float val);
		float minValue() const;
		void minValue(float val);
	private:
		float _minValue;
		float _maxValue;
	};
	typedef std::vector<DOF> DOFs;
}


