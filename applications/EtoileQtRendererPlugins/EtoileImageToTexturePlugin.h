/**
* Copyright(C) 2009-2012                
* @author Jing HUANG
* @file EtoileImageToTexturePlugin.h
* @brief 
* @date 1/2/2011
*/

#pragma once
#include "module/EPlugin.h"
#include "module/flowgraph/GeneralSocket.h"
#include "module/flowgraph/TextureSocket.h"
#include "geometry/Mesh.h"

Etoile::EPlugin* loadEtoileImageToTexturePlugin();

namespace Etoile
{
	class ImageToTextureStringInputSocket : public StringInputSocket
	{
	public:
		ImageToTextureStringInputSocket(const std::string& name = "ImageFileName");
		virtual void perform(std::string* signal) override;
		virtual void retrieve(std::string* signal) override;
	};


	class ImageToTextureOutputSocket : public TextureOutputSocket
	{
	public:
		ImageToTextureOutputSocket(const std::string& name = "Texture");
	};

	class EtoileImageToTexturePlugin : public EPlugin, public SocketNode 
	{
	public:
		EtoileImageToTexturePlugin(const std::string& name);
		~EtoileImageToTexturePlugin();
		void openFile(const std::string& filename);
	protected:
		ImageToTextureStringInputSocket *_pInput;
		ImageToTextureOutputSocket *_pOutput;
	};
}

