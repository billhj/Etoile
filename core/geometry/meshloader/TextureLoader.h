/**
* Copyright(C) 2009-2012                
* @author Jing HUANG
* @file TextureLoader.h
* @brief 
* @date 1/2/2011
*/

#pragma once
#include <string>

namespace Etoile
{
	class TextureLoader 
	{
	public:
		TextureLoader(){}
		~TextureLoader(){}
		virtual Texture* loadFromFile(const std::string& fileName) = 0;
	
	};

}
