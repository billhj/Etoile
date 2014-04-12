/**
* Copyright(C) 2009-2012                
* @author Jing HUANG
* @file DMultiBodySolver.h
* @brief 
* @date 1/2/2011
*/

#pragma once
#include <vector>
#include <iostream>
#include "DSkeleton.h"


namespace Etoile
{

	class DMultiBodySolver
	{

	public:
		DMultiBodySolver()
		{
		}

		~DMultiBodySolver(){}
		virtual void solve() = 0;
	};


	class FeatherStoneSolver
	{
	public:
		virtual void solve(DSkeleton* current,
			std::vector<float> torque)
		{
			Joints& joints =  current->getJoints();
			for(unsigned int i = 0; i < joints.size(); ++i)
			{
			
			}
		}
	};

}