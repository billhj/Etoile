/**
* Copyright(C) 2009-2012                
* @author Jing HUANG
* @file GameComponent.h
* @brief 
* @date 1/2/2011
*/

#pragma once
#ifndef GAME_COMPONENT_H
#define GAME_COMPONENT_H

#include <string>


namespace Etoile
{
	/**
	 * class GameComponent: implement basic game
	 */
	class GameComponent
	{
	public:
		GameComponent();
		virtual~GameComponent(){}
		virtual void init() = 0;
		virtual void update() = 0;
		virtual void release() = 0;
		
	protected:
		
	};

}

#endif //GAME_COMPONENT_H