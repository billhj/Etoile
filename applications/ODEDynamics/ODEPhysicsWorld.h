/**
* Copyright(C) 2009-2012                
* @author Jing HUANG
* @file ODEPhysicsWorld.h
* @brief 
* @date 1/2/2011
*/

#pragma once
#include "ODEPhysicsObject.h"
#include <map>

namespace Etoile
{
	/* 
	* callback for ODE. friend of PhysicsEngine. _this is meant to be 
	* a this pointer at the PhysicsEngine object currently using the callback.
	*/
	void nearCollisionCallback(void* _this, dGeomID id1, dGeomID id2);

	class ODEPhysicsWorld
	{

	public:
		ODEPhysicsWorld(void);
		~ODEPhysicsWorld(void);
		void init();
		void createPlane(double a = 0, double b = 1, double c = 0, double d = 0);
		void createGravity(double x = 0, double y = -9.8, double z = 0);
		void doPhysics();
		void release();
		void addPhysicsObject(ODEPhysicsObject*);
		dWorldID getWorldID(){return _worldID;}
		dSpaceID getSpaceID(){return _spaceID;}
		std::map<std::string, ODEPhysicsObject*>& getPhysicsObjects(){return _objects;}
		void updatePhysicsObjects();
	protected:
		dWorldID _worldID;
		dSpaceID _spaceID;
		dJointGroupID _contactgroupID;
		dGeomID _groundID;
		std::map<std::string, ODEPhysicsObject*> _objects;
	};
}

