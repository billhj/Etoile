/**
* Copyright(C) 2009-2012                
* @author Jing HUANG
* @file ODEPhysicsSkeleton.h
* @brief 
* @date 1/2/2011
*/


#include "ODEPhysicsSkeleton.h"
#include "geometry/SceneNode.h"
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
	ODEPhysicsSkeleton::ODEPhysicsSkeleton(dWorldID worldid, dSpaceID spaceid, dJointGroupID contactgroupID, const std::string& name, SceneEntity* obj): 
		_worldid(worldid), _spaceid(spaceid), _contactgroupID(contactgroupID), _name(name),_pObj(obj), _pSk(NULL), _density(1)
	{
	}


	ODEPhysicsSkeleton::~ODEPhysicsSkeleton(void)
	{
	}

	bool ODEPhysicsSkeleton::initSkeleton()
	{
		if(_pObj == NULL) return false;
		if(_pObj->getSkeleton() == NULL) return false;
		_pSk = _pObj->getSkeleton();
		//build bones
		for(unsigned int i = 0; i < _pSk->getJoints().size(); ++i)
		{
			Joint* j = _pSk->getJoints()[0];
			Joint* jp = j->getParent();
			if(jp != NULL)
			{
				dBodyID body = buildBone(jp->getName(),jp->getWorldPosition(), j->getWorldPosition(), 1);
			}
		}

		//buildjoint
		for(unsigned int i = 0; i < _pSk->getJoints().size(); ++i)
		{
			Joint* j = _pSk->getJoints()[0];
			Joint* jp = j->getParent();
			if(jp != NULL)
			{
				dJointID jointId = buildBallJoint(jp, j);
			}
		}
	}

	dJointID ODEPhysicsSkeleton::buildBallJoint(Joint* jp, Joint* j)
	{
		dBodyID parent = _bodyIds[_boneIds[jp->getName()]];
		dBodyID current = _bodyIds[_boneIds[j->getName()]];

		dJointID jointid = dJointCreateBall(_worldid, _contactgroupID);
		dJointAttach(jointid, parent, current);
		Vec3f pos = j->getWorldPosition();
		dJointSetBallAnchor(jointid, pos.x(), pos.y(), pos.z());
		return jointid;
	}

	dBodyID ODEPhysicsSkeleton::buildBone(const std::string& name, Vec3f start, Vec3f end, float radius)
	{
		dBodyID body = dBodyCreate(_worldid);
		dMass* mass = new dMass();
		dBodySetMass(body, mass);

		float cyllen = (start - end).length() - radius;
		//y direction
		mass->setCylinder(_density, 2, radius, cyllen);

		dGeomID geom = dCreateCylinder(_spaceid, radius, cyllen);
		dGeomSetBody (geom, body);
		Vec3f pos = (start + end) * 0.5;
		dBodySetPosition(body, pos.x(), pos.y(), pos.z());


		Vec3f dirOrig = (end-start);
		dirOrig.normalize();
		Vec3f localvec(0,1,0);
		Vec3f cross = localvec.cross3(dirOrig);
		//if the direction is inverse, so weird, then i change another axis
		if(cross == Vec3f())
		{
			cross = Vec3f(0,0,1);
		}
		float angle = (float)acos(dirOrig.dot3(localvec));
		Quaternionf newQuat(cross, angle);

		dReal r[4] = {newQuat.w(),newQuat.x(),newQuat.y(),newQuat.z()};
		dBodySetQuaternion(body, &r[0]);
		_boneIds[name] = _orientations.size();
		_orientations.push_back(newQuat);
		_bodyIds.push_back(body);
		return body;
	}

}