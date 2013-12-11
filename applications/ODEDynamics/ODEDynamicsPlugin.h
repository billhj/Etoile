/**
* Copyright(C) 2009-2012                
* @author Jing HUANG
* @file ODEDynamicsPlugin.h
* @brief 
* @date 1/2/2011
*/

#pragma once
#include "module/EPlugin.h"
#include "module/flowgraph/SceneObjectSocket.h"
#include "ODEPhysicsWorld.h"
#include "ODEDynamicsPanel.h"

PLUGIN_DLL void loadDescription(Etoile::EPluginsDescription** descrip);
Etoile::EPlugin* loadODEDynamicsPlugin();

namespace Etoile
{
	struct ODEDynamicsPluginDescription : public EPluginsDescription
	{
		ODEDynamicsPluginDescription()
		{
			this->menu = "Plugins";
			this->name = "OpenDynamics";
			this->plugins_load_functions.push_back(loadODEDynamicsPlugin);
			this->plugins_names.push_back("OpenDynamics");
		}
	};

	class ODESceneObjectInputSocket : public SceneObjectInputSocket
	{
	public:
		ODESceneObjectInputSocket(const std::string& name = "SceneObject");
		virtual void perform(SceneObject* signal) override;
		virtual void retrieve(SceneObject* signal) override;
	};

	class ODEPlaneInputSocket : public PlaneInputSocket
	{
	public:
		ODEPlaneInputSocket(const std::string& name = "Plane");
		virtual void perform(Plane* signal) override;
		virtual void retrieve(Plane* signal) override;
	};

	class ODEDynamicsPlugin : public EPlugin, public SocketNode 
	{
	public:
		ODEDynamicsPlugin(const std::string& name);
		virtual ~ODEDynamicsPlugin();
		virtual void init() override;
		virtual void apply() override;
		virtual void release() override;
		ODEPhysicsWorld * getPhysicsWorld(){return &world;}
	private:
		ODEDynamicsPanel _panel;
		ODEPhysicsWorld world;
		ODESceneObjectInputSocket* _pSceneobjectInput;
		ODEPlaneInputSocket* _pPlaneInput;
	};

}