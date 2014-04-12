/**
* Copyright(C) 2009-2012                
* @author Jing HUANG
* @file MeshViewerPlugin.cpp
* @brief 
* @date 1/2/2011
*/

#include "MeshViewerPlugin.h"

void loadDescription(Etoile::EPluginsDescription** descrip)
{
	descrip[0] = new Etoile::MeshViewerPluginDescription();
}

Etoile::EPlugin* loadMeshViewerPlugin()
{
	return new Etoile::MeshViewerPlugin("MeshViewer");
}

namespace Etoile
{
	MeshStringSocket::MeshStringSocket(const std::string& name) : StringInputSocket(name)
	{
	}

	void MeshStringSocket::perform(std::string* signal)
	{
		if(signal == NULL) return;
		MeshViewerPlugin* plugin = (MeshViewerPlugin*)(this->getNode());
		plugin->getViewer()->openFile(*signal);
	}

	MeshViewerMeshInputSocket::MeshViewerMeshInputSocket(const std::string& name) : MeshInputSocket(name)
	{

	}

	void MeshViewerMeshInputSocket::perform(Etoile::Mesh* signal)
	{
		if(signal == NULL) return;
		MeshViewerPlugin* plugin = (MeshViewerPlugin*)(this->getNode());
		plugin->getViewer()->setMesh(signal);
	}

	MeshViewerPlugin::MeshViewerPlugin(const std::string& name): EPlugin(),SocketNode()
	{
		this->getType()._description = "MeshViewer";
		this->getType()._name = name;
		this->getType()._color._r = 240;
		this->getType()._color._g = 130;
		this->getType()._color._b = 240;
		this->getType()._color._a = 255;

		_pmeshviewer = new EtoileMeshViewer();

		_socket = new MeshViewerMeshInputSocket();
		this->addInputSocket(_socket);

		MeshStringSocket* socket = new MeshStringSocket();
		this->addInputSocket(socket);
		_pmeshviewer->setVisible(true);
	}

	MeshViewerPlugin::~MeshViewerPlugin()
	{
		release();
	}

	void MeshViewerPlugin::init()
	{
		_pmeshviewer->setVisible(true);
	}

	void MeshViewerPlugin::apply()
	{
		_pmeshviewer->setVisible(true);
		_pmeshviewer->raise();
	}

	void MeshViewerPlugin::release()
	{
	}

}

