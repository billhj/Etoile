/**
* Copyright(C) 2009-2012                
* @author Jing HUANG
* @file GameEngine.h
* @brief 
* @date 1/2/2011
*/

#pragma once
#ifndef GAME_ENGINE_H
#define GAME_ENGINE_H

#include <string>
#include <vector>
#include "GameSystem.h"

namespace Etoile
{
	

	/**
	 * class GameEngine: implement basic system
	 */
	class GameEngine
	{
	public:
		GameEngine(){_systems.resize(SYSTEMNUMBER);}
		virtual~GameEngine(){}
		virtual void init() = 0;
		virtual void update() = 0;
		virtual void release() = 0;
		GameSystem* getSystem(GameSystemType type)
		{
			return _systems[type];
		}
	protected:
		std::vector<GameSystem*> _systems;
	};

}

#endif //GAME_ENGINE_H