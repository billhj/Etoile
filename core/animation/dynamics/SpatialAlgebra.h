/**
* Copyright(C) 2009-2012                
* @author Jing HUANG
* @file SpatialAlgebra.h
* @brief 
* @date 1/2/2011
*/

#pragma once
#include <vector>
#include <iostream>
#include <Eigen/Dense>

namespace Etoile
{
	struct SpatialVector
	{
	public:

		SpatialVector()
		{
			_a = Eigen::Vector3f(0,0,0);
			_b = Eigen::Vector3f(0,0,0);
		}

		SpatialVector(Eigen::Vector3f a, Eigen::Vector3f b) :
		_a(a), _b(b)
		{
		}

		SpatialVector(const SpatialVector& v) :
		_a(v._a), _b(v._b)
		{
		}

		Eigen::Vector3f _a;
		Eigen::Vector3f _b;
		EIGEN_MAKE_ALIGNED_OPERATOR_NEW
	};

	struct SpatialTransform
	{
	public:

		SpatialTransform()
		{
			_rotation = Eigen::Matrix3f::Identity();
			_r = Eigen::Vector3f(0,0,0);
		}

		SpatialTransform(Eigen::Matrix3f rotation, Eigen::Vector3f r) :
		_rotation(rotation), _r(r)
		{
		}

		SpatialTransform(const SpatialTransform& t) :
		_rotation(t._rotation), _r(t._r)
		{
		}

		SpatialVector operator* (const SpatialVector &v)
		{
			return transform(v);
		}

		SpatialVector transform(const SpatialVector& vector)
		{
			SpatialVector sv;
			sv._a = _rotation * vector._a;
			sv._b = _r.cross(sv._a) + _rotation * vector._b;
			return sv;
		}

		SpatialVector inversetransform(const SpatialVector& vector)
		{
			return this->inverse().transform(vector);
		}

		SpatialTransform inverse()
		{
			return SpatialTransform(_rotation.transpose(), -_r);
		}

		Eigen::Matrix3f _rotation;
		Eigen::Vector3f _r;

		EIGEN_MAKE_ALIGNED_OPERATOR_NEW
	};


	struct  SpatialRigidBodyInertia 
	{
	public:
		SpatialRigidBodyInertia() : _mass(0.),
			_distance(Eigen::Vector3f::Zero(3,1)),
			_I(Eigen::Matrix3f::Zero(3,3))
		{}
		SpatialRigidBodyInertia (
			float mass, const Eigen::Vector3f &com_mass, const Eigen::Matrix3f &inertia) : 
		_mass(mass), _distance(com_mass), _I(inertia)
		{ }

		//input accelaration vector {omega, a}  ==> {torque, force}
		SpatialVector operator* (const SpatialVector &v) 
		{
			return SpatialVector (
				_I * v._a + (_distance.cross(v._b) - _distance.cross(_distance.cross(v._a))) * _mass,
				(v._b  - _distance.cross(v._a)) * _mass
			);
		}

		SpatialRigidBodyInertia operator+ (const SpatialRigidBodyInertia &rbi)
		{
			return SpatialRigidBodyInertia (
				_mass + rbi._mass,
				_distance + rbi._distance,
				_I + rbi._I
				);
		}

		
		/// Mass
		float _mass;
		/// distance to the center of mass
		Eigen::Vector3f _distance;
		/// Inertia expressed at the origin
		Eigen::Matrix3f _I;

		EIGEN_MAKE_ALIGNED_OPERATOR_NEW
	};

	class SpatialAlgebra
	{
	public:

	};

}