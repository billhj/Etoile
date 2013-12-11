/**
* Copyright(C) 2009-2012                
* @author Jing HUANG
* @file ReadImage.h
* @brief 
* @date 1/2/2011
*/

#pragma once
#include "Image.h"

namespace Etoile
{
	class ReadImage
	{
	public:
		static bool loadImageFromFile(const std::string& filepath, Image& image);
	};

}
