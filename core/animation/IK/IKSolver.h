/**
* Copyright(C) 2009-2012                
* @author Jing HUANG
* @file IKSolver.h
* @brief 
* @date 1/2/2011
*/

#pragma once
#include <vector>
#include "geometry/Joint.h"
#include "geometry/DOFConstraint.h"

namespace Etoile
{
	enum IKSolverType
	{
		CCD,
		JACOBIAN_DLS,
		JACOBIAN_PSEUDO_INVERSE,
		JACOBIAN_TRANSPOSE,
		MASS_SPRING
	};

	class IKSolver
	{
	protected:
		int _maxTries;
		float _targetThreshold;
		float _stepweight;

	public:
		virtual std::string getIKSolverName() = 0;
		inline IKSolver(int maxTries = 50, float targetThreshold = 0.5)
			: _maxTries(maxTries),
			_targetThreshold(targetThreshold)
		{ 
			_stepweight = 1;
		}

		virtual bool compute(std::vector<Joint*>& _links, Vec3f, bool) = 0;

		inline float getSingleStepValue() const
		{
			return _stepweight;
		}

		void setSingleStepValue(float v)
		{
			this->_stepweight = v;
		}

		inline float getTargetThreshold() const
		{
			return _targetThreshold;
		}

		void setTargetThreshold(float targetThreshold)
		{
			this->_targetThreshold = targetThreshold;
		}

		inline int getMaxNumberOfTries() const
		{
			return _maxTries;
		}

		inline void setMaxNumberOfTries(int tries)
		{
			this->_maxTries = tries;
		}

		Vec3f checkForRetargetting(std::vector<Joint*>& joints, Vec3f endpos)
		{
			float sumdistance = 0;
			for(unsigned int i = 0; i < joints.size(); ++i)
			{
				sumdistance += joints[i]->getLocalPosition().length();
			}
			Vec3f basePos = joints[0]->getWorldPosition();
			Vec3f vecDistance = endpos - basePos;
			float distance = vecDistance.length();
			if(sumdistance < distance){
				return basePos + vecDistance * sumdistance / (distance) ;
			}
			return endpos;
		}

		virtual void checkDOFsRestrictions(Joint* joint, const DOFConstraints& dofs)
		{
			Quaternionf _localRotation = joint->getLocalRotation();
			Vec3f angle = _localRotation.getEulerAngleXYZ();

			bool modified = false;

			if(angle[0] < dofs[ROTATION_X].minValue())
			{
				angle[0] = dofs[ROTATION_X].minValue();
				modified = true;
			}
			else if(angle[0]  > dofs[ROTATION_X].maxValue())
			{
				angle[0] = dofs[ROTATION_X].maxValue();
				modified= true;
			}

			if(angle[1]  < dofs[ROTATION_Y].minValue())
			{
				angle[1]  = dofs[ROTATION_Y].minValue();
				modified= true;
			}
			else if(angle[1]  > dofs[ROTATION_Y].maxValue())
			{
				angle[1]  = dofs[ROTATION_Y].maxValue();
				modified= true;
			}

			if(angle[2]  < dofs[ROTATION_Z].minValue())
			{
				angle[2] = dofs[ROTATION_Z].minValue();
				modified= true;
			}
			else if(angle[2] > dofs[ROTATION_Z].maxValue())
			{
				angle[2] = dofs[ROTATION_Z].maxValue();
				modified= true;
			}

			if(modified)
			{
				_localRotation.fromEulerXYZ(angle[0],angle[1],angle[2]);
				joint->setLocalRotation(_localRotation);
			}

		}

	};
}

