/**
* Copyright(C) 2009-2012                
* @author Jing HUANG
* @file VideoPlayerPlugin.cpp
* @brief 
* @date 1/2/2011
*/

#include "VideoPlayerPlugin.h"

void loadDescription(Etoile::EPluginsDescription** descrip)
{
	descrip[0] = new Etoile::VideoPlayerPluginDescription();
}

Etoile::EPlugin* loadVideoPlayerPlugin()
{
	return new Etoile::VideoPlayerPlugin("VideoPlayer");
}

namespace Etoile
{
	VideoPlayerPlugin::VideoPlayerPlugin(const std::string& name):EPlugin(),SocketNode()
	{
		player = new EtoileVideoPlayer();
		this->getType()._description = "VideoPlayer";
		this->getType()._name = name;
		this->getType()._w = 80;
		this->getType()._h = 50;
		this->getType()._color._r = 200;
		this->getType()._color._g = 240;
		this->getType()._color._b = 240;
		this->getType()._color._a = 255;
		apply();
	}


	VideoPlayerPlugin::~VideoPlayerPlugin(void)
	{
	}

	void VideoPlayerPlugin::init()
	{
		
	}

	void VideoPlayerPlugin::apply()
	{
		player->show();
		player->raise();
	}

	void VideoPlayerPlugin::release()
	{
		
	}
}