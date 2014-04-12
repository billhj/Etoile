/**
* Copyright(C) 2009-2012                
* @author Jing HUANG
* @file EtoileQtRendererPlugins.h
* @brief 
* @date 1/2/2011
*/

#pragma once
#include "module/EPlugin.h"
#include "EtoileImageToTexturePlugin.h"
#include "EtoileGLSceneRenderPassPlugin.h"
#include "EtoileGLQuadRenderPassPlugin.h"
#include "DerivedSceneRenderPassPlugin.h"
#include "DerivedQuadRenderPassPlugin.h"
#include "EtoileSceneEditorPlugin.h"

PLUGIN_DLL void loadDescription(Etoile::EPluginsDescription** descrip);

namespace Etoile
{
	struct EtoileQtRendererPluginsDescription : public EPluginsDescription
	{
		EtoileQtRendererPluginsDescription()
		{
			this->menu = "Plugins";
			this->name = "EtoileQtRendererPlugins";
		
			//this->plugins_load_functions.push_back(loadEtoilePlanePlugin);
			//this->plugins_names.push_back("Plane");
			this->plugins_load_functions.push_back(loadEtoileImageToTexturePlugin);
			this->plugins_names.push_back("ImageToTexture");
			this->plugins_load_functions.push_back(loadEtoileGLSceneRenderPassPlugin);
			this->plugins_names.push_back("CommonSceneRenderPass");
			this->plugins_load_functions.push_back(loadEtoileGLQuadRenderPassPlugin);
			this->plugins_names.push_back("CommonQuadRenderPass");
			this->plugins_load_functions.push_back(loadEtoileSceneEditorPlugin);
			this->plugins_names.push_back("SceneEditor");
			this->plugins_load_functions.push_back(loadDrawSceneRenderPassPlugin);
			this->plugins_names.push_back("DrawScenePass");
			this->plugins_load_functions.push_back(loadGammaQuadRenderPassPlugin);
			this->plugins_names.push_back("GammaQuadPass");
			this->plugins_load_functions.push_back(loadDeferredQuadRenderPassPlugin);
			this->plugins_names.push_back("DeferredPass");
			this->plugins_load_functions.push_back(loadRainbowErrorQuadRenderPassPlugin);
			this->plugins_names.push_back("RainbowError");
		}
	};

	class EtoileQtRendererPlugins
	{
	public:
		EtoileQtRendererPlugins();
		~EtoileQtRendererPlugins();

	private:

	};
}

