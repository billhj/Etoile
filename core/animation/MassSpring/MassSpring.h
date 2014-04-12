/**
* Copyright(C) 2009-2012                
* @author Jing HUANG
* @file MassSpring.h
* @brief 
* @date 1/2/2011
*/

#pragma once
#include "Mass.h"
#include "Spring.h"

//it`s important the parameter to define the particles
#define STIFFNESS 0.8f
//#define SPRINGDAMPING 0.3f
//#define EQUILIBRIUMLENGTH 1.0f

namespace Etoile
{


	class MassSpring
	{
	public:
		MassSpring();
		MassSpring(const std::string& name);
		~MassSpring();
		const std::string getName(){ return _name;}
		void clear();
		virtual void init(){}
		virtual void reset(){}

		int addMass(Mass* node);
		Mass* getMass(unsigned int handle)
		{
			if( handle >= _nodes.size()) return NULL;
			return _nodes[handle];
		}

		int addSpring(Spring* spring);
		Spring* getSpring(unsigned int handle)
		{
			if( handle >= _springs.size()) return NULL;
			return _springs[handle];
		}

		Masses& getMasses(){return _nodes;}
		Springs& getSprings(){return _springs;}

		void update(unsigned int steps = 10);

	protected:
		void computeSpringsInternalForce();
		void moveNodes();

		std::string _name;
		Masses _nodes;
		Springs _springs;
	};

}

