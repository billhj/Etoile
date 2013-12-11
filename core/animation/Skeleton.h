/**
* Copyright(C) 2009-2012                
* @author Jing HUANG
* @file Skeleton.h
* @brief 
* @date 1/2/2011
*/

#pragma once
#include "Joint.h"

namespace Etoile
{
	class Skeleton
	{
	public:

		Skeleton(const std::string& name = "");
		Skeleton(Skeleton* skeleton);
		~Skeleton();
		void setName(const std::string& name){_name = name;}
		std::string getName(){return _name;}

		virtual int createJoint(const std::string& name = "", int parent = -1);
		int addJoint(Joint* joint, int parent);
		bool removeJoint(Joint* joint);
		bool removeJoint(unsigned int id);
		bool removeJoint(const std::string& name);

		Joint* getJoint(unsigned int id) const;
		Joint* getJoint(const std::string& name) const;
		int getJointIndex(const std::string& name)const;
		Joints& getJoints();

		void clear();
		void reset();
		void update();
		std::vector<Matrix4f> getMatrices();

	protected:
		Joints _joints;
		std::map<std::string, int> _nameIdMap;
		std::string _name;

	};
}
