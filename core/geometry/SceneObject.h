/**
* Copyright(C) 2009-2012                
* @author Jing HUANG
* @file SceneObject.h
* @brief 
* @date 1/2/2011
*/

#pragma once
#include "math/MathHead.h"
#include "AxisAlignedBoundingBox.h"

namespace Etoile
{
	class SceneObject 
	{
	public:
		SceneObject();
		SceneObject(const SceneObject& obj);
		SceneObject(const Vec3f& translate, const Quaternionf& rotation, const Vec3f& scale = Vec3f(1,1,1), SceneObject* ref = NULL);
		void set(const Vec3f& translate, const Quaternionf& rotation, const Vec3f& scale = Vec3f(1,1,1), SceneObject* ref = NULL);
		SceneObject& operator=(const SceneObject& obj);
		void setReferenceObject(SceneObject* obj);
		SceneObject* getReferenceObject() const {return _pRef;}

		void setScale(const Vec3f& scale);
		Vec3f getScale() const{return _scale;}
		void setTranslation(const Vec3f& translation);
		Vec3f getTranslation() const {return _translation;}
		void setRotation(const Quaternionf& rotation);
		Quaternionf getRotation() const {return _rotation;}
		Matrix4f getModelMatrix();
		Matrix4f getGLModelMatrix();
		void setTransform(const Vec3f& scale, const Quaternionf& rotate, const Vec3f& translate)
		{
			_scale = scale, _rotation = rotate, _translation = translate;
			_rotation.normalize();
			_modelTransform = Matrix4f().makeTranslation(_translation) * Matrix4f().makeAxisRotation(_rotation.axis(), _rotation.angle()) * Matrix4f().makeScale(_scale);
		}

		void setTransform()
		{
			setTransform(_scale, _rotation, _translation);
		}


		void setPosition(const Vec3f& position);
		Vec3f getPosition() const;
		void setOrientation(const Quaternionf& orientation);
		Quaternionf getOrientation() const;


		Vec3f computeLocalPosition(const Vec3f& globalposition) const; 
		Vec3f localCoordinatesOfReference(const Vec3f& globalposition) const;
		Vec3f computeGlobalPosition(const Vec3f& localposition) const;
		Vec3f coordinatesOfReference(const Vec3f& local) const;

		AxisAlignedBoundingBoxf* getAABB(){return &_aabb;}
	protected:
		Vec3f _scale;
		Vec3f _translation;  //local
		Quaternionf _rotation;  //local
		Matrix4f _modelTransform;
		AxisAlignedBoundingBoxf _aabb;
		SceneObject* _pRef;
	};

}