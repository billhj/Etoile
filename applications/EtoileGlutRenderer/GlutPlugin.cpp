#include "GlutPlugin.h"
/**
* Copyright(C) 2009-2012                
* @author Jing HUANG
* @file GlutPlugin.cpp
* @brief 
* @date 1/2/2011
*/

void loadPlugin(Etoile::EPlugin** plug)
{
	plug[0] = new Etoile::GlutPlugin("GlutRenderer");
}

void loadDescription(Etoile::EPluginsDescription** descrip)
{
	descrip[0] = new Etoile::EPluginsDescription();
}

namespace Etoile
{
	GlutTextureInputSocket::GlutTextureInputSocket()
	{
		this->initType(Etoile::SocketTYPE::SOCK_TEXTURE);
		this->_type._name = "Output_Texture";
		_pRenderer = new GlutRenderer();
		//_pRenderer->setVisible(false);
	}

	void GlutTextureInputSocket::perform(Texture* signal)
	{
		_pRenderer->setOutputTexture(signal);
		//_pRenderer->setVisible(true);
	}

	GlutPlugin::GlutPlugin()
	{
		this->getType()._description = "GlutRenderer";
		this->getType()._name = "GlutRenderer";

		_socket = new GlutTextureInputSocket();
		this->addInputSocket(_socket);
	}

	GlutPlugin::GlutPlugin(const std::string& name):EPlugin(name)
	{
		this->getType()._description = "GlutRenderer";
		this->getType()._name = "GlutRenderer";

		_socket = new GlutTextureInputSocket();
		this->addInputSocket(_socket);
	}

	GlutPlugin::~GlutPlugin()
	{
		release();
	}
	void GlutPlugin::init()
	{
		
	}

	void GlutPlugin::apply()
	{
		//_socket->getRenderer()->setVisible(true);
		//_socket->getRenderer()->raise();
	}

	void GlutPlugin::release()
	{
		//_socket->getRenderer()->close();
	}

}
