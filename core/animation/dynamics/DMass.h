/**
* Copyright(C) 2009-2012                
* @author Jing HUANG
* @file DMass.h
* @brief 
* @date 1/2/2011
*/

#pragma once
#include <vector>
#include <Eigen/Dense>

namespace Etoile
{

	struct Particle
	{
		float _mass;
		Eigen::Vector3f _originalPosition;
		Eigen::Vector3f _originalRi;

		Eigen::Vector3f _position;
		Eigen::Vector3f _ri;
		Eigen::Vector3f _speed;


		void init(Eigen::Vector3f position, float mass)
		{
			_mass = mass;
			_position = position;
			_originalPosition = position;
		}
		/*Eigen::Vector3f updateSpeed(Eigen::Vector3f vMass, Eigen::Vector3f angular)
		{
			return vMass + angular.cross3(_ri);
		}*/
		EIGEN_MAKE_ALIGNED_OPERATOR_NEW
	};

	struct DMass
	{
		std::vector<Particle> _particles;
	};



}