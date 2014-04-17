/**
* Copyright(C) 2009-2012                
* @author Jing HUANG
* @file GameObject.cpp
* @brief 
* @date 1/2/2011
*/
#include "GameObject.h"

namespace Etoile
{
	GameObject::GameObject(GameObject* ref) : _pRef(ref)
	{
	
	}

	GameObject::GameObject(const std::string& name, GameObject* ref) : _name(name), _pRef(ref)
	{
	
	}

	void GameObject::copy(GameObject& object)
	{
		_name = object.name();
		_pRef = object._pRef;
		_scale = object._scale; //local
		_translation = object._translation;  //local
		_rotation = object._rotation;  //local
		_localmodelTransform = object._localmodelTransform;
		_globalmodelTransform = object._globalmodelTransform;
	}

	void GameObject::setTransform(const Vec3f& scale, const Quaternionf& rotate, const Vec3f& translate)
	{
		_scale = scale, _rotation = rotate, _translation = translate;
		_rotation.normalize();
		if(_pRef == NULL)
		{
			_localmodelTransform = Matrix4f().makeTranslation(_translation) * Matrix4f().makeAxisRotation(_rotation.axis(), _rotation.angle()) * Matrix4f().makeScale(_scale);
			_globalmodelTransform = _localmodelTransform;
		}
		else
		{
			_localmodelTransform = Matrix4f().makeTranslation(_translation) * Matrix4f().makeAxisRotation(_rotation.axis(), _rotation.angle()) * Matrix4f().makeScale(_scale);
			_globalmodelTransform = _pRef->getGlobalTransform() *  _localmodelTransform;
		}
	}

	void GameObject::updateTransform()
	{
		setTransform(_scale, _rotation, _translation);
	}

	Matrix4f& GameObject::getGlobalTransform()
	{
		return _globalmodelTransform;
	}

	Matrix4f& GameObject::getLocalTransform()
	{
		return _localmodelTransform;
	}

	void GameObject::setScale(const Vec3f& scale)
	{ 
		_scale = scale;
	}

	void GameObject::setTranslation(const Vec3f& translation)
	{
		_translation = translation;
	}

	void GameObject::setRotation(const Quaternionf& rotation)
	{
		_rotation = rotation;
		_rotation.normalize();
	}

}