/**
* Copyright(C) 2009-2012                
* @author Jing HUANG
* @file HMMObservation.h
* @brief 
* @date 1/2/2011
*/

#pragma once
#include <string>

namespace Etoile
{
	class HMMObservation
	{
	public:
		HMMObservation(void);
		~HMMObservation(void);
		void setId(const std::string& id);
		std::string getId();
	private:
		std::string _id;
	};
}
