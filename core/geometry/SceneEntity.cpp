/**
* Copyright(C) 2009-2012                
* @author Jing HUANG
* @file SceneEntity.cpp
* @brief 
* @date 1/2/2011
*/

#include "SceneEntity.h"

namespace Etoile
{
	SceneEntity::SceneEntity() : MovableObject(), _pMesh(NULL), _pSkeleton(NULL)
	{
	
	}

	SceneEntity::SceneEntity(const SceneEntity& obj) : MovableObject(obj)
	{
		(*this) = obj;
	}

	void SceneEntity::addMesh(Mesh* mesh)
	{
		_pMesh = mesh;
	}

	void SceneEntity::addSkeleton(Skeleton* skeleton)
	{
		_pSkeleton = skeleton;
	}

	void SceneEntity::computerAABB()
	{
		std::vector<AxisAlignedBoundingBoxf*> aabbs;
		_pMesh->computeAABB();
		aabbs.push_back(_pMesh->getAABB());
		_aabb.build(aabbs);
	}

	void SceneEntity::perform(Matrix4f& gltransformation)
	{
		if(_pSkeleton != NULL)
		{
			_pSkeleton->perform(gltransformation);
		}

		if(_pMesh != NULL)
		{
			_pMesh->perform(gltransformation);
		}
	}
}