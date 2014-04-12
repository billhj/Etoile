/**
* Copyright(C) 2009-2012                
* @author Jing HUANG
* @file RigidBodySkeleton.h
* @brief 
* @date 1/2/2011
*/

#pragma once
#include "geometry/Skeleton.h"
#include "rbdl/Dynamics.h"
#include "rbdl/Model.h"
namespace Etoile
{

	class RigidBodySkeleton
	{
	public:
		RigidBodySkeleton(const std::string& name){_name = name;}
		virtual ~RigidBodySkeleton(){}
		std::string getName(){return _name;}
		void setSkeleton(Skeleton* skeleton){_pSkeleton = skeleton; build();}
		Skeleton* getSkeleton(){return _pSkeleton;}
		RigidBodyDynamics::Model* getModel(){return _pmodel;}
		void build()
		{	
			using namespace RigidBodyDynamics;
			if(_pSkeleton == NULL) return;
			_pmodel = new Model();
			_pmodel->gravity = Math::Vector3d (0., -9.81, 0.);
			Joints& joints = _pSkeleton->getJoints();
			_parentBodyId.push_back(0);
			for(unsigned int i = 1; i < joints.size(); ++i)
			{
				Joint* joint =  joints[i];
				Joint* parent = joint->getParent();
				/*if(parent == NULL)
				{
					Vec3f pos = joint->getWorldPosition();
					Body* body = new Body (1., Math::Vector3d (pos.x(), pos.y(), pos.z()), Math::Vector3d (1., 1., 1.));
					RigidBodyDynamics::Joint* joint = new RigidBodyDynamics::Joint(JointTypeRevolute, Math::Vector3d (0., 0., 1.));
					int body_a_id = _pmodel->AddBody(0, Math::Xtrans(Math::Vector3d(0., 0., 0.)), *joint, *body);
					_parentBodyId.push_back(body_a_id);
				}
				else*/
				{
					Vec3f pos = joint->getLocalPosition() * 0.5;
					Vec3f posT = parent->getLocalPosition();
					Body* body = new Body (1., Math::Vector3d (pos.x(), pos.y(), pos.z()), Math::Vector3d (1., 1., 1.));
					RigidBodyDynamics::Joint* joint = new RigidBodyDynamics::Joint(JointTypeRevolute, Math::Vector3d (0., 0., 1.));
					int p;
					
					p = _parentBodyId[parent->getId()];
					int body_a_id = _pmodel->AddBody(p, Math::Xtrans(Math::Vector3d(posT.x(), posT.y(), posT.z())), *joint, *body);
					_parentBodyId.push_back(body_a_id);
					_jointid.push_back(parent->getId());
				}
			}
			jointRotations = Math::VectorNd::Zero (_jointid.size());
			jointVelocitys = Math::VectorNd::Zero (_jointid.size());		
			torque = Math::VectorNd::Zero (_jointid.size());
		}

		void perform(double dt)
		{
			if(_pSkeleton == NULL) return;
			using namespace RigidBodyDynamics;
			Joints& joints = _pSkeleton->getJoints();
			jointAccelarations = Math::VectorNd::Zero (_jointid.size());
			
			ForwardDynamics(*_pmodel, jointRotations, jointVelocitys, torque, jointAccelarations, NULL);

			//std::cout<<"acc: " << jointAccelarations <<std::endl;
			jointVelocitys += (jointAccelarations * dt);

			//std::cout<<"vel: " <<jointVelocitys <<std::endl;
			jointRotations += (jointVelocitys * dt);

			bool b = true;
			while(b)
			{
				b = computeTorque(jointRotations,jointVelocitys, jointAccelarations, torque);
				//std::cout<< torque <<std::endl;
				ForwardDynamics(*_pmodel, jointRotations, jointVelocitys, torque, jointAccelarations, NULL);
				jointVelocitys += (jointAccelarations * dt);
				jointRotations += (jointVelocitys * dt);
			}

			torque = Math::VectorNd::Zero (_jointid.size());
			//std::cout<<"ang: " <<jointRotations <<std::endl;
			for(unsigned int i = 0; i < _jointid.size(); ++i)
			{
				Joint* joint =  joints[_jointid[i]];
				Quaternionf q;
				q.setAxisAngle(Vec3f(0., 0., 1), jointRotations[i]);
				joint->setLocalRotation(q);
			}
			_pSkeleton->update();
		}

		bool computeTorque(RigidBodyDynamics::Math::VectorNd& rotations, RigidBodyDynamics::Math::VectorNd& jointVelocitys,
			RigidBodyDynamics::Math::VectorNd& jointAccelarations,
			RigidBodyDynamics::Math::VectorNd& torque)
		{
			bool b = false;
			for(unsigned int i = 1; i < rotations.size() - 1; ++i)
			{
				torque[i] = 0;
				double r = rotations[i] * 100;
				torque[i] = -r;
			}
			return b;
		}
		void addTorque(int i, float t)
		{
			torque[i] += t * 100;
		}

		EIGEN_MAKE_ALIGNED_OPERATOR_NEW
	private:
		Skeleton* _pSkeleton;
		std::string _name;
		RigidBodyDynamics::Model* _pmodel;
		std::vector<int> _parentBodyId;
		RigidBodyDynamics::Math::VectorNd jointRotations;
		RigidBodyDynamics::Math::VectorNd jointVelocitys;
		RigidBodyDynamics::Math::VectorNd jointAccelarations;
		RigidBodyDynamics::Math::VectorNd torque;
		std::vector<int> _jointid;
		
	};

}