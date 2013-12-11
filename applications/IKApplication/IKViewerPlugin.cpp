/**
* Copyright(C) 2009-2012                
* @author Jing HUANG
* @file IKViewerPlugin.cpp
* @brief 
* @date 1/2/2011
*/

#include "IKViewerPlugin.h"

	
Etoile::EPlugin* loadIKViewerPlugin()
{
	return new Etoile::IKViewerPlugin("IKViewer");
}

void loadDescription(Etoile::EPluginsDescription** descrip)
{
	descrip[0] = new Etoile::IKViewerPluginDescription();
}