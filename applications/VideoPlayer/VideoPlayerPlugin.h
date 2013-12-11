/**
* Copyright(C) 2009-2012                
* @author Jing HUANG
* @file VideoPlayerPlugin.h
* @brief 
* @date 1/2/2011
*/

#pragma once
#include "module/EPlugin.h"
#include "module/flowgraph/SocketNode.h"
#include "EtoileVideoPlayer.h"


PLUGIN_DLL void loadDescription(Etoile::EPluginsDescription** descrip);
Etoile::EPlugin* loadVideoPlayerPlugin();

namespace Etoile
{
	struct VideoPlayerPluginDescription : public EPluginsDescription
	{
		VideoPlayerPluginDescription()
		{
			this->menu = "Plugins";
			this->name = "VideoPlayer";
			this->plugins_load_functions.push_back(loadVideoPlayerPlugin);
			this->plugins_names.push_back("VideoPlayer");
		}
	};

	class VideoPlayerPlugin : public EPlugin, public SocketNode 
	{
	protected:
		EtoileVideoPlayer* player;
	public:
		VideoPlayerPlugin(const std::string& name);
		~VideoPlayerPlugin(void);
		virtual void init() override;
		virtual void apply() override;
		virtual void release() override;
	};
}
