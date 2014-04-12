/**
* Copyright(C) 2009-2012                
* @author Jing HUANG
* @file DRigidBody.cpp
* @brief 
* @date 1/2/2011
*/


#include "DRigidBody.h"

namespace Etoile
{

	Eigen::Matrix3f getSkewMatrix(Eigen::Vector3f input)
	{
		Eigen::Matrix3f output;
		output<<0, -input[2], input[1],
			input[2], 0, -input[0],
			-input[1], input[0], 0;
		return output;
	}

	DRigidBody::DRigidBody(const std::string& name):  _name(name)
	{
		_rotation = Eigen::Matrix3f::Identity();
		_rotationq = Eigen::Quaternionf(_rotation);
		_IBody = Eigen::Matrix3f::Identity();
		_inverseIBody = Eigen::Matrix3f::Identity();
		_inverseI = Eigen::Matrix3f::Identity();

		_position = Eigen::Vector3f(0,0,0);
		_linearmomentum = Eigen::Vector3f(0,0,0);
		_angularmomentum = Eigen::Vector3f(0,0,0);

		_force = Eigen::Vector3f(0,0,0);
		_torque = Eigen::Vector3f(0,0,0);
	}

	DRigidBody::~DRigidBody(){}
	const std::string& DRigidBody::getName() const{ return _name;}

	void DRigidBody::setMass(const DMass& dmass)
	{
		_dmass = dmass;
		_mass = 0;

		std::vector<Particle>& particles = _dmass._particles;
		for(unsigned int i = 0; i < particles.size(); ++i)
		{
			_mass += particles[i]._mass;
			_position += particles[i]._originalPosition * particles[i]._mass;
			particles[i]._position = (particles[i]._originalPosition);
		}
		_position /= _mass;
		for(unsigned int i = 0; i < particles.size(); ++i)
		{
			particles[i]._originalRi = particles[i]._position - _position;
			particles[i]._ri = particles[i]._originalRi;

			Eigen::Matrix3f skew = getSkewMatrix(particles[i]._ri);
			Eigen::Matrix3f skewskew = skew * skew;
			_IBody -=  ( skewskew * particles[i]._mass);
		}
		_inverseIBody = _IBody.inverse();
		_inverseI = _inverseIBody;
	}

	DMass& DRigidBody::getMass()
	{
		return _dmass;
	}

	void DRigidBody::setPosition(Eigen::Vector3f currentPos)
	{
		_position = currentPos;
	}

	void DRigidBody::setVelocity(Eigen::Vector3f speed)
	{
		_linearmomentum = speed * _mass;
	}

	void DRigidBody::setRotation(Eigen::Matrix3f currentRotation)
	{
		_rotation = currentRotation;
		Eigen::Matrix3f transposeRotation = _rotation.transpose();
		_inverseI = _rotation * _inverseIBody * transposeRotation;
	}
	void DRigidBody::setAngularVelocity(Eigen::Vector3f angularvelocity)
	{
		_angularmomentum = _inverseI.inverse() * angularvelocity;
	}

	void DRigidBody::setAngularMomentum(Eigen::Vector3f angularMomentum)
	{
		_angularmomentum = angularMomentum;
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

	void DRigidBody::addForce(Eigen::Vector3f force)
	{
		_force += force;
	}

	void DRigidBody::addTorque(Eigen::Vector3f torque)
	{
		_torque += torque;
	}

	void DRigidBody::addForceAtRealPos(Eigen::Vector3f force, Eigen::Vector3f pos)
	{
		Eigen::Vector3f ri = pos - _position;
		addForce(force);
		addTorque(ri.cross(force));
	}

	void DRigidBody::addTorqueAtRealPos(Eigen::Vector3f torque, Eigen::Vector3f pos)
	{
		Eigen::Vector3f ri = pos - _position;
		addForce(torque.cross(ri));
		addTorque(torque);
	}

	void DRigidBody::update(float dt)
	{
#if defined(_DEBUG) || defined(DEBUG)
		//clock_t  timerS, timerE;
		//timerS = clock();
#endif
		std::vector<Particle>& particles = _dmass._particles;

		//update center of mass position
		Eigen::Vector3f speed = _linearmomentum / _mass;
		_position  = (_position + speed * dt);
		//std::cout<<_position<<std::endl;
		//update momentum
		_linearmomentum = _linearmomentum  + _force * dt;

		//update rotation"""""????????????

		Eigen::Vector3f angularVelocity = _inverseI * _angularmomentum;
		Eigen::Matrix3f omega = getSkewMatrix(angularVelocity);
		Eigen::Matrix3f  r = _rotation * dt;
		_rotation = _rotation + omega * r;
		_rotationq = Eigen::Quaternionf(_rotation);
		_rotationq.normalize();
		_rotation = _rotationq.toRotationMatrix();
		//_rotation.checkRotation();

		//update angular momentum
		_angularmomentum = _angularmomentum + _torque * dt;

		//update inverse of angular inertia
		Eigen::Matrix3f transposeRotation = _rotation.transpose();  
		_inverseI = _rotation * _inverseIBody * transposeRotation;

		//update angular speed
		angularVelocity = _inverseI * _angularmomentum;
		//std::cout<<"angular speed : " << _angularspeed << " _angularmomentum: "<<_angularmomentum<<std::endl;

		//update ri
		for(unsigned int i = 0; i < particles.size(); ++i)
		{
			//have pb for matrix 
			particles[i]._ri = _rotation * particles[i]._originalRi;
			//std::cout<<i << "   "<< particles[i]._originalRi.norm() << " norm " <<particles[i]._ri.norm()<<std::endl;
			particles[i]._position = _position + particles[i]._ri;
			//std::cout<<i << "    "<<particles[i]._position<<std::endl;
			particles[i]._speed = speed + angularVelocity.cross(particles[i]._ri);
		}
#if defined(_DEBUG) || defined(DEBUG)
		//timerE = clock();
		//double seconds = timerE - timerS;
		//std::cout<<"seconds:  "<<seconds /CLOCKS_PER_SEC<<std::endl;
#endif
	}


}