/**
* Copyright(C) 2009-2012                
* @author Jing HUANG
* @file HMMState.h
* @brief 
* @date 1/2/2011
*/

#pragma once
#include <string>

namespace Etoile
{
	class HMMState
	{
	public:
		HMMState(void);
		~HMMState(void);
		void setId(const std::string& id);
		std::string getId();
	private:
		std::string _id;
	};
}
