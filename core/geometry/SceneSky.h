/**
* Copyright(C) 2009-2012                
* @author Jing HUANG
* @file SceneSky.h
* @brief 
* @date 1/2/2011
*/

#pragma once
#include <sstream>
#include "Texture.h"

namespace Etoile
{
	class SceneSky
	{
	public:
		SceneSky() : _pSky(NULL)
		{
			generateID();
		}
		SceneSky(const SceneSky& obj)
		{
			(*this) = obj;
		}

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

		virtual void perform()  = 0;

		void setSkyTexture(const std::string& name, Texture* t)
		{
			idx[name] = idx.size();
			_skyList.push_back(t);
			_pSky = t;
		}

		void setSky(const std::string& name)
		{
			std::map<std::string, int>::iterator itor = idx.find(name);
			if(itor != idx.end())
			{
				_pSky = _skyList[itor->second];
			}	
		}
	protected:
		long _id;
		Texture* _pSky;
		std::vector<Texture*> _skyList;
		std::map<std::string, int> idx;
	};

}