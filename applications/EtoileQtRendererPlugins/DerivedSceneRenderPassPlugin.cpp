/**
* Copyright(C) 2009-2012                
* @author Jing HUANG
* @file DerivedSceneRenderPassPlugin.cpp
* @brief 
* @date 1/2/2011
*/

#include "DerivedSceneRenderPassPlugin.h"

/**
* @brief For tracking memory leaks under windows using the crtdbg
*/
#if ( defined( _DEBUG ) || defined( DEBUG ) ) && defined( _MSC_VER )
#define _CRTDBG_MAP_ALLOC
#include <crtdbg.h>

#define DEBUG_NEW new( _NORMAL_BLOCK, __FILE__, __LINE__ )
#define new DEBUG_NEW
#endif

Etoile::EPlugin* loadDrawSceneRenderPassPlugin()
{
	return new Etoile::DrawSceneRenderPassPlugin("DrawScenePass");
}

namespace Etoile
{

	

	DrawSceneRenderPassPlugin::DrawSceneRenderPassPlugin(const std::string& name): EtoileGLSceneRenderPassPlugin(name)
	{
		this->getType()._description = "DrawScenePass";
		loadFile("shader/deferred/drawmesh.xml");
	}

	void DrawSceneRenderPassPlugin::init()
	{
		
	}


}