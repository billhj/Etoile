/**
* Copyright(C) 2009-2012                
* @author Jing HUANG
* @file EtoileMeshPlugin.cpp
* @brief 
* @date 1/2/2011
*/

#include "EtoileMeshPlugin.h"
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


Etoile::EPlugin* loadEtoileMeshPlugin()
{
	return new Etoile::EtoileMeshPlugin("MeshLoader");
}

namespace Etoile
{

	MeshPluginStringSocket::MeshPluginStringSocket(const std::string& name) : StringInputSocket(name)
	{
	}

	MeshPluginMeshOutputSocket::MeshPluginMeshOutputSocket(const std::string& name) : MeshOutputSocket(name)
	{
	}

	void MeshPluginStringSocket::perform(std::string* signal)
	{
		if(signal == NULL) return;
		EtoileMeshPlugin* plugin = (EtoileMeshPlugin*)(this->getNode());
		plugin->openFile(*signal);
	}

	EtoileMeshPlugin::EtoileMeshPlugin(const std::string& name): EPlugin(), SocketNode()
	{
		this->getType()._description = "MeshLoader";
		this->getType()._name = name;
		this->getType()._w = 80;
		this->getType()._h = 80;
		this->getType()._color._r = 100;
		this->getType()._color._g = 220;
		this->getType()._color._b = 220;
		this->getType()._color._a = 240;

		_pInput = new MeshPluginStringSocket();
		this->addInputSocket(_pInput);
		_pOutput = new MeshPluginMeshOutputSocket();
		this->addOutputSocket(_pOutput);
		_pUnitOutput = new MeshPluginMeshRenderUnitOutputSocket;
		this->addOutputSocket(_pUnitOutput);
		_pSceneObjOutput = new MeshPluginSceneObjectOutputSocket();
		this->addOutputSocket(_pSceneObjOutput);

		_editing = new MeshEditing();
		_editing->setVisible(false);
	}

	EtoileMeshPlugin::~EtoileMeshPlugin()
	{

	}

	void EtoileMeshPlugin::openFile(const std::string& filename)
	{
		if(filename.empty()) return;
		std::string ext = File::getFileExtension(filename);
		Mesh *mesh = new Mesh(filename);
		mesh->setDeviceID(this->getDeviceID());
		if(ext.compare("obj") == 0)
		{
			OBJMeshLoader loader;
			QtTextureLoader* textureloader = new QtTextureLoader();
			loader.setTextureLoader(textureloader);
			loader.loadFromFile(filename, mesh);
		}
		else
		{
			OpenMeshLoader loader;
			QtTextureLoader* textureloader = new QtTextureLoader();
			loader.setTextureLoader(textureloader);
			loader.loadFromFile(filename, mesh);
		}
		_pSceneObjOutput->set(mesh);
		_pSceneObjOutput->signalEmit(mesh);
		_pOutput->set(mesh);
		_pOutput->signalEmit(mesh);
		/*if(_pUnitOutput->getPerformers().size() > 0)
		{*/
			std::string mode = _editing->getMeshRenderUnitType();
			MeshRenderUnit* renderunit;
			if(mode.compare("RenderMode_Immediate")==0)
			{
				renderunit = new ImmediateMeshRenderUnit(mesh->getName());
				renderunit->setMesh(mesh);
			}
			else if(mode.compare("RenderMode_GPUVBO")==0)
			{
				renderunit = new GPUBasedVBOMeshRenderUnit(mesh->getName());
				renderunit->setMesh(mesh);
			}
			else if(mode.compare("RenderMode_AvancedGPUVBO")==0)
			{
				renderunit = new AvancedGPUBasedVBOMeshRenderUnit(mesh->getName());
				renderunit->setMesh(mesh);
			}
			_pUnitOutput->set(renderunit);
			_pUnitOutput->signalEmit(renderunit);
		//}
			_editing->init(_pOutput->get());
	}

	void EtoileMeshPlugin::init()
	{

	}

	void EtoileMeshPlugin::apply()
	{
		_editing->setVisible(true);
		_editing->raise();
	}
	void EtoileMeshPlugin::release()
	{}
}