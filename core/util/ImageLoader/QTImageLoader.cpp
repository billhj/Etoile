/**
* Copyright(C) 2009-2012                
* @author Jing HUANG
* @file QTImageLoader.cpp
* @brief 
* @date 1/2/2011
*/

#include "QTImageLoader.h"
#include <QImage>

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
	QTImageLoader::QTImageLoader()
	{
	}


	bool QTImageLoader::loadImageFromQImage(QImage const& qimage, Image& image)
	{
		image.setWidth(qimage.width());
		image.setHeight(qimage.height());

		std::vector<Vec4f>& data = image.getData();
		data.resize(image.getHeight() * image.getWidth());

		for(int y = 0; y < qimage.height(); ++y)
		{
			for(int x = 0; x < qimage.width(); ++x)
			{
				Vec4f pixel;
				int index = (qimage.height()- y - 1) * qimage.width() + x;
				QRgb rgb = qimage.pixel(x, y);
				pixel[0] = float(qRed(rgb)) / 255.0f;
				pixel[1] = float(qGreen(rgb)) / 255.0f;
				pixel[2] = float(qBlue(rgb)) / 255.0f;
				pixel[3] = float(qAlpha(rgb)) / 255.0f;
				data[index] = pixel;
			}
		}

		return true;
	}

	bool QTImageLoader::loadImageFromFile(const std::string& filepath, Image& image)
	{
		QImage qimage;
		bool b_qimage = qimage.load(QString::fromStdString(filepath));
		bool b_image = loadImageFromQImage(qimage, image);
		return b_qimage && b_image;
	}

}
