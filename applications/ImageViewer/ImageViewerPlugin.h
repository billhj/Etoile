/**
* Copyright(C) 2009-2012                
* @author Jing HUANG
* @file ImageViewerPlugin.h
* @brief 
* @date 1/2/2011
*/

#pragma once
#include "module/EPlugin.h"
#include "ImageViewer.h"
#include "module/flowgraph/GeneralSocket.h"

PLUGIN_DLL void loadDescription(Etoile::EPluginsDescription** descrip);
Etoile::EPlugin* loadImageViewerPlugin();

namespace Etoile
{

	struct ImageViewerPluginDescription : public EPluginsDescription
	{
		ImageViewerPluginDescription()
		{
			this->menu = "Plugins";
			this->name = "ImageViewer";
			this->plugins_load_functions.push_back(loadImageViewerPlugin);
			this->plugins_names.push_back("ImageViewer");
		}
	};


	class ImageStringSocket : public StringInputSocket
	{
	public:
		ImageStringSocket(const std::string& name = "FileName");
		virtual void perform(std::string* signal) override;
		virtual void retrieve(std::string* signal) override{}
	};

	class ImageViewerImageInputSocket: public ImageInputSocket
	{		
	public:
		ImageViewerImageInputSocket(const std::string& name);
		virtual void perform(Etoile::Image* signal) override;
		virtual void retrieve(Etoile::Image* signal) override{}
	};

	class ImageViewerPlugin : public EPlugin, public SocketNode 
	{
	protected:
		ImageViewer* _pimageviewer;
		ImageViewerImageInputSocket* _imagesocket;
		ImageStringSocket* _stringsocket;
	public:
		ImageViewerPlugin(const std::string& name);
		virtual ~ImageViewerPlugin();
		virtual void init() override;
		virtual void apply() override;
		virtual void release() override;
		ImageViewer* getViewer(){return _pimageviewer;}
	};

}

