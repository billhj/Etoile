/**
* Copyright(C) 2009-2012                
* @author Jing HUANG
* @file HMMObservation.cpp
* @brief 
* @date 1/2/2011
*/

#include "HMMObservation.h"

namespace Etoile
{
	HMMObservation::HMMObservation(void)
	{
	}


	HMMObservation::~HMMObservation(void)
	{
	}

	void HMMObservation::setId(const std::string& id)
	{
		_id = id;
	}

	std::string HMMObservation::getId()
	{
		return _id;
	}
}