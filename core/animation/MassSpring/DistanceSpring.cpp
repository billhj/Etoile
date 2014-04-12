/**
* Copyright(C) 2009-2012                
* @author Jing HUANG
* @file DistanceSpring.cpp
* @brief 
* @date 1/2/2011
*/

#include "DistanceSpring.h"
#include "MassSpring.h"

/**
* @brief For tracking memory leaks under windows using the crtdbg
*/
#if ( defined( _DEBUG ) || defined( DEBUG ) ) && defined( _MSC_VER )
#define _CRTDBG_MAP_ALLOC
#include <crtdbg.h>

#define DEBUG_NEW new( _NORMAL_BLOCK, __FILE__, __LINE__ )
#define new DEBUG_NEW
#endif

namespace Etoile{

	DistanceSpring::DistanceSpring(Mass* node1, Mass* node2, float stiffness, float damping): Spring(stiffness, damping)   // _pSystem(system), _stiffness(stiffness), _damping(damping)
	{
		_nodes.resize(2);
		_nodesInternalForces.resize(2);
		_nodes[0] = node1, _nodes[1] = node2;
		setEquilibriumLengthByNodesDistance();
	}


	DistanceSpring::~DistanceSpring()
	{

	}


	void DistanceSpring::setEquilibriumLength(float length)
	{
		_releasedLength = length;
		if(length < 0) setEquilibriumLengthByNodesDistance();
	}


	void DistanceSpring::setEquilibriumLengthByNodesDistance()
	{
		Vec3f vect = Vec3f(_nodes[0]->getPosition() - _nodes[1]->getPosition());
		float lengthPos =  vect.length();
		setEquilibriumLength(lengthPos);
	}

	Mass* DistanceSpring::getMass1()
	{
		return _nodes[0];
	}

	Mass* DistanceSpring::getMass2()
	{
		return _nodes[1];
	}


	//todo: need to complete the dampling force
	void DistanceSpring::computerInternalForce()
	{
		if(!_actived) return;

		Mass* node1 = _nodes[0];
		Mass* node2 = _nodes[1];
		Vec3f vect = Vec3f(node1->getPosition() - node2->getPosition());

		float lengthPos =  vect.length();

		float dif = lengthPos - _releasedLength;

		//if(constraitActived()) return;
		if(dif == 0) return;
		float Felastic = -_stiffness * dif;

		_currentInternalForce = Felastic * (1 - _damping);

		vect.normalize();
		_nodesInternalForces[0] = vect * _currentInternalForce;
		_nodes[0]->addForce(_nodesInternalForces[0]);
		_nodesInternalForces[1] = -vect * _currentInternalForce;
		_nodes[1]->addForce(_nodesInternalForces[1]);
	}

	Vec3f DistanceSpring::getMass1InternalForce()
	{
		return _nodesInternalForces[0];
	}

	Vec3f DistanceSpring::getMass2InternalForce()
	{
		return _nodesInternalForces[1];
	}
}
