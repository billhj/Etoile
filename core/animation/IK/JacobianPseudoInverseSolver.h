/**
* Copyright(C) 2009-2012                
* @author Jing HUANG
* @file JacobianPseudoInverseSolver.h
* @brief 
* @date 1/2/2011
*/

#pragma once
#include "IKSolver.h"

namespace Etoile
{
	class JacobianPseudoInverseSolver : public IKSolver
	{
	public:
		JacobianPseudoInverseSolver(int maxTries = 50, float targetThreshold = 0.5): IKSolver(maxTries, targetThreshold){}
		virtual std::string getIKSolverName(){ return "JacobianPseudoInverse";}
		virtual bool compute(std::vector<Joint*>& _links, Vec3f, bool) override;

	};
}
