/**
* Copyright(C) 2009-2012                
* @author Jing HUANG
* @file JacobianTransposeSolver.cpp
* @brief 
* @date 1/2/2011
*/

#include "JacobianTransposeSolver.h"
#include "math/MathHead.h"

namespace Etoile
{
	bool JacobianTransposeSolver::compute(std::vector<Joint*>& _links, Vec3f target, bool enableConstraints)
	{
		/*for(unsigned int i = 0; i < _links.size(); ++i)
		{
			_links[i]->reset();
		}*/

		int size = _links.size() - 1;
		Vec3f rootPos, curEnd = _links[size]->getWorldPosition(), targetVector, curVector, endPos = target;
		float distance;
		int tries = 0;

		MatrixMNf jacobian(3, size);
		std::vector<Vec3f> axis(size);


		Vec3f difference = endPos - curEnd;
		distance = difference.length();

		while (++tries < _maxTries &&
			distance > _targetThreshold)
		{

			//compute the jacobian
			for(int i = 0; i < size; i++)
			{
				rootPos = _links[i]->getWorldPosition();	// Position of current node

				curVector    = curEnd - rootPos;	// Vector current link -> current effector position
				targetVector = endPos - rootPos;	// Vector current link -> target

				curVector.normalize();
				targetVector.normalize();

				//rotation axis
				//axis[i] = targetVector.cross3(curVector);
				axis[i] = curVector.cross3(targetVector);
				axis[i].normalize();

				Vec3f entry = -curVector.cross3(axis[i]);
				entry.normalize();

				axis[i] = _links[i]->getWorldRotation().inverse() * axis[i];
				VecNf v(3, &entry[0]);
				jacobian.setColumn(i, v);
			}

			//compute the transpose
			MatrixMNf transpose = jacobian.transpose();

			//setting the force
			//VecNf force(6);

			VecNf force(3);
			force[0] = difference.x() * _stepweight;
			force[1] = difference.y() * _stepweight;
			force[2] = difference.z() * _stepweight;

			//computing q'
			VecNf q = transpose * force;

			//Integrate and apply changes
			for(int i = 0; i < size; i++ )
			{
				Quaternionf rotation(axis[i], q[i] * 0.01);
				rotation.normalize();
				_links[i]->rotate(rotation);
				if(enableConstraints)
				{
					checkDOFsRestrictions(_links[i], _links[i]->getDOFConstraints());
				}
				//_links[i]->update();
			}

			for(unsigned int i = 0; i < _links.size(); i++ )
			{
				_links[i]->update();

			}
			curEnd = _links[size]->getWorldPosition();
			difference = endPos - curEnd;
			distance = difference.length();

		}

		if (tries == _maxTries)
		{
			return false;
		}

		return true;
	}
}
