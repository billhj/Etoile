/**
* Copyright(C) 2009-2012                
* @author Jing HUANG
* @file HMMState.cpp
* @brief 
* @date 1/2/2011
*/

#include "HMMState.h"

namespace Etoile
{
	HMMState::HMMState(void)
	{
	}


	HMMState::~HMMState(void)
	{
	}


	void HMMState::setId(const std::string& id)
	{
		_id = id;
	}

	std::string HMMState::getId()
	{
		return _id;
	}
}