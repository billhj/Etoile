/**
* Copyright(C) 2009-2012                
* @author Jing HUANG
* @file DerivedQuadRenderPassPlugin.cpp
* @brief 
* @date 1/2/2011
*/

#include "DerivedQuadRenderPassPlugin.h"

/**
* @brief For tracking memory leaks under windows using the crtdbg
*/
#if ( defined( _DEBUG ) || defined( DEBUG ) ) && defined( _MSC_VER )
#define _CRTDBG_MAP_ALLOC
#include <crtdbg.h>

#define DEBUG_NEW new( _NORMAL_BLOCK, __FILE__, __LINE__ )
#define new DEBUG_NEW
#endif


Etoile::EPlugin* loadGammaQuadRenderPassPlugin()
{
	return new Etoile::GammaQuadRenderPassPlugin("GammaQuadPass");
}

Etoile::EPlugin* loadDeferredQuadRenderPassPlugin()
{
	return new Etoile::DeferredQuadRenderPassPlugin("DeferredPass");
}

Etoile::EPlugin* loadRainbowErrorQuadRenderPassPlugin()
{
	return new Etoile::RainbowErrorQuadRenderPassPlugin("RainbowError");
}

namespace Etoile
{

	GammaQuadRenderPassPlugin::GammaQuadRenderPassPlugin(const std::string& name): EtoileGLQuadRenderPassPlugin(name)
	{
		this->getType()._description = "GammaQuadPass";loadFile("shader/gammaoutput/gammaoutput.xml");
	}

	void GammaQuadRenderPassPlugin::init()
	{
		
	}

	DeferredQuadRenderPassPlugin::DeferredQuadRenderPassPlugin(const std::string& name): EtoileGLQuadRenderPassPlugin(name)
	{
		this->getType()._description = "DeferredPass";
		initMatrixSocket();
		loadFile("shader/deferred/deferred.xml");
	}

	void DeferredQuadRenderPassPlugin::init()
	{
		
	}

	RainbowErrorQuadRenderPassPlugin::RainbowErrorQuadRenderPassPlugin(const std::string& name): EtoileGLQuadRenderPassPlugin(name)
	{
		this->getType()._description = "RainbowError";
		loadFile("shader/rainbowerror/rainbowError.xml");
	}

	void RainbowErrorQuadRenderPassPlugin::init()
	{
		
	}
}