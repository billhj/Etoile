/**
* Copyright(C) 2009-2012                
* @author Jing HUANG
* @file SceneEntity.h
* @brief 
* @date 1/2/2011
*/

#pragma once
#include "math/MathHead.h"
#include "MovableObject.h"
#include "Mesh.h"
#include "Skeleton.h"
namespace Etoile
{
	class SceneEntity : public MovableObject
	{
	public:
		SceneEntity();
		SceneEntity(const SceneEntity& obj);
		void addMesh(Mesh* mesh);
		void addSkeleton(Skeleton* skeleton);
		Mesh* getMesh(){return _pMesh;}
		Skeleton* getSkeleton(){return _pSkeleton;}
		virtual void computerAABB() override;
		virtual void perform(Matrix4f& gltransformation) override;
	protected:
		Mesh* _pMesh;
		Skeleton* _pSkeleton;
	};

}