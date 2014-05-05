/**
* Copyright(C) 2009-2012                
* @author Jing HUANG
* @file BipedIKSolver.cpp
* @brief 
* @date 1/2/2011
*/
#include "BipedIKSolver.h"

namespace Etoile
{
	
	BipedIKSolver::BipedIKSolver(BipedSkeleton* skel) : _pSkeleton(skel)
	{
		
	}
	
	void BipedIKSolver::solve()
	{
	
	}

	void BipedIKSolver::addTarget(const std::string& jointname, Vec3f target)
	{
		_target[jointname] = target;
	}
}