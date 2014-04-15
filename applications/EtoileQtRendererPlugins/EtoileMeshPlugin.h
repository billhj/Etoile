/**
* Copyright(C) 2009-2012                
* @author Jing HUANG
* @file EtoileMeshPlugin.h
* @brief 
* @date 1/2/2011
*/

#pragma once
#include "module/EPlugin.h"
#include "module/flowgraph/GeneralSocket.h"
#include "module/flowgraph/MeshSocket.h"
#include "module/flowgraph/SceneObjectSocket.h"
#include "geometry/Mesh.h"
#include "MeshEditing.h"
#include "renderer/MeshRenderUnit.h"

Etoile::EPlugin* loadEtoileMeshPlugin();

namespace Etoile
{
	class MeshPluginStringSocket : public StringInputSocket
	{
	public:
		MeshPluginStringSocket(const std::string& name = "MeshFileName");
		virtual void perform(std::string* signal) override;
		virtual void retrieve(std::string* signal) override{}
	};


	class MeshPluginMeshOutputSocket : public MeshOutputSocket
	{
	public:
		MeshPluginMeshOutputSocket(const std::string& name = "Mesh");
	};

	class MeshPluginSceneObjectOutputSocket : public SceneObjectOutputSocket
	{
	public:
		MeshPluginSceneObjectOutputSocket(const std::string& name = "SceneObject"):SceneObjectOutputSocket(name){}
	};

	class MeshPluginMeshRenderUnitOutputSocket : public TemplateOutputSocket<MeshRenderUnit>
	{
	public:
		MeshPluginMeshRenderUnitOutputSocket(const std::string& name = "MeshRenderUnit") : TemplateOutputSocket<MeshRenderUnit>()
		{
			this->initType("SOCK_MESH_RENDER_UNIT");
			this->setName(name);
		}
	};

	class EtoileMeshPlugin : public EPlugin, public SocketNode 
	{
		
	public:
		EtoileMeshPlugin(const std::string& name);
		~EtoileMeshPlugin();
		void openFile(const std::string& filename);
		virtual void init() override;
		virtual void apply() override;
		virtual void release() override;
	protected:
		MeshPluginStringSocket *_pInput;
		MeshPluginMeshOutputSocket *_pOutput;
		MeshPluginMeshRenderUnitOutputSocket * _pUnitOutput;
		MeshPluginSceneObjectOutputSocket * _pSceneObjOutput;
		MeshEditing *_editing;
	};
}

