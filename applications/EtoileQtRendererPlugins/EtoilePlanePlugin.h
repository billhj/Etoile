/**
* Copyright(C) 2009-2012                
* @author Jing HUANG
* @file EtoilePlanePlugin.h
* @brief 
* @date 1/2/2011
*/

#pragma once
#include "module/EPlugin.h"
#include "module/flowgraph/GeneralSocket.h"
#include "module/flowgraph/MeshSocket.h"
#include "geometry/primitive.h"
#include "MeshEditing.h"
#include "renderer/MeshRenderUnit.h"
#include "module/flowgraph/SceneObjectSocket.h"

Etoile::EPlugin* loadEtoilePlanePlugin();

namespace Etoile
{
	class PlanePluginMeshOutputSocket : public MeshOutputSocket
	{
	public:
		PlanePluginMeshOutputSocket(const std::string& name = "Mesh") : MeshOutputSocket(name)
		{
		}
	};

	class PlanePluginMeshRenderUnitOutputSocket : public TemplateOutputSocket<MeshRenderUnit>
	{
	public:
		PlanePluginMeshRenderUnitOutputSocket(const std::string& name = "MeshRenderUnit") : TemplateOutputSocket<MeshRenderUnit>()
		{
			this->initType("SOCK_MESH_RENDER_UNIT");
			this->setName(name);
		}
	};

	class PlanePluginPlaneOutputSocket : public PlaneOutputSocket
	{
	public:
		PlanePluginPlaneOutputSocket(const std::string& name = "Plane") : PlaneOutputSocket(name)
		{
		}
	};

	class EtoilePlanePlugin : public EPlugin, public SocketNode 
	{
		
	public:
		EtoilePlanePlugin(const std::string& name);
		~EtoilePlanePlugin();
		virtual void init() override;
		virtual void apply() override;
		virtual void release() override;
	protected:
		PlanePluginMeshOutputSocket *_pOutput;
		PlanePluginMeshRenderUnitOutputSocket * _pUnitOutput;
		PlanePluginPlaneOutputSocket *_pPlaneSocket;
		MeshEditing *_editing;
	};
}

