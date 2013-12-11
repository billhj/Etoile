/**
* Copyright(C) 2009-2012                
* @author Jing HUANG
* @file GlutPlugin.h
* @brief 
* @date 1/2/2011
*/

#pragma once
#include "module/EPlugin.h"
#include "module/flowgraph/TextureSocket.h"
#include "GlutRenderer.h"

namespace Etoile
{
	class GlutTextureInputSocket : public TextureInputSocket
	{
	protected:
		GlutRenderer* _pRenderer;
	public:
		GlutTextureInputSocket();
		virtual void perform(Texture* signal) override;
		GlutRenderer* getRenderer(){return _pRenderer;}
	};

	class GlutPlugin : public EPlugin, public SocketNode 
	{
	protected:
		GlutTextureInputSocket* _socket;
	public:
		GlutPlugin();
		GlutPlugin(const std::string& name);
		virtual ~GlutPlugin();
		virtual void init() override;
		virtual void apply() override;
		virtual void release() override;
	};

}

PLUGIN_DLL void loadPlugin(Etoile::EPlugin** plug);
PLUGIN_DLL void loadDescription(Etoile::EPluginsDescription** descrip);