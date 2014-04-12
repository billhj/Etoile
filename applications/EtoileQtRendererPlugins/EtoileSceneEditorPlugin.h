/**
* Copyright(C) 2009-2012                
* @author Jing HUANG
* @file EtoileSceneEditorPlugin.h
* @brief 
* @date 1/2/2011
*/

#pragma once
#include "module/EPlugin.h"
#include "module/flowgraph/SceneNodeSocket.h"
#include "SceneEditor.h"

Etoile::EPlugin* loadEtoileSceneEditorPlugin();

namespace Etoile
{
	

	class EtoileSceneEditorPlugin : public EPlugin, public SocketNode
	{
	public:
		EtoileSceneEditorPlugin(const std::string& name);
		~EtoileSceneEditorPlugin();
		virtual void init() override;
		virtual void apply() override;
		virtual void release() override;
	protected:
		Scene _scene;
		SceneEditor *_editor;
		SceneOutputSocket* _scenesocket;
	};
}

