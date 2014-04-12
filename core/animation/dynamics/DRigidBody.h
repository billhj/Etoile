/**
* Copyright(C) 2009-2012                
* @author Jing HUANG
* @file RigidBody.h
* @brief 
* @date 1/2/2011
*/

#pragma once
#include <vector>
#include <iostream>
#include "DMass.h"
#if defined(_DEBUG) || defined(DEBUG)
#include <ctime>
#endif


namespace Etoile
{
	Eigen::Matrix3f getSkewMatrix(Eigen::Vector3f input);
	class DRigidBody
	{
		
	public:
		DRigidBody(const std::string& name);

		~DRigidBody();
		const std::string& getName() const;

		void setMass(const DMass& dmass);

		DMass& getMass();
		void setPosition(Eigen::Vector3f currentPos);
		Eigen::Vector3f position() const
		{
			return _position;
		}

		void setVelocity(Eigen::Vector3f speed);
		Eigen::Vector3f velocity() const
		{
			return _linearmomentum / _mass;
		}
		Eigen::Vector3f linearMomentum() const
		{
			return _linearmomentum;
		}
		void setRotation(Eigen::Matrix3f currentRotation);
		Eigen::Matrix3f rotation() const
		{
			return _rotation;
		}
		void setAngularMomentum(Eigen::Vector3f angularMomentum);
		Eigen::Vector3f angularMomentum()
		{
			return _angularmomentum;
		}
		void setAngularVelocity(Eigen::Vector3f angularvelocity);
		Eigen::Vector3f angularVelocity()
		{
			return _inverseI * _angularmomentum;
		}

		//void initialize(Eigen::Vector3f currentPos, Eigen::Vector3f speed, Eigen::Matrix3f currentRotation, Eigen::Vector3f angularMomentum)
		//{
		//	_position = currentPos;
		//	_rotation = currentRotation;
		//	_angularmomentum = angularMomentum;
		//	_linearspeed = speed;
		//	_linearmomentum = speed * _mass;

		//	//H_b = T_b/a * (I_a * omega_a)
		//	//    = T_b/a * (I_a * T_b/a' * T_b/a * omega_a)
		//	//    = T_b/a * (I_a * T_b/a' * T_b/a * omega_a)
		//	//	  =  T_b/a * (I_a * T_b/a' * omega_b)
		//	// H_b / omega_b = I_b
		//	//	I_b = T_b/a * I_a * T_b/a';
		//	Eigen::Matrix3f transposeRotation = _rotation.transpose();
		//	_inverseI = _rotation * _inverseIBody * transposeRotation;
		//	_angularspeed = _inverseI * _angularmomentum;
		//}

		void addForce(Eigen::Vector3f force);

		void addTorque(Eigen::Vector3f torque);

		void addForceAtRealPos(Eigen::Vector3f force, Eigen::Vector3f pos);

		void addTorqueAtRealPos(Eigen::Vector3f torque, Eigen::Vector3f pos);

		void update(float dt);

		Eigen::Vector3f getPosition();
		Eigen::Matrix3f getRotation();
		Eigen::Quaternionf getRotationQuaternion();

		void clearForce();

		void clearTorque();	
		EIGEN_MAKE_ALIGNED_OPERATOR_NEW
	protected:
		std::string _name;
		Eigen::Vector3f _position;    // position in time t	x(t)
		Eigen::Matrix3f _rotation;   //orientation in time t	R(t)
		Eigen::Quaternionf _rotationq;
		DMass _dmass;
		float _mass;
		Eigen::Matrix3f _IBody;
		Eigen::Matrix3f _inverseIBody;
		Eigen::Matrix3f _inverseI;

		//Eigen::Vector3f _linearspeed;
		Eigen::Vector3f _linearmomentum;   //momentum in time t     P(t)
		Eigen::Vector3f _angularmomentum; //angular momentum in time t		L(t)
		//Eigen::Vector3f _angularspeed;

		
		Eigen::Vector3f _force;
		Eigen::Vector3f _torque;

	
	};
}