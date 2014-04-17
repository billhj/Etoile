/**
* Copyright(C) 2009-2012                
* @author Jing HUANG
* @file GameObject.h
* @brief 
* @date 1/2/2011
*/

#pragma once
#ifndef GAME_OBJECT_H
#define GAME_OBJECT_H

#include <string>
#include "../math/MathHead.h"

namespace Etoile
{
	/**
	 * class GameObject: implement basic gameobject with simple transformation operations
	 */
	class GameObject
	{
	public:
		GameObject(GameObject* ref = NULL);
		GameObject(const std::string&, GameObject* ref = NULL);
		void copy(GameObject&);
		virtual~GameObject(){}
		const std::string name(){return _name;}
		GameObject* getReference(){return _pRef;}
		void setTransform(const Vec3f& scale, const Quaternionf& rotate, const Vec3f& translate);
		void updateTransform();
		Matrix4f& getGlobalTransform();
		Matrix4f& getLocalTransform();
		void setScale(const Vec3f& scale);
		Vec3f getScale() const{return _scale;}
		void setTranslation(const Vec3f& translation);
		Vec3f getTranslation() const {return _translation;}
		void setRotation(const Quaternionf& rotation);
		Quaternionf getRotation() const {return _rotation;}
	protected:
		std::string _name;
		GameObject* _pRef;
		Vec3f _scale; //local
		Vec3f _translation;  //local
		Quaternionf _rotation;  //local
		Matrix4f _localmodelTransform;
		Matrix4f _globalmodelTransform;
	};

}

#endif GAME_OBJECT_H