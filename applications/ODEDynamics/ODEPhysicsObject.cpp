/**
* Copyright(C) 2009-2012                
* @author Jing HUANG
* @file ODEPhysicsObject.h
* @brief 
* @date 1/2/2011
*/


#include "ODEPhysicsObject.h"

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
	ODEPhysicsObject::ODEPhysicsObject(const std::string& name, SceneObject* obj): _name(name), _worldID(NULL), _spaceID(NULL), _pBody(NULL),
		_pMass(NULL), _pGeom(NULL), _pObj(obj), _type(NO_TYPE)
	{
	}


	ODEPhysicsObject::~ODEPhysicsObject(void)
	{
	}

	void ODEPhysicsObject::init(dWorldID worldID, dSpaceID spaceID)
	{
		_worldID = worldID;
		_spaceID = spaceID;
	}

	void ODEPhysicsObject::updateSceneObject()
	{
		if(_pObj == NULL || _pGeom == NULL || _pMass == NULL || _pBody == NULL)
		{
			return;
		}
		const dReal* p = _pBody->getPosition();
		const dReal* r = _pBody->getQuaternion();
		_pObj->setTransform(_pObj->getScale(), Quaternionf(r[1], r[2], r[3], r[0]).normalized(), Vec3f(p[0], p[1], p[2]));
	}

	void ODEPhysicsObject::updatePhysicsObject()
	{
		if(_pObj == NULL || _pGeom == NULL || _pMass == NULL || _pBody == NULL)
		{
			return;
		}
		Vec3f pos = _pObj->getPosition();
		_pBody->setPosition(pos.x(), pos.y(), pos.z());
		Quaternionf q = _pObj->getRotation();
		dReal r[4] = {q.w(), q.x(), q.y(), q.z()};
		_pBody->setQuaternion(r);
		
		if(_type == BOX_TYPE)
		{
			AxisAlignedBoundingBoxf* aabb = _pObj->getAABB();
			Vec3f scale = _pObj->getScale();
			Vec3f l = aabb->maximum() - aabb->minimum();
			_pMass->setBox (1, l.x() * scale.x(), l.y() * scale.y(), l.z() * scale.z());
			_pBody->setMass(_pMass);
			((dBox*)_pGeom)->create(_spaceID, l.x() * scale.x(), l.y() * scale.y(), l.z() * scale.z());
			((dBox*)_pGeom)->setBody(*_pBody);
		}
	}

	void ODEPhysicsObject::initBox()
	{
		if(_pObj == NULL)
		{
			return;
		}
		_type = BOX_TYPE;
		
		_pBody = new dBody();
		_pBody->create(_worldID);
		_pMass = new dMass();
		dBox* box = new dBox();
		updatePhysicsObject();
		_pGeom = box;
	}
}