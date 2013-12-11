/**
* Copyright(C) 2009-2012                
* @author Jing HUANG
* @file ImageLoader.h
* @brief 
* @date 1/2/2011
*/

#pragma once
#include "Image.h"

namespace Etoile
{
	class ImageLoader
	{
	public :
		virtual bool loadImageFromFile(const std::string& filepath, Image& image) = 0;
	};
}
