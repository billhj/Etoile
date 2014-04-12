/**
* Copyright(C) 2009-2012                
* @author Jing HUANG
* @file MeshViewerPlugin.h
* @brief 
* @date 1/2/2011
*/

#include "EtoileMeshViewer.h"
#include "module/EPlugin.h"
#include "module/flowgraph/GeneralSocket.h"

PLUGIN_DLL void loadDescription(Etoile::EPluginsDescription** descrip);
Etoile::EPlugin* loadMeshViewerPlugin();

namespace Etoile
{
	struct MeshViewerPluginDescription : public EPluginsDescription
	{
		MeshViewerPluginDescription()
		{
			this->menu = "Plugins";
			this->name = "MeshViewer";
			this->plugins_load_functions.push_back(loadMeshViewerPlugin);
			this->plugins_names.push_back("MeshViewer");
		}
	};

	class MeshStringSocket : public StringInputSocket
	{
	public:
		MeshStringSocket(const std::string& name = "MeshFileName");
		virtual void perform(std::string* signal) override;
		virtual void retrieve(std::string* signal) override
		{
			if(signal == NULL) return;
		}
	};

	class MeshViewerMeshInputSocket : public MeshInputSocket
	{

	public:
		MeshViewerMeshInputSocket(const std::string& name = "Mesh");
		virtual void perform(Etoile::Mesh* signal) override;
		virtual void retrieve(Etoile::Mesh* signal) override
		{
			GLSceneRenderPass* plugin = (GLSceneRenderPass*)(this->getNode());
			std::vector<RenderUnit*>& units = plugin->getRenderUnits();
			for(unsigned int i = 0; i < units.size(); ++i)
			{
				GLSceneRenderUnit* unit = dynamic_cast<GLSceneRenderUnit*>(units[i]);
				if(unit != NULL)
				{
					Scene* scene = unit->getScene();
					for(unsigned int  j = 0; j < scene->getSceneNodes().size(); ++j)
					{
						SceneNode* pnode = scene->getSceneNodes()[j];
						for(int i = 0; i < pnode->getMovableObjects().size(); ++i)
						{
							MovableObject* obj = pnode->getMovableObjects()[i];
							SceneEntity* entity = dynamic_cast<SceneEntity*>(obj);
							if(entity != NULL)
							{
								Mesh* mesh = dynamic_cast<Mesh*>(entity->getMesh());
								if(mesh == signal)
								{
									units.erase(units.begin() + i);
									delete unit;
								}
							}
						}
					}
				}
			}
		}
	};

	class MeshViewerPlugin : public EPlugin, public SocketNode 
	{
	protected:
		EtoileMeshViewer* _pmeshviewer;
		MeshViewerMeshInputSocket* _socket;
	public:
		MeshViewerPlugin(const std::string& name = "MeshViewer");
		virtual ~MeshViewerPlugin();
		virtual void init() override;
		virtual void apply() override;
		virtual void release() override;
		EtoileMeshViewer* getViewer(){return _pmeshviewer;}
	};

}
