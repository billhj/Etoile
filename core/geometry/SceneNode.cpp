/**
* Copyright(C) 2009-2012                
* @author Jing HUANG
* @file SceneNode.cpp
* @brief 
* @date 1/2/2011
*/

#include "SceneNode.h"
#include "MovableObject.h"
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
	SceneNode::SceneNode(): _pRef(NULL)
	{
		setScale(Vec3f(1,1,1));
		setTranslation(Vec3f());
		setRotation(Quaternionf());
		updateTransform();
		generateID();
	}

	SceneNode::SceneNode(const SceneNode& obj)
	{
		(*this) = obj;
		updateTransform();
	}

	SceneNode::SceneNode(const Vec3f& translate, const Quaternionf& rotation, const Vec3f& scale, SceneNode* ref):  _pRef(ref)
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
		generateID();
	}

	void SceneNode::set(const Vec3f& translate, const Quaternionf& rotation, const Vec3f& scale, SceneNode* ref)
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

	SceneNode& SceneNode::operator=(const SceneNode& obj)
	{
	  // setTranslationAndRotation.
		setScale(obj.getScale());
		setTranslation(obj.getTranslation());
		setRotation(obj.getRotation());
		setReferenceNode(obj.getReferenceNode());
		return *this;
	}

	void SceneNode::setReferenceNode(SceneNode* obj)
	{
		_pRef = obj;
	}

	void SceneNode::setScale(const Vec3f& scale)
	{ 
		_scale = scale;
	}

	void SceneNode::setTranslation(const Vec3f& translation)
	{
		_translation = translation;
	}

	void SceneNode::setRotation(const Quaternionf& rotation)
	{
		_rotation = rotation;
		_rotation.normalize();
	}

	Matrix4f SceneNode::getModelMatrix(){return _modelTransform;}
	Matrix4f SceneNode::getGLModelMatrix()
	{
		Matrix4f model;
		_modelTransform.transpose(model);
		return model;
	}

	void SceneNode::setPosition(const Vec3f& position)
	{
		const SceneNode* so = getReferenceNode();
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

	Vec3f SceneNode::getPosition() const 
	{
		const SceneNode* so = getReferenceNode();
		Vec3f v = _translation;
		while(so)
		{
			Vec3f scaleSo = so->getScale();
			v.scale(scaleSo.x(), scaleSo.y(), scaleSo.z());
			v = so->getRotation() * v + so->getTranslation();
			so = so->getReferenceNode();
		}
		return v;

		/*
			return computeGlobalPosition(Vec3f(0,0,0));
		*/
	}

	/**
	* set by global orientation
	*/
	void SceneNode::setOrientation(const Quaternionf& orientation)
	{
		if (this->getReferenceNode())
			setRotation(this->getReferenceNode()->getOrientation().inverse() * orientation);
		else
			setRotation(orientation);
	}

	/**
	* get global orientation
	*/
	Quaternionf SceneNode::getOrientation() const
	{
		Quaternionf res = getRotation();
		const SceneNode* so = getReferenceNode();
		while (so != NULL)
		{
			res = so->getRotation() * res;
			so  = so->getReferenceNode();
		}
		return res;
	}

	/**
	* give a global position, return the local position in this obj space
	*/
	Vec3f SceneNode::computeLocalPosition(const Vec3f& globalposition) const
	{
		const SceneNode* so = getReferenceNode();
		if (so)
			return localCoordinatesOfReference(so->computeLocalPosition(globalposition));
		else
			return localCoordinatesOfReference(globalposition);
	}

	/**
	* give referenced globalpositionm, return reference local position
	*/
	Vec3f SceneNode::localCoordinatesOfReference(const Vec3f& globalposition) const
	{
		Vec3f local = getRotation().inverse() * (globalposition - getTranslation());
		local.scale(1.0/_scale.x(), 1.0/_scale.y(), 1.0/_scale.z());
		return local;
	}

	/**
	* give a local position, return the global position
	*/
	Vec3f SceneNode::computeGlobalPosition(const Vec3f& localposition) const
	{
		const SceneNode* so = this;
	    Vec3f res = localposition;
	    while (so != NULL)
		{
			res = so->coordinatesOfReference(res);
			so  = so->getReferenceNode();
		}
	    return res;
	}

	/**
	* give a local position, return the global-reference position in this reference obj space
	*/
	Vec3f SceneNode::coordinatesOfReference(const Vec3f& localPosition) const
	{
		Vec3f pos = localPosition;
		pos.scale(_scale.x(), _scale.y(), _scale.z());
		return this->getRotation() * pos  + this->getTranslation();
	}

	void SceneNode::attachMovableObject(MovableObject* obj)
	{
		_movableObjects.push_back(obj); 
		obj->setReferenceNode(this);
	}
}