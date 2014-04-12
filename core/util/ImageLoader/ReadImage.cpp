/**
* Copyright(C) 2009-2012                
* @author Jing HUANG
* @file ReadImage.cpp
* @brief 
* @date 1/2/2011
*/

#include "ReadImage.h"
#include "PFMImageLoader.h"
#include "QTImageLoader.h"
#include "TGAImageLoader.h"
#include "../File.h"

/**
* @brief For tracking memory leaks under windows using the crtdbg
*/
#if ( defined( _DEBUG ) || defined( DEBUG ) ) && defined( _MSC_VER )
#define _CRTDBG_MAP_ALLOC
#include <crtdbg.h>

#define DEBUG_NEW new( _NORMAL_BLOCK, __FILE__, __LINE__ )
#define new DEBUG_NEW
#endif


namespace Etoile
{

	bool ReadImage::loadImageFromFile(const std::string& filepath, Image& image)
	{
		std::string strname = File::getFileExtension(filepath, true);
		bool b_image = false;
		if(strname.compare("pfm")==0)
		{
			PFMImageLoader loader;
			b_image = loader.loadImageFromFile(filepath, image);

		}else if(strname.compare("tga")==0)
		{
			TGAImageLoader loader;
			b_image = loader.loadImageFromFile(filepath, image);
		}
		else
		{
			QTImageLoader loader;
			b_image = loader.loadImageFromFile(filepath, image);
		}
		return b_image;
	}

}
