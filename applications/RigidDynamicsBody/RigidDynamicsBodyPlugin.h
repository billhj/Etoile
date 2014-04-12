/**
* Copyright(C) 2009-2012                
* @author Jing HUANG
* @file RigidDynamicsBodyPlugin.h
* @brief 
* @date 1/2/2011
*/

#pragma once
#include "module/EPlugin.h"
#include "module/flowgraph/SceneNodeSocket.h"
#include <QTimer>
#include <QTime>

PLUGIN_DLL void loadDescription(Etoile::EPluginsDescription** descrip);
Etoile::EPlugin* loadRigidDynamicsBodyPlugin();

namespace Etoile
{
	struct RigidDynamicsBodyPluginDescription : public EPluginsDescription
	{
		RigidDynamicsBodyPluginDescription()
		{
			this->menu = "Plugins";
			this->name = "RigidDynamicsBody";
			this->plugins_load_functions.push_back(loadRigidDynamicsBodyPlugin);
			this->plugins_names.push_back("RigidDynamicsBody");
		}
	};

	class RDBSceneInputSocket : public SceneInputSocket
	{
	public:
		RDBSceneInputSocket(const std::string& name = "Scene");
		virtual void perform(Scene* signal) override;
		virtual void retrieve(Scene* signal) override;
	};

	/*class ODEPlaneInputSocket : public PlaneInputSocket
	{
	public:
		ODEPlaneInputSocket(const std::string& name = "Plane");
		virtual void perform(Plane* signal) override;
		virtual void retrieve(Plane* signal) override;
	};*/

	class RigidDynamicsBodyPlugin : public QObject, public EPlugin, public SocketNode
	{
		Q_OBJECT
	public:
		RigidDynamicsBodyPlugin(const std::string& name);
		virtual ~RigidDynamicsBodyPlugin();
		virtual void init() override;
		virtual void apply() override;
		virtual void release() override;
		void setScene(Scene* scene);
		Scene* getScene(){return _pScene;}
		void buildSkeleton();
	public slots:
		void update();
	private:
		RDBSceneInputSocket* _pSceneInput;
		Scene* _pScene;
	};

}