/**
* Copyright(C) 2009-2012                
* @author Jing HUANG
* @file CCDSolver.cpp
* @brief 
* @date 1/2/2011
*/

#include "CCDSolver.h"
#include "math/MathHead.h"

namespace Etoile
{
	bool CCDSolver::compute(std::vector<Joint*>& _links, Vec3f target, bool enableConstraints)
	{
		/*for(unsigned int i = 0; i < _links.size(); ++i)
		{
			_links[i]->reset();
		}*/
		//TO DO
		/*
		add constraints
		do something when the cross product is 0
		this happens when the curVector and targetVector are collinears
		*/

		// start at the last link in the chain
		Joint*	link = _links[_links.size()-1]->getParent();

		Vec3f	rootPos  = link->getWorldPosition(), curEnd = _links[_links.size()-1]->getWorldPosition(), targetVector, curVector, crossResult, endPos = target;
		float	cosAngle,turnAngle;

		int tries = 0;

		float distance = Vec3f(curEnd - endPos).length();

		while (++tries < _maxTries &&
			distance > _targetThreshold)
		{
			// create the vector to the current effector pos
			curVector = curEnd - rootPos;
			// create the desired effector position vector
			targetVector = endPos - rootPos;

			// normalize the vectors (expensive, requires a sqrt)
			curVector.normalize();
			targetVector.normalize();

			// the dot product gives me the cosine of the desired angle
			cosAngle = curVector.dot3(targetVector);
			// if the dot product returns 1.0, i don't need to rotate as it is 0 degrees
			if (cosAngle < 0.9999999)
			{
				if(cosAngle < -0.9999999)
				{
					//the 2 vectors are collinear
					//TBD
					//
					// check if we can use cross product of source vector and [1, 0, 0]
					//
					crossResult.set(0, curVector.x(), -curVector.y());

					if( crossResult.length() < 1.0e-10 )
					{
						//
						// nope! we need cross product of source vector and [0, 1, 0]
						//
						crossResult.set( -curVector.z(), 0, curVector.x() ) ;
					}
				}
				else
				{
					// use the cross product to check which way to rotate
					crossResult = curVector.cross3(targetVector);
				}

				crossResult.normalize();
				turnAngle = acos(cosAngle);	// get the angle

				crossResult = link->getWorldRotation().inverse() * crossResult;
				Quaternionf rotation;
				rotation.setAxisAngle(crossResult, turnAngle);
				rotation.normalize();
				link->rotate(rotation);
				if(enableConstraints)
				{
					checkDOFsRestrictions(link, link->getDOFConstraints());
				}

				link->update();
			}

			if (link->getParent() == NULL)
			{
				link =  _links[_links.size()-1]->getParent();	// start of the chain, restart
			}
			else
			{
				link = link->getParent();
			}

			for(unsigned int i = 0; i < _links.size(); i++ )
			{
				_links[i]->update();

			}
			rootPos = link->getWorldPosition();
			curEnd = _links[_links.size()-1]->getWorldPosition();
			distance = Vec3f(curEnd - endPos).length();

		}

		if (tries == _maxTries)
		{
			return false;
		}

		return true;
	}
}
