/**
* Copyright(C) 2009-2012                
* @author Jing HUANG
* @file TGAImageLoader.h
* @brief 
* @date 1/2/2011
*/

#pragma once
#include <string>
#include "ImageLoader.h"

namespace Etoile
{
	class TGAImageLoader : public ImageLoader
	{
	public :
		virtual bool loadImageFromFile(const std::string& filepath, Image& image) override;

	};
}
