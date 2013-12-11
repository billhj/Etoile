/**
* Copyright(C) 2009-2012                
* @author Jing HUANG
* @file SceneObject.cpp
* @brief 
* @date 1/2/2011
*/

#include "SceneObject.h"

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
	SceneObject::SceneObject(): _pRef(NULL)
	{
		setScale(Vec3f(1,1,1));
		setTranslation(Vec3f());
		setRotation(Quaternionf());
		setTransform();
	}

	SceneObject::SceneObject(const SceneObject& obj)
	{
		(*this) = obj;
		setTransform();
	}

	SceneObject::SceneObject(const Vec3f& translate, const Quaternionf& rotation, const Vec3f& scale, SceneObject* ref):  _pRef(ref)
	{
		if(_pRef == NULL)
		{
			setScale(scale);
			setTranslation(translate);
			setRotation(rotation);
		}
		else
		{
			setScale(scale);
			setTranslation(translate);
			setRotation(rotation);
		}
	}

	void SceneObject::set(const Vec3f& translate, const Quaternionf& rotation, const Vec3f& scale, SceneObject* ref)
	{
		_pRef = ref;
		if(_pRef == NULL)
		{
			setScale(scale);
			setTranslation(translate);
			setRotation(rotation);
		}
		else
		{
			setScale(scale);
			setTranslation(translate);
			setRotation(rotation);
		}
	}

	SceneObject& SceneObject::operator=(const SceneObject& obj)
	{
	  // setTranslationAndRotation.
		setScale(obj.getScale());
		setTranslation(obj.getTranslation());
		setRotation(obj.getRotation());
		setReferenceObject(obj.getReferenceObject());
		return *this;
	}

	void SceneObject::setReferenceObject(SceneObject* obj)
	{
		_pRef = obj;
	}

	void SceneObject::setScale(const Vec3f& scale)
	{ 
		_scale = scale;
	}

	void SceneObject::setTranslation(const Vec3f& translation)
	{
		_translation = translation;
	}

	void SceneObject::setRotation(const Quaternionf& rotation)
	{
		_rotation = rotation;
		_rotation.normalize();
	}

	Matrix4f SceneObject::getModelMatrix(){return _modelTransform;}
	Matrix4f SceneObject::getGLModelMatrix()
	{
		Matrix4f model;
		_modelTransform.transpose(model);
		return model;
	}

	void SceneObject::setPosition(const Vec3f& position)
	{
		const SceneObject* so = getReferenceObject();
		if(so)
		{
			Vec3f localWithoutScale = so->getOrientation().inverse() * (position - so->getPosition());
			localWithoutScale.scale(1.0/_scale.x(), 1.0/_scale.y(), 1.0/_scale.z());
			_translation = localWithoutScale;
		}
		else
		{
			Vec3f pos = position;
			pos.scale(1.0/_scale.x(), 1.0/_scale.y(), 1.0/_scale.z());
			_translation = position;
		}
		/**
		*	_translation = computeLocalPosition(position)
		*/
	}

	Vec3f SceneObject::getPosition() const 
	{
		const SceneObject* so = getReferenceObject();
		Vec3f v = _translation;
		while(so)
		{
			Vec3f scaleSo = so->getScale();
			v.scale(scaleSo.x(), scaleSo.y(), scaleSo.z());
			v = so->getRotation() * v + so->getTranslation();
			so = so->getReferenceObject();
		}
		return v;

		/*
			return computeGlobalPosition(Vec3f(0,0,0));
		*/
	}

	/**
	* set by global orientation
	*/
	void SceneObject::setOrientation(const Quaternionf& orientation)
	{
		if (this->getReferenceObject())
			setRotation(this->getReferenceObject()->getOrientation().inverse() * orientation);
		else
			setRotation(orientation);
	}

	/**
	* get global orientation
	*/
	Quaternionf SceneObject::getOrientation() const
	{
		Quaternionf res = getRotation();
		const SceneObject* so = getReferenceObject();
		while (so != NULL)
		{
			res = so->getRotation() * res;
			so  = so->getReferenceObject();
		}
		return res;
	}

	/**
	* give a global position, return the local position in this obj space
	*/
	Vec3f SceneObject::computeLocalPosition(const Vec3f& globalposition) const
	{
		const SceneObject* so = getReferenceObject();
		if (so)
			return localCoordinatesOfReference(so->computeLocalPosition(globalposition));
		else
			return localCoordinatesOfReference(globalposition);
	}

	/**
	* give referenced globalpositionm, return reference local position
	*/
	Vec3f SceneObject::localCoordinatesOfReference(const Vec3f& globalposition) const
	{
		Vec3f local = getRotation().inverse() * (globalposition - getTranslation());
		local.scale(1.0/_scale.x(), 1.0/_scale.y(), 1.0/_scale.z());
		return local;
	}

	/**
	* give a local position, return the global position
	*/
	Vec3f SceneObject::computeGlobalPosition(const Vec3f& localposition) const
	{
		const SceneObject* so = this;
	    Vec3f res = localposition;
	    while (so != NULL)
		{
			res = so->coordinatesOfReference(res);
			so  = so->getReferenceObject();
		}
	    return res;
	}

	/**
	* give a local position, return the global-reference position in this reference obj space
	*/
	Vec3f SceneObject::coordinatesOfReference(const Vec3f& localPosition) const
	{
		Vec3f pos = localPosition;
		pos.scale(_scale.x(), _scale.y(), _scale.z());
		return this->getRotation() * pos  + this->getTranslation();
	}


}