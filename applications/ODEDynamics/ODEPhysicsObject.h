/**
* Copyright(C) 2009-2012                
* @author Jing HUANG
* @file ODEPhysicsObject.h
* @brief 
* @date 1/2/2011
*/

#pragma once
#include "ode/ode.h"
#include "geometry/SceneEntity.h"
#include <string>

namespace Etoile
{
	enum GEOMETRY_TYPE{
	NO_TYPE,
	BOX_TYPE,
	};
	class ODEPhysicsObject
	{
	public:
		ODEPhysicsObject(const std::string& name, SceneEntity*);
		~ODEPhysicsObject(void);
		void init(dWorldID worldID, dSpaceID spaceID);
		const std::string getName(){return _name;}
		dWorldID getWorldID(){return _worldID;}
		dSpaceID getSpaceID(){return _spaceID;}

		void setBody(dBody* body){ _pBody = body;}
		void setMass(dMass* mass){ _pMass = mass;}
		void setGeometry(dGeom* geom){ _pGeom = geom;}
		dBody* getBody(){return _pBody;}
		dMass* getMass(){return _pMass;}
		dGeom* getGeometry(){return _pGeom;}

		void initBox();

		SceneEntity* getSceneEntity(){return _pObj;}
		void updateSceneEntity();
		void updatePhysicsObject();
	protected:
		std::string _name;
		dBody* _pBody;
		dMass* _pMass;
		dGeom* _pGeom;

		dWorldID _worldID;
		dSpaceID _spaceID;

		SceneEntity* _pObj;
		GEOMETRY_TYPE _type;
		//Vec3f offset;
	};

}