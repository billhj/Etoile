/**
* Copyright(C) 2009-2012                
* @author Jing HUANG
* @file Controller.h
* @brief 
* @date 1/2/2011
*/

#pragma once
#include <vector>
#include "math/Vec3.h"

namespace Etoile
{
	class Controller
	{
	public:
		Controller(const std::string& name): _name(name)
		{
		}

		~Controller(){}
		const std::string& getName() const{ return _name;}
		virtual void apply() = 0;

	protected:
		std::string _name;
	};
}