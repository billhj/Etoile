/**
* Copyright(C) 2009-2012                
* @author Jing HUANG
* @file EtoileQtRendererPlugins.h
* @brief 
* @date 1/2/2011
*/

#pragma once
#include "module/EPlugin.h"
#include "EtoileMeshPlugin.h"
#include "EtoilePlanePlugin.h"
#include "EtoileImageToTexturePlugin.h"
#include "EtoileGLMeshRenderPassPlugin.h"
#include "EtoileGLQuadRenderPassPlugin.h"
#include "DerivedMeshRenderPassPlugin.h"
#include "DerivedQuadRenderPassPlugin.h"

PLUGIN_DLL void loadDescription(Etoile::EPluginsDescription** descrip);

namespace Etoile
{
	struct EtoileQtRendererPluginsDescription : public EPluginsDescription
	{
		EtoileQtRendererPluginsDescription()
		{
			this->menu = "Plugins";
			this->name = "EtoileQtRendererPlugins";
			this->plugins_load_functions.push_back(loadEtoileMeshPlugin);
			this->plugins_names.push_back("MeshLoader");
			this->plugins_load_functions.push_back(loadEtoilePlanePlugin);
			this->plugins_names.push_back("Plane");
			this->plugins_load_functions.push_back(loadEtoileImageToTexturePlugin);
			this->plugins_names.push_back("ImageToTexture");
			this->plugins_load_functions.push_back(loadEtoileGLMeshRenderPassPlugin);
			this->plugins_names.push_back("CommonMeshRenderPass");
			this->plugins_load_functions.push_back(loadEtoileGLQuadRenderPassPlugin);
			this->plugins_names.push_back("CommonQuadRenderPass");

			this->plugins_load_functions.push_back(loadDrawMeshRenderPassPlugin);
			this->plugins_names.push_back("DrawMeshPass");
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

