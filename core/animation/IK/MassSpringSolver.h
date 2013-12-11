/**
* Copyright(C) 2009-2012                
* @author Jing HUANG
* @file MassSpringSolver.h
* @brief 
* @date 1/2/2011
*/

#pragma once
#include "IKSolver.h"
#include "../MassSpring/MassSpring.h"


namespace Etoile
{

	class MassSpringSolver : public IKSolver
	{
	public:
		MassSpringSolver(int maxTries = 50, float targetThreshold = 0.5, int updateNumberForMassSpring = 30);
		virtual std::string getIKSolverName(){ return "MassSpring";}
		virtual void setupChain(std::vector<Joint*>&);
		virtual bool compute(std::vector<Joint*>& links, Vec3f, bool) override;
		void moveToTarget(std::vector<Joint*>& _links, Vec3f&);

		Vec3f updateUpVector(Vec3f, Vec3f);
		Quaternionf calculateGlobalRotation(Vec3f, Vec3f, Vec3f, Vec3f);
		Quaternionf calculateGlobalRotation(Vec3f, Vec3f);
		Quaternionf calculateLocalRotation(Quaternionf, Quaternionf);

		void updateMassesPosition(std::vector<Joint*>& _links);
		void updateUpVectors(std::vector<Joint*>& _links);
		
		void checkJointsDOFsRestrictions(std::vector<Joint*>& joints);
		void setMassSpringUpdateNumber(int number);
	private:
		MassSpring _massSpring;
		int _massSpringUpdateNumber;
		std::vector<Vec3f> _upvectors;
		std::vector<Vec3f> _o_upvectors;
	};
}
