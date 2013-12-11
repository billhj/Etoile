/**
* Copyright(C) 2009-2012                
* @author Jing HUANG
* @file MassSpring.cpp
* @brief 
* @date 1/2/2011
*/

#include "MassSpring.h"
#include "DistanceSpring.h"
/**
* @brief For tracking memory leaks under windows using the crtdbg
*/
#if ( defined( _DEBUG ) || defined( DEBUG ) ) && defined( _MSC_VER )
#define _CRTDBG_MAP_ALLOC
#include <crtdbg.h>

#define DEBUG_NEW new( _NORMAL_BLOCK, __FILE__, __LINE__ )
#define new DEBUG_NEW
#endif

namespace Etoile
{

	MassSpring::MassSpring()
	{

	}

	MassSpring::MassSpring(const std::string& name)
	{
		_name = name;
	}

	MassSpring::~MassSpring()
	{
		clear();
	}

	void MassSpring::clear()
	{
		for(unsigned int i = 0; i < _springs.size(); ++i)
		{
			Spring* spring = _springs[i];
			delete spring;
		}
		_springs.clear();

		for(unsigned int i = 0; i < _nodes.size(); ++i)
		{
			Mass* node = _nodes[i];
			delete node;
		}
		_nodes.clear();
	}


	int MassSpring::addMass(Mass* node)
	{
		int index = _nodes.size();
		_nodes.push_back(node);
		return index;
	}


	int  MassSpring::addSpring(Spring* spring)
	{
		int index =  _springs.size();
		_springs.push_back(spring);
		return index;
	}

	void MassSpring::computeSpringsInternalForce()
	{
		for(unsigned int i = 0; i < _springs.size(); ++i)
		{
			Spring* spring = _springs[i];
			spring->computerInternalForce();
		}
	}

	void MassSpring::moveNodes()
	{
		for(unsigned int i = 0; i < _nodes.size(); ++i)
		{
			Mass* node = _nodes[i];
			node->move();
		}
	}

	void MassSpring::update(unsigned int steps)
	{
		unsigned int n = 0;
		while(n < steps){
			computeSpringsInternalForce();
			moveNodes();
			n++;
		}
	}

}
