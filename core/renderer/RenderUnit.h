/**
* Copyright(C) 2009-2012                
* @author Jing HUANG
* @file RenderUnit.h
* @brief 
* @date 1/2/2011
*/

#pragma once
#include <string>

namespace Etoile
{
	class RenderUnit
	{
	protected:
		std::string _name;
	public:
		RenderUnit(const std::string& name): _name(name)
		{
		}
		virtual void draw() = 0;
		
	};
}
