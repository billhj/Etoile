/**
* Copyright(C) 2009-2012                
* @author Jing HUANG
* @file EtoileSceneEditorPlugin.cpp
* @brief 
* @date 1/2/2011
*/

#include "EtoileSceneEditorPlugin.h"
#include "util/File.h"
#include "QtTextureLoader.h"
#include "tinyxml/tinyxml.h"
#include <QFileDialog>
#include <QMessageBox>

Etoile::EPlugin* loadEtoileSceneEditorPlugin()
{
	return new Etoile::EtoileSceneEditorPlugin("SceneEditor");
}

/**
* @brief For tracking memory leaks under windows using the crtdbg
*/
#if ( defined( _DEBUG ) || defined( DEBUG ) ) && defined( _MSC_VER )
#define _CRTDBG_MAP_ALLOC
#include <crtdbg.h>

#define DEBUG_NEW new( _NORMAL_BLOCK, __FILE__, __LINE__ )
#define new DEBUG_NEW
#endif

namespace Etoile
{

	

	EtoileSceneEditorPlugin::EtoileSceneEditorPlugin(const std::string& name): EPlugin(), SocketNode()
	{
		this->getType()._description = "SceneEditor";
		this->getType()._name = name;
		_editor = new SceneEditor();
		_editor->init(&_scene);
		_editor->setVisible(false);

		_scenesocket = new SceneOutputSocket("Scene");
		this->addOutputSocket(_scenesocket);
		_scenesocket->set(&_scene);
	}

	EtoileSceneEditorPlugin::~EtoileSceneEditorPlugin()
	{

	}


	void EtoileSceneEditorPlugin::init()
	{
		
	}

	void EtoileSceneEditorPlugin::apply()
	{
		_editor->updateTreeView();
		_editor->setVisible(true);
		_editor->raise();
		_scenesocket->set(&_scene);
		_scenesocket->signalEmit(&_scene);
	}

	void EtoileSceneEditorPlugin::release()
	{}
}