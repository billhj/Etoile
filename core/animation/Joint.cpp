/**
* Copyright(C) 2009-2012                
* @author Jing HUANG
* @file Joint.cpp
* @brief 
* @date 1/2/2011
*/

#include "Joint.h"
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


	Joint::Joint(const std::string& name, Skeleton* skeleton, int id , int parentId)
		: _name(name), _id(id),
		_parent(parentId),_pSkeleton(skeleton), _scale(1)
	{
		for(unsigned int i = 0; i < 6; ++i)
		{
			_dofs.push_back(DOF(-3.14159266, 3.14159266));
		}
	}

	Joint* Joint::getParent() const
	{
		if(_parent == -1)
		{
			return NULL;
		}
		assert(_pSkeleton != NULL);
		return _pSkeleton->getJoint(_parent);
	}

	Skeleton* Joint::getSkeleton() const
	{ 
		return _pSkeleton;
	}

	void Joint::setSkeleton(Skeleton* skeleton)
	{ 
		_pSkeleton = skeleton;
	}

	void Joint::rotate(const Quaternionf& rotation)
	{
		Quaternionf tmp = rotation;
		tmp.normalize();
		_localRotation *= tmp;
	}

	void Joint::update()
	{
		if(_parent == -1)
		{
			_worldRotation = _localRotation ;
			_worldRotation.normalize();
			_worldPosition = _worldRotation * Vec3f(_translateFromParent);
		}
		else
		{
			_worldPosition = getParent()->getWorldRotation() * Vec3f(_translateFromParent.x() * getParent()->getScale().x(), _translateFromParent.y() * getParent()->getScale().y(), _translateFromParent.z() * getParent()->getScale().z());
			_worldPosition += getParent()->getWorldPosition();
			//Given two quaternions p, q and multiplying them to form the quaternion r = pq,
			//applying the quaternion r to a vector v first rotates the vector by q then by p.
			//still wonder which should be first local or global G1 = G0 * L   or G1 =  L * G0
			_worldRotation = getParent()->getWorldRotation() * _localRotation;
			_worldRotation.normalize();
		}
	}

	void Joint::reset()
	{
		_localRotation = Quaternionf();
		update();
	}

	void Joint::setLocalRotation(const Quaternionf& local)
	{
		_localRotation = local;
	}

	Quaternionf Joint::getLocalRotation()  
	{
		return _localRotation;
	}

	Quaternionf Joint::getWorldRotation()  
	{
		return _worldRotation;
	}

	void Joint::setLocalPosition(const Vec3f& pos)
	{
		_translateFromParent = pos;
	}

	const Vec3f& Joint::getLocalPosition() 
	{
		return _translateFromParent;
	}

	const Vec3f& Joint::getWorldPosition()  
	{
		return _worldPosition;
	}

	Vec3f Joint::transformFromOriginalGlobalToGlobalPosition(Vec3f global)
	{
		Vec3f localPoint = computeOriginalLocalPosition(global);
		return  _worldPosition + _worldRotation * Vec3f(_scale.x() * localPoint.x(), _scale.y() * localPoint.y(), _scale.z() * localPoint.z());
	}

	Vec3f Joint::transformFromLocalToGlobalPosition(Vec3f localPoint)
	{
		return  _worldPosition + _worldRotation * Vec3f(_scale.x() * localPoint.x(), _scale.y() * localPoint.y(), _scale.z() * localPoint.z());
	}

	Vec3f Joint::transformFromLocalToGlobalOrienation(Vec3f localorientation)
	{
		return _worldRotation * localorientation;
	}

	Vec3f Joint::computeOriginalLocalPosition(const Vec3f& globalposition)
	{
		Joint* so = getParent();
		if (so)
			return localCoordinatesOfOriginalReference(so->computeOriginalLocalPosition(globalposition));
		else
			return localCoordinatesOfOriginalReference(globalposition);
	}

	Vec3f Joint::localCoordinatesOfOriginalReference(const Vec3f& globalposition)
	{
	  return (globalposition - _translateFromParent);
	}

	Matrix4f Joint::getMatrix()
	{
		Matrix4f rmatrix, smatrix, tmatrix;
		smatrix.makeScale(_scale);
		_worldRotation.getMatrix(rmatrix);
		tmatrix.makeTranslation(_worldPosition);
		return tmatrix * rmatrix * smatrix;
	}
}
