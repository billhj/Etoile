/**
* Copyright(C) 2009-2012                
* @author Jing HUANG
* @file MassSpringSolver.cpp
* @brief 
* @date 1/2/2011
*/

#include "MassSpringSolver.h"
#include "../../Math/MathHead.h"
#include "../MassSpring/DistanceSpring.h"

#define _MASSSPRING_GLOBALSOLUTION
namespace Etoile
{

	MassSpringSolver::MassSpringSolver(int maxTries, float targetThreshold, int updateNumberForMassSpring) : IKSolver(maxTries, targetThreshold), _massSpringUpdateNumber(updateNumberForMassSpring)
	{

	}
	void MassSpringSolver::setMassSpringUpdateNumber(int number)
	{
		_massSpringUpdateNumber = number;
	}

	void MassSpringSolver::setupChain(std::vector<Joint*>& joints)
	{
		_massSpring.clear();
		_upvectors.clear();
		_o_upvectors.clear();
		for(unsigned int i = 0; i < joints.size(); ++i)
		{
			joints[i]->reset();
			Mass* node = new Mass(joints[i]->getWorldPosition(), joints.size() - i);
			_massSpring.addMass(node);
		}

		Mass* baseNode = _massSpring.getMass(0);
		baseNode->setMovable(false);

		for(unsigned int i = 1; i < joints.size(); ++i)
		{
			DistanceSpring* spring = new DistanceSpring(_massSpring.getMass(i - 1), _massSpring.getMass(i));
			_massSpring.addSpring(spring);
			Vec3f bone =  _massSpring.getMass(i)->getPosition() - _massSpring.getMass(i - 1)->getPosition();
			bone.normalize();
			if(bone != Vec3f(0,1,0))
			{
				Vec3f up = updateUpVector(Vec3f(0,1,0), bone);
				_upvectors.push_back(up);
				_o_upvectors.push_back(up);
			}
			else
			{
				Vec3f up = updateUpVector(Vec3f(0,0,1), bone);
				_upvectors.push_back(up);
				_o_upvectors.push_back(up);
			}
		}

	}

	//TODO
	//need to check,  seems to get stable is much more important than constraints
	//parameter for MassSpring needs to be set  it seems force will be better then position,
	void MassSpringSolver::moveToTarget(std::vector<Joint*>& _links, Vec3f& target)
	{
		Mass* nodeCurrent = _massSpring.getMasses()[_links.size() - 1];
		int idx = 0;
		while(idx < _massSpringUpdateNumber * 10)
		{
			Vec3f force = target - nodeCurrent->getPosition();
			nodeCurrent->addForce(force / force.length() * 1);
			idx++;
			_massSpring.update(5);
		}

		//nodeCurrent->setPosition(target);
		//nodeCurrent->setMovable(false);
		//_massSpring.update(_massSpringUpdateNumber);
	}


	bool MassSpringSolver::compute(std::vector<Joint*>& _links, Vec3f target, bool enableConstraints)
	{
		setupChain(_links);
		int size = _links.size() - 1;
		Vec3f curEnd = _links[size]->getWorldPosition(), endPos = target;

		int tries = 0;
		endPos = checkForRetargetting(_links, endPos);
	
		while (tries < _maxTries &&
			Vec3f(curEnd - endPos).length() > _targetThreshold)
		{

			moveToTarget(_links, endPos);
			//_links[size]->setUpDirectionVector(target.getUpDirectionVector());
			std::vector<Quaternionf> global;

			for(unsigned int i = 1; i < _links.size(); i++ )
			{
				Vec3f bone = _massSpring.getMass(i)->getPosition() -  _massSpring.getMass(i - 1)->getPosition();
				_upvectors[i - 1] = updateUpVector(_upvectors[i - 1], bone);
				Quaternionf q = calculateGlobalRotation(_upvectors[i - 1], _o_upvectors[i - 1], bone, _links[i]->getLocalPosition());
				//Quaternionf q = calculateGlobalRotation(bone.normalized(), _links[i]->getOriginalLocalPosition().normalized());
				global.push_back(q);
			}

			_links[0]->setLocalRotation(global[0]);
			//_links[0]->rotate(global[0]);
			for(unsigned int i = 1; i < _links.size() - 1; i++ )
			{
				_links[i]->setLocalRotation(calculateLocalRotation(global[i - 1], global[i]));
				//_links[i]->rotate(calculateLocalRotation(global[i - 1], global[i]));
			}

			if(enableConstraints)
				checkJointsDOFsRestrictions(_links);
			for(unsigned int i = 0; i < _links.size(); i++ )
			{
				_links[i]->update();
				//std::cout<<i <<" "<< _links[i]->getLocalPosition().length() <<"  "<<_links[i]->getWorldPosition()<<std::endl;
			}
			updateUpVectors(_links);
			updateMassesPosition(_links);

			curEnd = _links[size]->getWorldPosition();
			
			++tries;
		}
		for(unsigned int i = 0; i < _links.size(); i++ )
			{
				_links[i]->update();
				//std::cout<<i <<" "<<_links[i]->getWorldPosition()<<std::endl;
			}

		if (tries == _maxTries)
		{
			return false;
		}

		return true;
	}

	

