/**
* Copyright(C) 2009-2012                
* @author Jing HUANG
* @file DerivedMeshRenderPassPlugin.cpp
* @brief 
* @date 1/2/2011
*/

#include "DerivedMeshRenderPassPlugin.h"

/**
* @brief For tracking memory leaks under windows using the crtdbg
*/
#if ( defined( _DEBUG ) || defined( DEBUG ) ) && defined( _MSC_VER )
#define _CRTDBG_MAP_ALLOC
#include <crtdbg.h>

#define DEBUG_NEW new( _NORMAL_BLOCK, __FILE__, __LINE__ )
#define new DEBUG_NEW
#endif

Etoile::EPlugin* loadDrawMeshRenderPassPlugin()
{
	return new Etoile::DrawMeshRenderPassPlugin("DrawMeshPass");
}

namespace Etoile
{

	

	DrawMeshRenderPassPlugin::DrawMeshRenderPassPlugin(const std::string& name): EtoileGLMeshRenderPassPlugin(name)
	{
		this->getType()._description = "DrawMeshPass";
	}

	void DrawMeshRenderPassPlugin::init()
	{
		loadFile("shader/deferred/drawmesh.xml");
	}


}