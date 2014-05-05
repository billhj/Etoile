/**
* Copyright(C) 2009-2012                
* @author Jing HUANG
* @file BipedIKSolver.h
* @brief 
* @date 1/2/2011
*/
#pragma once
#ifndef BIPED_IKSOLVER_H
#define BIPED_IKSOLVER_H

#include "geometry/BipedSkeleton.h"

namespace Etoile
{
	class BipedIKSolver
	{
	public:
		BipedIKSolver(BipedSkeleton* skel);
		void solve();
		void addTarget(const std::string& jointname, Vec3f target);
	private:
		BipedSkeleton* _pSkeleton;
		std::map<std::string, Vec3f> _target;
	};
}

#endif //BIPED_IKSOLVER_H