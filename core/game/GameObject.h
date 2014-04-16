/**
* Copyright(C) 2009-2012                
* @author Jing HUANG
* @file GameObject.h
* @brief 
* @date 1/2/2011
*/

#pragma once
#ifndef GAME_OBJECT_H
#define GAME_OBJECT_H

#include <string>

namespace Etoile
{
	class GameObject
	{
	public:
		GameObject();
		GameObject(const std::string&);
		virtual~GameObject(){}
		const std::string name(){return _name;}
	protected:
		std::string _name;
	};

}

#endif GAME_OBJECT_H