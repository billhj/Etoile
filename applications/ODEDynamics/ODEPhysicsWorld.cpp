/**
* Copyright(C) 2009-2012                
* @author Jing HUANG
* @file ODEPhysicsWorld.h
* @brief 
* @date 1/2/2011
*/


#include "ODEPhysicsWorld.h"
/**
* @brief For tracking memory leaks under windows using the crtdbg
*/
#if ( defined( _DEBUG ) || defined( DEBUG ) ) && defined( _MSC_VER )
#define _CRTDBG_MAP_ALLOC
#include <crtdbg.h>

#define DEBUG_NEW new( _NORMAL_BLOCK, __FILE__, __LINE__ )
#define new DEBUG_NEW
#endif

#include <iostream>
namespace Etoile
{
	dWorldID worldID;
	dSpaceID spaceID;
	dJointGroupID contactgroupID;

	void nearCollisionCallback(void* _this, dGeomID o1, dGeomID o2)
	{
		//std::cout<<"hittest"<<std::endl;
		// exit without doing anything if the two bodies are connected by a joint
		dBodyID b1 = dGeomGetBody(o1);
		dBodyID b2 = dGeomGetBody(o2);
		if (b1 && b2 && dAreConnected (b1,b2)) return;

		/*dContact contact;
		contact.surface.mode = 0;
		contact.surface.mu = dInfinity;
		if (dCollide (o1,o2,1,&contact.geom,sizeof(dContactGeom))) {
		dJointID c = dJointCreateContact (worldID, contactgroupID, &contact);
		dJointAttach (c,b1,b2);
		}*/
		const int N = 10;
		dContact contact[N];
		int n = dCollide (o1,o2,N,&contact[0].geom,sizeof(dContact));
		if (n > 0) 
		{
			for (int i=0; i<n; i++) {
				contact[i].surface.mode = dContactSlip1 | dContactSlip2 |
					dContactSoftERP | dContactSoftCFM | dContactApprox1;
				contact[i].surface.mu = dInfinity;
				contact[i].surface.mu2 = 0;
				contact[i].surface.bounce = 0.1;
				contact[i].surface.bounce_vel = 0.1;
				contact[i].surface.slip1 = 0.1;
				contact[i].surface.slip2 = 0.1;
				contact[i].surface.soft_erp = 0.5;
				contact[i].surface.soft_cfm = 0.3;
				dJointID c = dJointCreateContact (worldID, contactgroupID,&contact[i]);
				dJointAttach (c,
					dGeomGetBody(contact[i].geom.g1),
					dGeomGetBody(contact[i].geom.g2));
			}
		}
	}

	ODEPhysicsWorld::ODEPhysicsWorld(void): _worldID(NULL), _spaceID(NULL), _contactgroupID(NULL), _groundID(NULL)
	{

	}


	ODEPhysicsWorld::~ODEPhysicsWorld(void)
	{
	}

	void ODEPhysicsWorld::init()
	{
		// create world
		dInitODE2(0);
		_worldID = dWorldCreate();
		_spaceID = dHashSpaceCreate (0);
		_contactgroupID = dJointGroupCreate (1000000);

		worldID = _worldID;
		spaceID = _spaceID;
		contactgroupID = _contactgroupID;
	}

	void ODEPhysicsWorld::createPlane(double a, double b, double c, double d)
	{
		_groundID = dCreatePlane (_spaceID, a, b, c, d);
	}

	void ODEPhysicsWorld::createGravity(double x, double y, double z)
	{
		dWorldSetGravity (_worldID, x, y, z);
	}

	void ODEPhysicsWorld::doPhysics()
	{
		dSpaceCollide(_spaceID, 0, &(nearCollisionCallback));
		dWorldQuickStep(_worldID, 1);
		dJointGroupEmpty(_contactgroupID);
		//dJointGroupDestroy(_contactgroupID);
		std::map<std::string, ODEPhysicsObject*>::iterator itor = _objects.begin();
		for(; itor != _objects.end(); ++itor)
		{
			itor->second->updateSceneEntity();
		}
	}

	void ODEPhysicsWorld::release()
	{
		dJointGroupDestroy(_contactgroupID);
		dSpaceDestroy (_spaceID);
		dWorldDestroy (_worldID);
		dCloseODE();
	}

	void ODEPhysicsWorld::addPhysicsObject(ODEPhysicsObject* object)
	{
		if(object->getSpaceID() == _spaceID && object->getWorldID() == _worldID)
		{
			_objects[object->getName()] = object;
		}
	}

	void ODEPhysicsWorld::updatePhysicsObjects()
	{
		std::map<std::string, ODEPhysicsObject*>::iterator itor = _objects.begin();
		for(; itor != _objects.end(); ++itor)
		{
			itor->second->updatePhysicsObject();
		}
	}
}