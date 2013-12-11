/**
* Copyright(C) 2009-2012                
* @author Jing HUANG
* @file QTImageLoader.h
* @brief 
* @date 1/2/2011
*/

#pragma once
#include <string>
#include "ImageLoader.h"
#include <QImage>

namespace Etoile
{
	class QTImageLoader : public ImageLoader
	{
	public:
		QTImageLoader();
	public :
		virtual bool loadImageFromFile(const std::string& filepath, Image& image) override;

		bool loadImageFromQImage(QImage const& qimage, Image& image);
	};


}
