/**
* Copyright(C) 2009-2012                
* @author Jing HUANG
* @file Joint.h
* @brief 
* @date 1/2/2011
*/

#pragma once
#include <string>
#include <vector>
#include <map>
#include "math/Vectors.h"
#include "math/Quaternion.h"
#include "DOFConstraint.h"

namespace Etoile
{
	class Skeleton;
	class Joint
	{
	public:
		Joint(const std::string& name = "", Skeleton* skeleton = NULL, int id = -1 , int parentId = -1);

		std::string getName(){return _name;}
		void setName(const std::string& name){ _name = name;}
		int getId(){ return _id;}
		void setId(int id){ _id = id;}
		void setParent(int parent)
		{ 
			_parent = parent;
			if(_pSkeleton != NULL && _parent != -1)
			{
				if(!getParent()->hasChild(_id))
				{
					getParent()->getChildrenIds().push_back(_id);
				}
			}
		}
		bool hasChild(int id)
		{
			for(int i = 0; i < _children.size(); ++i)
			{
				if(_children[i] == id)
				{
					return true;
				}
			}
			return false;
		}

		int getParentId(){return _parent;}
		Joint* getParent()const;
		std::vector<int>& getChildrenIds(){return _children;}
		Skeleton* getSkeleton() const;
		void setSkeleton(Skeleton*);

		void rotate(const Quaternionf& rotation);
		virtual void update();
		void reset();

		//void setScale(Vec3f scale){ _scale = scale;}
		//Vec3f getScale(){return _scale;}

		void setLocalRotation(const Quaternionf& local);
		Quaternionf getLocalRotation();

		void setLocalPosition(const Vec3f& pos);
		const Vec3f& getLocalPosition();

		Quaternionf getWorldRotation();
		const Vec3f& getWorldPosition();

		Matrix4f getMatrix();

		DOFConstraints& getDOFConstraints(){return _dofs;}
		void addDOF(DOFConstraintType t, DOFConstraint dof){ _dofs[t] = dof;}
		/*Vec3f transformFromLocalToGlobalPosition(Vec3f);
		Vec3f transformFromOriginalGlobalToGlobalPosition(Vec3f);
		Vec3f transformFromLocalToGlobalOrienation(Vec3f);
		Vec3f computeOriginalLocalPosition(const Vec3f& globalposition);
		Vec3f localCoordinatesOfOriginalReference(const Vec3f& globalposition);*/
	protected:
		Skeleton* _pSkeleton;
		std::string _name;
		int _id;
		int _parent;
		std::vector<int> _children;
		Quaternionf _localRotation;
		Quaternionf _worldRotation;
		//Vec3f _scale;

		Vec3f _translateFromParent;
		Vec3f _worldPosition;
	
		DOFConstraints _dofs;
	};

	typedef std::vector<Joint*> Joints;

}