	Vec3f MassSpringSolver::updateUpVector(Vec3f upOriginal, Vec3f bone)
	{
		bone.normalize();
		upOriginal.normalize();
		Vec3f vPerpendicular = bone.cross3(upOriginal);
		Vec3f upV = vPerpendicular.cross3(bone);
		return upV.normalized();
	}

	Quaternionf MassSpringSolver::calculateGlobalRotation(Vec3f a, Vec3f a_original, Vec3f b, Vec3f b_original)
	{
		return Quaternionf::fromRotatedTriangle(a_original, a, b_original, b);
	}

	Quaternionf MassSpringSolver::calculateGlobalRotation(Vec3f b, Vec3f b_original)
	{
		Quaternionf rotation;
		rotation.setAxisAngle(b.cross3(b_original), -acos(b_original.dot3(b)));
		return rotation;
	}

	Quaternionf MassSpringSolver::calculateLocalRotation(Quaternionf parent, Quaternionf j)
	{
		return  parent.inverse() * j;
	}



	void MassSpringSolver::updateMassesPosition(std::vector<Joint*>& _links)
	{
		for(unsigned int i = 0; i < _links.size() ; ++i)
		{
			Joint* j = _links[i];
			_massSpring.getMass(i)->setPosition(j->getWorldPosition());
		}
	}

	void MassSpringSolver::updateUpVectors(std::vector<Joint*>& _links)
	{
		for(unsigned int i = 0; i < _links.size() - 1; ++i)
		{
			_upvectors[i] = _links[i]->getWorldRotation() * Vec3f(0,1,0);
		}
	}

	// for ensureThe constaints no to pass to much, need to think about later
	void MassSpringSolver::checkJointsDOFsRestrictions(std::vector<Joint*>& joints)
	{

		int start = joints.size() - 1;
		for(int i = start; i >= 0; --i )
		{
			Joint* joint = joints[i];
			//std::cout<<joint->getName()<<std::endl;
			Quaternionf _localRotation = joint->getLocalRotation();
			const DOFConstraints& _dofs = joint->getDOFConstraints();
			Vec3f angle = _localRotation.getEulerAngleXYZ();
			Vec3f difference;
			bool modified = false;

			//Quaternionf y_parent;
			if(angle[0] < _dofs[ROTATION_X].minValue())
			{
				difference[0] = _dofs[ROTATION_X].minValue() - angle[0];
				angle[0] = _dofs[ROTATION_X].minValue();

				modified = true;
			}
			else if(angle[0]  > _dofs[ROTATION_X].maxValue())
			{
				difference[0] = _dofs[ROTATION_X].maxValue() - angle[0];
				angle[0] = _dofs[ROTATION_X].maxValue();

				modified= true;
			}

			if(angle[1]  < _dofs[ROTATION_Y].minValue())
			{
				difference[1] = _dofs[ROTATION_Y].minValue() - angle[1];
				angle[1]  = _dofs[ROTATION_Y].minValue();

				// y_parent.fromEulerXYZ(0, difference[1],0);
				modified= true;

			}
			else if(angle[1]  > _dofs[ROTATION_Y].maxValue())
			{
				difference[1] = _dofs[ROTATION_Y].maxValue() - angle[1];
				angle[1]  = _dofs[ROTATION_Y].maxValue();

				//y_parent.fromEulerXYZ(0, difference[1],0);
				modified= true;
			}

			if(angle[2]  < _dofs[ROTATION_Z].minValue())
			{
				difference[2] = _dofs[ROTATION_Z].minValue()  - angle[2];
				angle[2] = _dofs[ROTATION_Z].minValue();

				modified= true;
			}
			else if(angle[2] > _dofs[ROTATION_Z].maxValue())
			{
				difference[2] = _dofs[ROTATION_Z].maxValue() - angle[2];
				angle[2] = _dofs[ROTATION_Z].maxValue();

				modified= true;
			}

			joint->setLocalRotation(_localRotation);
			if(modified)
			{
				Quaternionf old(_localRotation);
				_localRotation.fromEulerXYZ(angle[0],angle[1],angle[2]);
				joint->setLocalRotation(_localRotation);

				if(i > 0)
				{
					joint->update();
					Quaternionf q;
					q  =  _localRotation * old.inverse();
					float distanceJoint = (Vec3f(joints[i - 1]->getWorldPosition()) - joint->getWorldPosition()).length();
					float distanceTarget = Vec3f(Vec3f(joint->getWorldPosition()) - joints[start]->getWorldPosition()).length();
					float influenceFactor = distanceTarget/ (distanceTarget + distanceJoint);
					q.invert();
					Quaternionf parent = Quaternionf::slerp(Quaternionf(), q, influenceFactor);
					joints[i - 1]->rotate(parent);
					joints[i - 1]->update();
				}

			}
			joints[i]->update();
		}

	}
}
