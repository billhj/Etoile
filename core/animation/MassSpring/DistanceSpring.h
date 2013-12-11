/**
* Copyright(C) 2009-2012                
* @author Jing HUANG
* @file DistanceSpring.h
* @brief 
* @date 1/2/2011
*/

#pragma once
#include "Mass.h"
#include "Spring.h"

namespace Etoile
{
	class DistanceSpring : public Spring
	{
	public:

		// if equilibriumLength < 0 then, the length equals the original length between two nodes,  stiffness should be small for avoiding the jump of nodes
		DistanceSpring(Mass* node1, Mass* node2, float stiffness = 1, float damping = 0.1);
		~DistanceSpring();

		Mass* getMass1();
		Mass* getMass2();

		void setEquilibriumLength(float length);
		virtual void computerInternalForce() override;
		virtual bool constraitActived(float differenceLength = 0){ std::cout << differenceLength<<std::endl;return false;}
		Vec3f getMass1InternalForce();
		Vec3f getMass2InternalForce();

		void setEquilibriumLengthByNodesDistance();

	private:

	};

	typedef std::vector<DistanceSpring*> DistanceSprings;
}

