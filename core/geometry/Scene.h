/**
* Copyright(C) 2009-2012                
* @author Jing HUANG
* @file Scene.h
* @brief 
* @date 1/2/2011
*/

#pragma once
#include <sstream>
#include "SceneNode.h"
#include "SceneSky.h"
namespace Etoile
{
	class Scene
	{
	public:
		Scene() : _pSky(NULL)
		{
			generateID();
		}
		Scene(const Scene& obj)
		{
			(*this) = obj;
		}

		void addSceneNode(SceneNode* node){_children.push_back(node);}
		std::vector<SceneNode*>& getSceneNodes(){return _children;}
		void setID(long id)
		{
			_id = id;
		}

		long getID(){return _id;}
		std::string getStringID()
		{
			std::string number;
			std::stringstream strstream;
			strstream << _id;
			strstream >> number;
			return number;
		}
		void generateID()
		{
			_id = rand();
		}
		void setSky(SceneSky* sky)
		{
			_pSky = sky;
		}

		SceneSky* getSkye()
		{
			return _pSky;
		}
	protected:
		std::vector<SceneNode*> _children;
		SceneSky* _pSky; 
		long _id;
	};

}