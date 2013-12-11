/**
* Copyright(C) 2009-2012                
* @author Jing HUANG
* @file Comparable.h
* @brief 
* @date 1/2/2011
*/

#pragma once
#include <vector>

namespace Etoile
{
	
	class Comparable
	{
	public:
		virtual bool operator <(const Comparable& c) const = 0;
	};
}