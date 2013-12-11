/**
* Copyright(C) 2009-2012                
* @author Jing HUANG
* @file Joint.cpp
* @brief 
* @date 1/2/2011
*/

#include "Skeleton.h"

/**
* @brief For tracking memory leaks under windows using the crtdbg
*/
#if ( defined( _DEBUG ) || defined( DEBUG ) ) && defined( _MSC_VER )
#define _CRTDBG_MAP_ALLOC
#include <crtdbg.h>

#define DEBUG_NEW new( _NORMAL_BLOCK, __FILE__, __LINE__ )
#define new DEBUG_NEW
#endif

namespace Etoile
{

	Skeleton::Skeleton(const std::string& name): _name(name)
	{

	}

	Skeleton::Skeleton(Skeleton* skeleton)
	{
		_name = skeleton->getName();
		Joints joints = skeleton->getJoints();
		for(unsigned int i = 0; i < joints.size(); ++i){
			Joint* j = joints[i];
			int id =this->createJoint(j->getName(), j->getParentId());
			Joint* newj = this->getJoint(id);
			newj->setLocalPosition(j->getLocalPosition());
		}
		if(joints.size() > 0)
			this->getJoint(0)->update();
	}

	Skeleton::~Skeleton()
	{
		this->clear();
	
	}

	int Skeleton::createJoint(const std::string& name, int parent)
	{
		int id = _joints.size();
		Joint* joint = new Joint(name, this, id, parent);
		_joints.push_back(joint);
		_nameIdMap[name] = id;
		joint->setParent(parent);	
		return id;
	}

	int Skeleton::addJoint(Joint* joint, int parent)
	{
		joint->setSkeleton(this);
		int id = _joints.size();
		joint->setId(id);
		_joints.push_back(joint);
		_nameIdMap[joint->getName()] = id;

		joint->setParent(parent);
		return id;
	}


	bool Skeleton::removeJoint(Joint* joint)
	{
		return removeJoint(joint->getId());
	}

	bool Skeleton::removeJoint(const std::string& name)
	{
		return removeJoint(_nameIdMap[name]);
	}

	bool Skeleton::removeJoint(unsigned int id)
	{
		if(id < _joints.size())
		{

			Joint* joint = _joints[id];
			delete joint;
			_joints[id] = NULL;
			_nameIdMap.erase(joint->getName());
			return true;
		}
		return false;
	}

	Joint* Skeleton::getJoint(unsigned int id) const
	{
		if(id < _joints.size())
		{
			Joint* j = _joints[id];
			return j;
		}
		return NULL;
	}

	Joint* Skeleton::getJoint(const std::string& name) const
	{
		if(_nameIdMap.find(name)!=_nameIdMap.end())
		{
			int id = (*_nameIdMap.find(name)).second;
			return _joints[id];
		}
		return NULL;
	}


	int Skeleton::getJointIndex(const std::string& name)const
	{
		if(_nameIdMap.find(name)!=_nameIdMap.end())
		{
			int id = (*_nameIdMap.find(name)).second;
			return id;
		}
		return -1;
	}

	Joints& Skeleton::getJoints()
	{
		return _joints;
	}

	void Skeleton::clear()
	{
		Joints::iterator itor;
		for(itor = _joints.begin(); itor != _joints.end(); ++itor)
		{
			Joint* j = (*itor);
			if( j != 0)
			{
				delete j;
			}
		}
		_nameIdMap.clear();
		_joints.clear();
	}

	void Skeleton::reset()
	{
		if(_joints.size() > 0)
		{
			Joint* j = _joints[0];
			j->reset();
		}

	}

	void Skeleton::update()
	{
		Joints::iterator itor;
		for(itor = _joints.begin(); itor != _joints.end(); ++itor)
		{
			Joint* j = (*itor);
			if( j != 0)
			{
				j->update();
			}
		}
	}

	std::vector<Matrix4f> Skeleton::getMatrices()
	{
		update();
		std::vector<Matrix4f> matrices(_joints.size());
		for(unsigned int i = 0; i < _joints.size(); ++i)
		{
			Joint* j = _joints[i];
			int pId = j->getParentId();
			matrices[i] = j->getMatrix();
		}
		return matrices;
	}
}
