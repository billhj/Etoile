/**
* Copyright(C) 2009-2012                
* @author Jing HUANG
* @file DerivedQuadRenderPassPlugin.h
* @brief 
* @date 1/2/2011
*/

#pragma once
#include "EtoileGLQuadRenderPassPlugin.h"

Etoile::EPlugin* loadGammaQuadRenderPassPlugin();
Etoile::EPlugin* loadDeferredQuadRenderPassPlugin();
Etoile::EPlugin* loadRainbowErrorQuadRenderPassPlugin();

namespace Etoile
{

	class GammaQuadRenderPassPlugin : public EtoileGLQuadRenderPassPlugin 
	{
	public:
		GammaQuadRenderPassPlugin(const std::string& name);
		virtual void init() override;
	};

	class DeferredQuadRenderPassPlugin : public EtoileGLQuadRenderPassPlugin 
	{
	public:
		DeferredQuadRenderPassPlugin(const std::string& name);
		virtual void init() override;
	};


	class RainbowErrorQuadRenderPassPlugin : public EtoileGLQuadRenderPassPlugin 
	{
	public:
		RainbowErrorQuadRenderPassPlugin(const std::string& name);
		virtual void init() override;
	};
}

