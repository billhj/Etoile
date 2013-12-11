/**
* Copyright(C) 2009-2012                
* @author Jing HUANG
* @file JacobianTransposeSolver.h
* @brief 
* @date 1/2/2011
*/

#pragma once
#include "IKSolver.h"

namespace Etoile
{
	class JacobianTransposeSolver : public IKSolver
	{
	public:
		JacobianTransposeSolver(int maxTries = 50, float targetThreshold = 0.5):IKSolver(maxTries, targetThreshold){}
		virtual std::string getIKSolverName(){ return "JacobianTranspose";}
		virtual bool compute(std::vector<Joint*>& _links, Vec3f, bool) override;
	};
}
