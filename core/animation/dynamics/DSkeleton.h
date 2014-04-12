/**
* Copyright(C) 2009-2012                
* @author Jing HUANG
* @file DSkeleton.h
* @brief 
* @date 1/2/2011
*/

#pragma once
#include <vector>
#include <iostream>
#include "DJoint.h"
#include "DRigidBody.h"
#include "geometry/Skeleton.h"

namespace Etoile
{
	
	class DSkeleton : public Skeleton
	{
		
	public:
		DSkeleton(const std::string& name):  Skeleton(name)
		{
		}

		virtual ~DSkeleton(){}
		
		void addBody(DRigidBody* body)
		{
			_bodys.push_back(body);
		}

		void apply()
		{   

			//for(int i = 0;  i < _joints.size(); ++i)
			//{
			//	DJoint* joint = _joints[i];
			//	DRigidBody* p = joint->getBody()[0];
			//	DRigidBody* s = joint->getBody()[1];
			//	int idxp = _bodyIds[p->getName()];
			//	int idxs = _bodyIds[s->getName()];
			//	Eigen::Vector3f ri_j2s = s->getPosition() - joint->getAnchor();
			//	//compute velocity   vo = vp + w x co
			//	s->setVelocity(p->velocity() + _jointsvelocitys[i].cross(ri_j2s));
			//	//compute angular velocity
			//	s->setAngularVelocity(p->angularVelocity() + _jointsvelocitys[i]);
			//}

			//for(int i = 0;  i < _bodys.size(); ++i)
			//{
			//	//DRigidBody* body = _bodys[i];
			//	if(i == 0)
			//	{
			//		velocity[i] = Eigen::Vector3f();
			//		accelaration[i] = Eigen::Vector3f();
			//	}else
			//	{
			//		velocity[i] = velocity[i - 1];
			//		accelaration[i] = accelaration[i - 1];
			//	}
			//}
		}


	
	public:
		std::map<std::string, int> _bodyIds;
		std::vector<DRigidBody*> _bodys;
		std::vector<int> _parentLinkIds;
		std::vector<std::vector<int>> _childrenLinkIds;
		//std::vector<Eigen::Vector3f> _velocitys;
		std::vector<Eigen::Vector3f> _jointAxis;
		std::vector<float> _jointsrotations;
		std::vector<float> _jointsvelocitys;
		std::vector<float> _accelarations;
	};
}