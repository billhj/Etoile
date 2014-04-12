/**
* Copyright(C) 2009-2012                
* @author Jing HUANG
* @file QglviewerPlugin.h
* @brief 
* @date 1/2/2011
*/

#pragma once
#include "module/EPlugin.h"
#include "module/flowgraph/TextureSocket.h"
#include "QglviewerRenderer.h"
#include "module/flowgraph/GeneralSocket.h"
#include "../EtoileMeshViewer/CameraWidget.h"

PLUGIN_DLL void loadDescription(Etoile::EPluginsDescription** descrip);
Etoile::EPlugin* loadQglviewerRendererPlugin();

namespace Etoile
{
	struct QglviewerPluginDescription : public EPluginsDescription
	{
		QglviewerPluginDescription()
		{
			this->menu = "Plugins";
			this->name = "EtoileQtRendererPlugins";
			this->plugins_load_functions.push_back(loadQglviewerRendererPlugin);
			this->plugins_names.push_back("QglviewerRenderer");
		}
	};

	class QglTextureInputSocket : public TextureInputSocket
	{
	public:
		QglTextureInputSocket(const std::string& name);
		virtual void perform(Texture* signal) override;
		virtual void retrieve(Texture* signal) override;
	};

	class QglViewMatrix4fOutputSocket : public Matrix4fOutputSocket
	{
	public:
		QglViewMatrix4fOutputSocket(const std::string& name) : Matrix4fOutputSocket(name){}
	
	};

	class QglProjectionMatrix4fOutputSocket : public Matrix4fOutputSocket
	{
	public:
		QglProjectionMatrix4fOutputSocket(const std::string& name) : Matrix4fOutputSocket(name){}
	
	};

	class QglViewProjectionMatrix4fOutputSocket : public Matrix4fOutputSocket
	{
	public:
		QglViewProjectionMatrix4fOutputSocket(const std::string& name) : Matrix4fOutputSocket(name){}
	
	};

	class QglviewerPlugin : public EPlugin, public SocketNode, public ESignalPerformer<GraphScene>
	{
	protected:
		QglTextureInputSocket* _socket;
		QglviewerRenderer* _pRenderer;
		QglViewMatrix4fOutputSocket* _pviewsocket;
		QglProjectionMatrix4fOutputSocket* _pprojsocket;
		QglViewProjectionMatrix4fOutputSocket* _pviewprojsocket;
		CameraWidget* _pCamera;
	public:
		QglviewerPlugin(const std::string& name);
		virtual ~QglviewerPlugin();
		virtual void init() override;
		virtual void apply() override;
		virtual void release() override;
		void updateModelViewMatrix(Matrix4f* view, Matrix4f* project);
		void setupRenderOrder();
		void sort(BasicNode* current, std::vector<BasicNode*>& nodes);
		QglviewerRenderer* getRenderer(){return _pRenderer;}
		virtual void perform(GraphScene* signal) override;
	};

}

