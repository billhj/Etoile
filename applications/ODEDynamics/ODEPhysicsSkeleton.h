/**
* Copyright(C) 2009-2012                
* @author Jing HUANG
* @file ODEPhysicsSkeleton.h
* @brief 
* @date 1/2/2011
*/

#pragma once
#include "ode/ode.h"
#include "geometry/SceneEntity.h"
#include <string>

namespace Etoile
{
	class ODEPhysicsSkeleton
	{
	public:
		ODEPhysicsSkeleton(dWorldID worldid, dSpaceID spaceid, dJointGroupID contactgroupID, const std::string& name, SceneEntity*);
		bool initSkeleton();
		dBodyID buildBone(const std::string& name, Vec3f start, Vec3f end, float radius);
		~ODEPhysicsSkeleton(void);
		const std::string getName(){return _name;}
		SceneEntity* getSceneEntity(){return _pObj;}

		dJointID buildBallJoint(Joint* parent, Joint* j);

	protected:
		std::string _name;
		SceneEntity* _pObj;
		Skeleton* _pSk;
		dWorldID _worldid;
		dSpaceID _spaceid;
		dJointGroupID _contactgroupID;
		float _density;

		std::map<std::string, int> _boneIds;
		std::vector<dBodyID> _bodyIds;
		std::vector<Quaternionf> _orientations;
	};

}