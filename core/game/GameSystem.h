/**
* Copyright(C) 2009-2012                
* @author Jing HUANG
* @file GameSystem.h
* @brief 
* @date 1/2/2011
*/

#pragma once
#ifndef GAME_SYETEM_H
#define GAME_SYETEM_H

#include <string>


namespace Etoile
{
	const int SYSTEMNUMBER = 6;
	enum GameSystemType
	{
		ANIMATION_SYSTEM = 0,
		PHYSICS_SYSTEM,
		RENDERRING_SYSTEM,
	};

	/**
	 * class GameSystem: implement basic system
	 */
	class GameSystem
	{
	public:
		GameSystem();
		virtual~GameSystem(){}
		virtual void init() = 0;
		virtual void update() = 0;
		virtual void release() = 0;
		
	protected:
		
	};

}

#endif GAME_SYETEM_H