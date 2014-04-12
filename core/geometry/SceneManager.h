/**
* Copyright(C) 2009-2012                
* @author Jing HUANG
* @file SceneManager.h
* @brief 
* @date 1/2/2011
*/

#pragma once
#include "Scene.h"
#include <map>

namespace Etoile
{
	class SceneManager
	{
	private:
		SceneManager()
		{
			_pcurrent = NULL;
		}

	public:
		
		static SceneManager* getInstance()
		{
			if(scenemanager == NULL)
				scenemanager = new SceneManager();
			return scenemanager;
		}

		Scene* getScene(const std::string& name)
		{
			std::map<std::string, int>::iterator itor = _idx.find(name);
			if(itor != _idx.end())
			{
				return _scenes[itor->second];
			}
			Scene* scene = new Scene();
			_idx[name] = _scenes.size();
			_scenes.push_back(scene);
			return scene; 
		}

		Scene* getScene(long id)
		{
			for(unsigned int i = 0; i < _scenes.size(); ++i)
			{
				if(_scenes[i]->getID() == id)
					return _scenes[i];
			}
			return NULL;
		}
		Scene* getCurrent()
		{
			return _pcurrent;
		}
		void setCurrent(Scene* c)
		{
			_pcurrent = c;
		}

	protected:
		std::vector<Scene*> _scenes;
		std::map<std::string, int> _idx;
		Scene* _pcurrent;
		static SceneManager* scenemanager;
	};
	SceneManager* SceneManager::scenemanager = NULL;
}