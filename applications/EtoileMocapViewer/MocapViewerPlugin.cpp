/**
* Copyright(C) 2009-2012                
* @author Jing HUANG
* @file MocapViewerPlugin.cpp
* @brief 
* @date 1/2/2011
*/

#include "MocapViewerPlugin.h"

void loadDescription(Etoile::EPluginsDescription** descrip)
{
	descrip[0] = new Etoile::MocapViewerPluginDescription();
}

Etoile::EPlugin* loadMocapViewerPlugin()
{
	return new Etoile::MocapViewerPlugin("MocapViewer");
}

namespace Etoile
{
	MocapStringSocket::MocapStringSocket(const std::string& name): StringInputSocket(name)
	{
	}

	void MocapStringSocket::perform(std::string* signal) 
	{
		if(signal == NULL) return;
		MocapViewerPlugin* plugin = (MocapViewerPlugin*)(this->getNode());
		plugin->getViewer()->openFile(*signal);
	}

	MocapViewerPlugin::MocapViewerPlugin(const std::string& name): EPlugin(), SocketNode()
	{
		_pMocapviewer = NULL; 
		this->getType()._description = "MocapViewer";
		this->getType()._name = name;
		this->getType()._color._r = 100;
		this->getType()._color._g = 120;
		this->getType()._color._b = 180;
		this->getType()._color._a = 255;

		_pMocapviewer = new MocapViewer();
		_pMocapviewer->setVisible(false);
		MocapStringSocket* socket = new MocapStringSocket();
		this->addInputSocket(socket);
		_pMocapviewer->setVisible(true);
	}

	MocapViewerPlugin::~MocapViewerPlugin()
	{
		release();
	}

	void MocapViewerPlugin::init(){}

	void MocapViewerPlugin::apply()
	{
		_pMocapviewer->setVisible(true);
		_pMocapviewer->raise();
	}

	void MocapViewerPlugin::release()
	{
		_pMocapviewer->close();
	}

};


