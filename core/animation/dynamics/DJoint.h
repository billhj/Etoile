/**
* Copyright(C) 2009-2012                
* @author Jing HUANG
* @file DJoint.h
* @brief 
* @date 1/2/2011
*/

#pragma once
#include <vector>
#include <Eigen/Dense>
#include "DRigidBody.h"

namespace Etoile
{
	
	class DJoint
	{
	public:
		DJoint()
		{
			_bodys.resize(2);
		}

		/*Eigen::Quaternionf getRotationQuaternion()
		{
			assert(_bodys[0]);
			return _bodys[0]->getRotationQuaternion();
		}

		Eigen::Matrix3f getRotation()
		{
			assert(_bodys[0]);
			return _bodys[0]->getRotation();
		}*/
		virtual void setAnchor(Eigen::Vector3f anchor) = 0;
		virtual Eigen::Vector3f getAnchor() = 0;
		void setBody(int idx, DRigidBody* body)
		{
			if(_bodys.size() <= idx)
			{
				_bodys.resize(idx + 1);
			}
			_bodys[idx] = body;
		}
		std::vector<DRigidBody*>& getBody(){return _bodys;}
	protected:
		std::vector<DRigidBody*> _bodys;
	};

	void setAnchors(DJoint* j, float x, float y, float z, Eigen::Vector3f& anchor1, Eigen::Vector3f& anchor2);
	void setAxes(DJoint* j, float x, float y, float z, Eigen::Vector3f& axis1, Eigen::Vector3f& axis2);
	Eigen::Vector3f getAnchor1(DJoint* j, Eigen::Vector3f axis);
	Eigen::Vector3f getAnchor2(DJoint* j, Eigen::Vector3f axis);
	Eigen::Vector3f getAxis1(DJoint* j, Eigen::Vector3f axis);
	Eigen::Vector3f getAxis2(DJoint* j, Eigen::Vector3f axis);
	void addHingeTorque(DJoint* j, float torque );
	

	class DJointHinge : public DJoint
	{
	public:
		DJointHinge(): DJoint()
		{
		}
		void addTorque(float value)
		{
			addHingeTorque(this, value);
		}
		void attach(DRigidBody* body0, DRigidBody* body1, Eigen::Vector3f anchor);
		virtual void setAnchor(Eigen::Vector3f anchor);
		virtual Eigen::Vector3f getAnchor();
		void setAxis(Eigen::Vector3f axis);
		Eigen::Vector3f getAxis();	
		EIGEN_MAKE_ALIGNED_OPERATOR_NEW
	protected:
		Eigen::Vector3f _axis1;
		Eigen::Vector3f _axis2;
		Eigen::Vector3f _anchor1;   // anchor w.r.t first body
		Eigen::Vector3f _anchor2;   // anchor w.r.t second body

	
	};

	class DJointBall : public DJoint
	{
	public:
		DJointBall(): DJoint()
		{
		}
		virtual void setAnchor(Eigen::Vector3f anchor);
		virtual Eigen::Vector3f getAnchor();
		void attach(DRigidBody* body0, DRigidBody* body1, Eigen::Vector3f anchor);
		EIGEN_MAKE_ALIGNED_OPERATOR_NEW
	protected:
		Eigen::Vector3f _anchor1;   // anchor w.r.t first body
		Eigen::Vector3f _anchor2;   // anchor w.r.t second body

		
	};
}