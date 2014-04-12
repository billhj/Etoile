/**
* Copyright(C) 2009-2012                
* @author Jing HUANG
* @file Comparable.h
* @brief 
* @date 1/2/2011
*/

#pragma once
#include <algorithm>

namespace Etoile
{
	
	/*
	*	Comparable class:
	*	
	**/

	class Comparable
	{
	public:
		Comparable()
		{
			
		}
		~Comparable()
		{
		}

		virtual bool operator <(const Comparable& e) = 0;
		virtual bool operator >(const Comparable& e) = 0;
		virtual bool operator ==(const Comparable& e) = 0;
	};

}