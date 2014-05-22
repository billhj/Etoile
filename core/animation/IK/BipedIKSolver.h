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
		void solveLeftArm(Vec3f baseOffset, Vec3f armOffset);
		void solveRightArm(Vec3f baseOffset, Vec3f armOffset);
		void solveLeftLeg(Vec3f baseOffset, Vec3f legOffset);
		void solveRightLeg(Vec3f baseOffset, Vec3f legOffset);
		void applyConstraint(Joint* j, float xMin, float xMax, float yMin, float yMax, float zMin, float zMax);
	private:
		BipedSkeleton* _pSkeleton;
		std::map<std::string, Vec3f> _target;
		int _maxLoop;
		float _thresholdDistance;
		float _distanceStep;
		float _rotationStep;
	};
}

#endif //BIPED_IKSOLVER_H