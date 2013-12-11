/**
* Copyright(C) 2009-2012                
* @author Jing HUANG
* @file IKViewerPlugin.h
* @brief 
* @date 1/2/2011
*/

#pragma once
#include "module/EPlugin.h"
#include "IKApplication.h"
#include "module/flowgraph/SocketNode.h"

PLUGIN_DLL void loadDescription(Etoile::EPluginsDescription** descrip);
Etoile::EPlugin* loadIKViewerPlugin();

namespace Etoile
{
	struct IKViewerPluginDescription : public EPluginsDescription
	{
		IKViewerPluginDescription()
		{
			this->menu = "Plugins";
			this->name = "IKViewer";
			this->plugins_load_functions.push_back(loadIKViewerPlugin);
			this->plugins_names.push_back("IKViewer");
		}
	};

	class IKViewerPlugin : public EPlugin, public SocketNode 
	{
	protected:
		IKApplication* _pAppli;
	public:
		IKViewerPlugin(const std::string& name)
		{
			this->getType()._description = "InverseKinematics";
			this->getType()._name = "IK";
			this->getType()._w = 80;
			this->getType()._h = 80;
			this->getType()._color._r = 230;
			this->getType()._color._g = 100;
			this->getType()._color._b = 150;
			this->getType()._color._a = 255;
			_pAppli = new IKApplication();
			_pAppli->setVisible(false);
		}
		virtual ~IKViewerPlugin(){}
		virtual void init() override{}
		virtual void apply() override
		{
			_pAppli->setVisible(true);
			_pAppli->raise();
		}
		virtual void release() override{_pAppli->close();}
	};

}

