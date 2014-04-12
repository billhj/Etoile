/**
* Copyright(C) 2009-2012                
* @author Jing HUANG
* @file Mass.cpp
* @brief 
* @date 1/2/2011
*/

#include "Mass.h"

/**
* @brief For tracking memory leaks under windows using the crtdbg
*/
#if ( defined( _DEBUG ) || defined( DEBUG ) ) && defined( _MSC_VER )
#define _CRTDBG_MAP_ALLOC
#include <crtdbg.h>

#define DEBUG_NEW new( _NORMAL_BLOCK, __FILE__, __LINE__ )
#define new DEBUG_NEW
#endif

namespace Etoile
{
	Mass::Mass(bool movable):_mass(1), _position(Vec3f(0,0,0)), _oldPosition(Vec3f(0,0,0)), _movable(movable)//,_velocity(0)
	{

	}

	Mass::Mass(const Vec3f& position, float mass, bool movable): _mass(mass),_position(position), _oldPosition(position), _movable(movable)//,_velocity(0)
	{


	}


	Mass::~Mass()
	{

	}


	void Mass::setPosition(const Vec3f& position)
	{
		_oldPosition = position;
		_position = position;
	}

	void Mass::addForce(Vec3f force)
	{
		assert(_mass && "mass = 0");
		if(_mass == 0) return;
		_acceleration += force/_mass;

	}


	//verlet intergration
	void Mass::move(float time, float dampling)
	{
		if(!_movable || _acceleration.length() < 0.0001) return;
		Vec3f temp = _position;
		_position  = _position + ((_position - _oldPosition)  + _acceleration * time) * (1.0f - dampling);
		_oldPosition = temp;
		_acceleration = Vec3f(0,0,0);
	}
}
