/**
* Copyright(C) 2009-2012                
* @author Jing HUANG
* @file MocapViewerPlugin.h
* @brief 
* @date 1/2/2011
*/

#pragma once
#include "module/EPlugin.h"
#include "MocapViewer.h"
#include "module/flowgraph/GeneralSocket.h"


PLUGIN_DLL void loadDescription(Etoile::EPluginsDescription** descrip);
Etoile::EPlugin* loadMocapViewerPlugin();

namespace Etoile
{
	struct MocapViewerPluginDescription : public EPluginsDescription
	{
		MocapViewerPluginDescription()
		{
			this->menu = "Plugins";
			this->name = "MocapViewer";
			this->plugins_load_functions.push_back(loadMocapViewerPlugin);
			this->plugins_names.push_back("MocapViewer");
		}
	};

	class MocapStringSocket : public StringInputSocket
	{
	public:
		MocapStringSocket(const std::string& name = "MocapFileName");
		virtual void perform(std::string* signal) override;
		virtual void retrieve(std::string* signal) override{}
	};

	class MocapViewerPlugin : public EPlugin, public SocketNode 
	{
		MocapViewer* _pMocapviewer;
	public:
		MocapViewerPlugin(const std::string& name);
		virtual ~MocapViewerPlugin();
		virtual void init() override;
		virtual void apply() override;
		virtual void release() override;
		MocapViewer* getViewer(){return _pMocapviewer;}
	};

}


