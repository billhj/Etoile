/**
* Copyright(C) 2009-2012                
* @author Jing HUANG
* @file DJoint.cpp
* @brief 
* @date 1/2/2011
*/

#include "DJoint.h"

namespace Etoile
{
	void setAnchors(DJoint* j, float x, float y, float z, Eigen::Vector3f& anchor1, Eigen::Vector3f& anchor2)
	{
		std::vector<DRigidBody*>& bodys = j->getBody();
		if(bodys[0] != NULL)
		{
			Eigen::Vector3f ri;
			ri[0] = x - bodys[0]->getPosition().x();
			ri[1] = y - bodys[0]->getPosition().y();
			ri[2] = z - bodys[0]->getPosition().z();
	
			anchor1 = bodys[0]->getRotation().inverse() * ri;
			if (bodys[1] != NULL)
			{
				ri[0] = x - bodys[1]->getPosition().x();
				ri[1] = y - bodys[1]->getPosition().y();
				ri[2] = z - bodys[1]->getPosition().z();
				anchor2 = bodys[1]->getRotation().inverse() * ri;
			}
			else
			{
				anchor2[0] = x;
				anchor2[1] = y;
				anchor2[2] = z;
			}
		}
		//anchor1[3] = 0;
		//anchor2[3] = 0;
	}

	void setAxes(DJoint* j, float x, float y, float z, Eigen::Vector3f& axis1, Eigen::Vector3f& axis2)
	{
		std::vector<DRigidBody*>& bodys = j->getBody();
		if(bodys[0] != NULL)
		{
			Eigen::Vector3f axis;
			axis[0] = x;
			axis[1] = y;
			axis[2] = z;
			axis.normalize();

			axis1 = bodys[0]->getRotation().inverse() * axis;
	
			if (bodys[1] != NULL)
			{
				axis2 = bodys[1]->getRotation().inverse() * axis;
			}
			else
			{
				axis2[0] = x;
				axis2[1] = y;
				axis2[2] = z;
			}
		}
	}

	Eigen::Vector3f getAxis1(DJoint* j, Eigen::Vector3f axis)
	{
		assert(j->getBody()[0]);
		Eigen::Vector3f result;
		return j->getBody()[0]->getRotationQuaternion() * axis;
	}

	Eigen::Vector3f getAxis2(DJoint* j, Eigen::Vector3f axis)
	{
		assert(j->getBody()[1]);
		Eigen::Vector3f result;
		return j->getBody()[1]->getRotationQuaternion() * axis;
	}

	Eigen::Vector3f getAnchor1(DJoint* j, Eigen::Vector3f anchor)
	{
		assert(j->getBody()[0]);
		Eigen::Vector3f result;
		return j->getBody()[0]->getRotationQuaternion() * anchor;
	}

	Eigen::Vector3f getAnchor2(DJoint* j, Eigen::Vector3f anchor)
	{
		assert(j->getBody()[1]);
		Eigen::Vector3f result;
		return j->getBody()[1]->getRotationQuaternion() * anchor;
	}



	/*void setAxis(Eigen::Vector3f axis)
	{
	
	}*/

	void addHingeTorque(DJoint* j, float torque)
	{
		DJointHinge* jh = (DJointHinge*)(j); 
		assert(jh);
		Eigen::Vector3f axis = jh->getAxis() * torque;
		std::vector<DRigidBody*>& bodys = j->getBody();
		if(bodys[0] != NULL)
		{
			bodys[0]->addTorque(axis);
		}
		if(bodys[1] != NULL)
		{
			bodys[1]->addTorque(-axis);
		}
	}


	void DJointHinge::setAnchor(Eigen::Vector3f anchor)
	{
		setAnchors(this, anchor.x(), anchor.y(), anchor.z(), _anchor1, _anchor2);
	}

	Eigen::Vector3f DJointHinge::getAnchor()
	{
		return getAnchor1(this, _anchor1);
	}

	void DJointHinge::setAxis(Eigen::Vector3f axis)
	{
		setAxes(this, axis.x(), axis.y(), axis.z(), _axis1, _axis2);
	}

	Eigen::Vector3f DJointHinge::getAxis()
	{
		return getAxis1(this, _axis1);
	}

	void DJointHinge::attach(DRigidBody* body0, DRigidBody* body1, Eigen::Vector3f anchor)
	{
		this->setBody(0, body0);
		this->setBody(1, body1);
		setAnchor(anchor);
	}

	void DJointBall::setAnchor(Eigen::Vector3f anchor)
	{
		setAnchors(this, anchor.x(), anchor.y(), anchor.z(), _anchor1, _anchor2);
	}

	Eigen::Vector3f DJointBall::getAnchor()
	{
		return getAnchor1(this, _anchor1);
	}

	void DJointBall::attach(DRigidBody* body0, DRigidBody* body1, Eigen::Vector3f anchor)
	{
		this->setBody(0, body0);
		this->setBody(1, body1);
		setAnchor(anchor);
	}
}