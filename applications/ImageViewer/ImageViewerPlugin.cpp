#include "ImageViewerPlugin.h"
/**
* Copyright(C) 2009-2012                
* @author Jing HUANG
* @file ImageViewerPlugin.cpp
* @brief 
* @date 1/2/2011
*/

void loadDescription(Etoile::EPluginsDescription** descrip)
{
	descrip[0] = new Etoile::ImageViewerPluginDescription();
}

Etoile::EPlugin* loadImageViewerPlugin()
{
	return new Etoile::ImageViewerPlugin("ImageViewer");
}

namespace Etoile
{
	ImageStringSocket::ImageStringSocket(const std::string& name): StringInputSocket(name)
	{
	}
	void ImageStringSocket::perform(std::string* signal) 
	{
		if(signal == NULL) return;
		ImageViewerPlugin* plugin = (ImageViewerPlugin*)(this->getNode());
		plugin->getViewer()->openFile(*signal);
	}

	ImageViewerImageInputSocket::ImageViewerImageInputSocket(const std::string& name) : ImageInputSocket(name)
	{
	}

	void ImageViewerImageInputSocket::perform(Etoile::Image* signal)
	{
		ImageViewerPlugin* plugin = (ImageViewerPlugin*)(this->getNode());
		plugin->getViewer()->openImage(signal);
	}


	ImageViewerPlugin::ImageViewerPlugin(const std::string& name): EPlugin(),SocketNode()
	{
		this->getType()._description = "ImageViewer";
		this->getType()._name = name;
		this->getType()._w = 80;
		this->getType()._h = 80;
		this->getType()._color._r = 130;
		this->getType()._color._g = 100;
		this->getType()._color._b = 150;
		this->getType()._color._a = 255;

		_pimageviewer = new ImageViewer();

		_imagesocket = new ImageViewerImageInputSocket("ImageInput");
		this->addInputSocket(_imagesocket);

		_stringsocket = new ImageStringSocket();
		this->addInputSocket(_stringsocket);
		apply();
	}

	ImageViewerPlugin::~ImageViewerPlugin()
	{
		release();
	}
	void ImageViewerPlugin::init()
	{

	}

	void ImageViewerPlugin::apply()
	{
		_pimageviewer->setVisible(true);
		_pimageviewer->raise();
	}

	void ImageViewerPlugin::release()
	{
		_pimageviewer->close();
	}

}
