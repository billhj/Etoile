/**
* Copyright(C) 2009-2012                
* @author Jing HUANG
* @file JacobianDLSSolver.h
* @brief 
* @date 1/2/2011
*/

#pragma once
#include "IKSolver.h"

namespace Etoile
{

	class JacobianDLSSolver : public IKSolver
	{
	public:
		JacobianDLSSolver(int maxTries = 50, float targetThreshold = 0.5):IKSolver(maxTries, targetThreshold){}
		virtual std::string getIKSolverName(){ return "JacobianDLS";}
		virtual bool compute(std::vector<Joint*>& _links, Vec3f, bool) override;
	};
}
