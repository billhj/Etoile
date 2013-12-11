/**
* Copyright(C) 2009-2012                
* @author Jing HUANG
* @file Mass.h
* @brief 
* @date 1/2/2011
*/

#pragma once
#include "../../Math/Vectors.h"
#include <vector>

namespace Etoile
{

#define TIME_STEPSIZE2 0.5f * 0.5f
#define DAMPING 0.2

	class Mass
	{
	public:
		Mass(bool movable = true);
		Mass(const Vec3f& position, float mass = 1, bool movable = true);
		~Mass();
		void setMass(float mass){ _mass = mass;}
		float getMass(){return _mass;}

		void setPosition(const Vec3f& position);
		Vec3f& getPosition(){return _position;}

		void setMovable(bool movable){ _movable = movable;}
		bool isMovable(){return _movable;}

		void addForce(Vec3f force);

		Vec3f getAcceleration(){ return _acceleration;}
		void resetAcceleration(Vec3f acceleration = Vec3f(0,0,0)){ _acceleration = acceleration;}

		void move(float time = TIME_STEPSIZE2, float dampling = DAMPING);
	private:
		float _mass;
		Vec3f _position;
		Vec3f _oldPosition;
		Vec3f _acceleration;
		bool _movable;
	};

	typedef std::vector<Mass*> Masses;


}
