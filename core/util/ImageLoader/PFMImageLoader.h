/**
* Copyright(C) 2009-2012                
* @author Jing HUANG
* @file PFMImageLoader.h
* @brief 
* @date 1/2/2011
*/

#pragma once
#include <string>
#include "ImageLoader.h"

namespace Etoile
{
	class PFMImageLoader : public ImageLoader
	{
	public:
		PFMImageLoader();
		virtual bool loadImageFromFile(const std::string& filepath, Image& image) override;
	};


}
