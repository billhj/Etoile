/**
* Copyright(C) 2009-2012                
* @author Jing HUANG
* @file EtoilePlanePlugin.cpp
* @brief 
* @date 1/2/2011
*/

#include "EtoilePlanePlugin.h"
#include "util/File.h"

#include "QtTextureLoader.h"
#include "geometry/meshloader/OBJMeshLoader.h"
#include "geometry/meshloader/OpenMeshLoader.h"
#include "renderer/OpenGL/VBOMeshRenderUnit.h"
#include "renderer/OpenGL/ImmediateMeshRenderUnit.h"


/**
* @brief For tracking memory leaks under windows using the crtdbg
*/
#if ( defined( _DEBUG ) || defined( DEBUG ) ) && defined( _MSC_VER )
#define _CRTDBG_MAP_ALLOC
#include <crtdbg.h>

#define DEBUG_NEW new( _NORMAL_BLOCK, __FILE__, __LINE__ )
#define new DEBUG_NEW
#endif


Etoile::EPlugin* loadEtoilePlanePlugin()
{
	return new Etoile::EtoilePlanePlugin("Plane");
}

namespace Etoile
{

	EtoilePlanePlugin::EtoilePlanePlugin(const std::string& name): EPlugin(), SocketNode()
	{
		this->getType()._description = "Plane";
		this->getType()._name = name;
		this->getType()._w = 80;
		this->getType()._h = 80;
		this->getType()._color._r = 100;
		this->getType()._color._g = 220;
		this->getType()._color._b = 220;
		this->getType()._color._a = 240;

		_pOutput = new PlanePluginMeshOutputSocket();
		this->addOutputSocket(_pOutput);
		_pUnitOutput = new PlanePluginMeshRenderUnitOutputSocket;
		this->addOutputSocket(_pUnitOutput);
		_pPlaneSocket = new PlanePluginPlaneOutputSocket();
		this->addOutputSocket(_pPlaneSocket);

		_editing = new MeshEditing();
		_editing->setVisible(false);
		init();
	}

	EtoilePlanePlugin::~EtoilePlanePlugin()
	{

	}


	void EtoilePlanePlugin::init()
	{
		Plane* plane = new Plane("Plane");
		float emptyMap[16] = {1,1,1,1, 1,1,1,1, 1,1,1,1, 1,1,1,1};
		GLTexture2D* t = new GLTexture2D("emptyMap");
		t->create(2, 2, 1 , GL_RGBA32F_ARB, GL_RGBA, GL_FLOAT , &emptyMap[0], false);

		float checkboard[64] = {0,0,0,0, 1,1,1,1, 0,0,0,0, 1,1,1,1, 
			1,1,1,1,0,0,0,0, 1,1,1,1,0,0,0,0,  
			0,0,0,0, 1,1,1,1, 0,0,0,0, 1,1,1,1, 
			1,1,1,1,0,0,0,0, 1,1,1,1,0,0,0,0};
		GLTexture2D* t2 = new GLTexture2D("checkBoardMap");
		t2->create(4, 4, 1 , GL_RGBA32F_ARB, GL_RGBA, GL_FLOAT , &checkboard[0], false);

		plane->getMaterial()->setDiffuseTexture(t2);
		_pOutput->set(plane);
		_pOutput->signalEmit(plane);
		_pPlaneSocket->set(plane);
		_pPlaneSocket->signalEmit(plane);
		/*if(_pUnitOutput->getPerformers().size() > 0)
		{*/
			std::string mode = _editing->getMeshRenderUnitType();
			MeshRenderUnit* renderunit;
			if(mode.compare("RenderMode_Immediate")==0)
			{
				renderunit = new ImmediateMeshRenderUnit(plane->getName());
				renderunit->setMesh(plane);
			}
			else if(mode.compare("RenderMode_GPUVBO")==0)
			{
				renderunit = new GPUBasedVBOMeshRenderUnit(plane->getName());
				renderunit->setMesh(plane);
			}
			else if(mode.compare("RenderMode_AvancedGPUVBO")==0)
			{
				renderunit = new AvancedGPUBasedVBOMeshRenderUnit(plane->getName());
				renderunit->setMesh(plane);
			}
			_pUnitOutput->set(renderunit);
			_pUnitOutput->signalEmit(renderunit);
	}

	void EtoilePlanePlugin::apply()
	{
		_editing->init(_pOutput->get());
		_editing->setVisible(true);
		_editing->raise();
	}
	void EtoilePlanePlugin::release()
	{}
}