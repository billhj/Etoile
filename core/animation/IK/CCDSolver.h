/**
* Copyright(C) 2009-2012                
* @author Jing HUANG
* @file CCDSolver.h
* @brief 
* @date 1/2/2011
*/

#pragma once
#include "IKSolver.h"


namespace Etoile
{

	class CCDSolver : public IKSolver
	{
	public:
		CCDSolver(int maxTries = 50, float targetThreshold = 0.5):IKSolver(maxTries, targetThreshold){}
		virtual std::string getIKSolverName(){ return "CCD";}
		virtual bool compute(std::vector<Joint*>& links, Vec3f, bool) override;
	};
}
