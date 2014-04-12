/**
* Copyright(C) 2009-2012                
* @author Jing HUANG
* @file EtoileImageToTexturePlugin.cpp
* @brief 
* @date 1/2/2011
*/

#include "EtoileImageToTexturePlugin.h"
#include "util/File.h"

#include "QtTextureLoader.h"


/**
* @brief For tracking memory leaks under windows using the crtdbg
*/
#if ( defined( _DEBUG ) || defined( DEBUG ) ) && defined( _MSC_VER )
#define _CRTDBG_MAP_ALLOC
#include <crtdbg.h>

#define DEBUG_NEW new( _NORMAL_BLOCK, __FILE__, __LINE__ )
#define new DEBUG_NEW
#endif

Etoile::EPlugin* loadEtoileImageToTexturePlugin()
{
	return new Etoile::EtoileImageToTexturePlugin("ImageToTexture");
}

namespace Etoile
{

	ImageToTextureStringInputSocket::ImageToTextureStringInputSocket(const std::string& name) : StringInputSocket(name)
	{
	}

	
	void ImageToTextureStringInputSocket::perform(std::string* signal)
	{
		if(signal == NULL) return;
		EtoileImageToTexturePlugin* plugin = (EtoileImageToTexturePlugin*)(this->getNode());
		plugin->openFile(*signal);
	}

	void ImageToTextureStringInputSocket::retrieve(std::string* signal)
	{
		EtoileImageToTexturePlugin* plugin = (EtoileImageToTexturePlugin*)(this->getNode());
		plugin->openFile("");
	}

	ImageToTextureOutputSocket::ImageToTextureOutputSocket(const std::string& name) : TextureOutputSocket(name)
	{
	}


	EtoileImageToTexturePlugin::EtoileImageToTexturePlugin(const std::string& name): EPlugin(), SocketNode()
	{
		this->getType()._description = "ImageToTexture";
		this->getType()._name = name;
		this->getType()._w = 80;
		this->getType()._h = 60;
		this->getType()._color._r = 120;
		this->getType()._color._g = 240;
		this->getType()._color._b = 250;
		this->getType()._color._a = 240;
		_pInput = new ImageToTextureStringInputSocket();
		this->addInputSocket(_pInput);
		_pOutput = new ImageToTextureOutputSocket();
		this->addOutputSocket(_pOutput);
	}

	EtoileImageToTexturePlugin::~EtoileImageToTexturePlugin()
	{

	}

	void EtoileImageToTexturePlugin::openFile(const std::string& filename)
	{
		if(filename.empty()) return;
		std::string ext = File::getFileExtension(filename);
		Mesh *mesh = new Mesh(filename);
		
		QtTextureLoader textureloader;
			
		Texture* t = textureloader.loadFromFile(filename);
		_pOutput->set(t);
		_pOutput->signalEmit(t);
	}
}