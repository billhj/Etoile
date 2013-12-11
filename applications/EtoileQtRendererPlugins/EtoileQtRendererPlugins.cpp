/**
* Copyright(C) 2009-2012                
* @author Jing HUANG
* @file EtoileQtRendererPlugins.cpp
* @brief 
* @date 1/2/2011
*/

#include "EtoileQtRendererPlugins.h"
#include "EtoileMeshPlugin.h"

/**
* @brief For tracking memory leaks under windows using the crtdbg
*/
#if ( defined( _DEBUG ) || defined( DEBUG ) ) && defined( _MSC_VER )
#define _CRTDBG_MAP_ALLOC
#include <crtdbg.h>

#define DEBUG_NEW new( _NORMAL_BLOCK, __FILE__, __LINE__ )
#define new DEBUG_NEW
#endif

void loadDescription(Etoile::EPluginsDescription** descrip)
{
	descrip[0] = new Etoile::EtoileQtRendererPluginsDescription();
}


namespace Etoile
{
	EtoileQtRendererPlugins::EtoileQtRendererPlugins()
	{

	}

	EtoileQtRendererPlugins::~EtoileQtRendererPlugins()
	{

	}
}