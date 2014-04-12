/**
* Copyright(C) 2009-2012                
* @author Jing HUANG
* @file SceneNode.h
* @brief 
* @date 1/2/2011
*/

#pragma once
#include <sstream>
#include "math/MathHead.h"

namespace Etoile
{
	class MovableObject;
	class SceneNode
	{
	public:
		SceneNode();
		SceneNode(const SceneNode& obj);
		SceneNode(const Vec3f& translate, const Quaternionf& rotation, const Vec3f& scale = Vec3f(1,1,1), SceneNode* ref = NULL);
		void set(const Vec3f& translate, const Quaternionf& rotation, const Vec3f& scale = Vec3f(1,1,1), SceneNode* ref = NULL);
		SceneNode& operator=(const SceneNode& obj);
		void setReferenceNode(SceneNode* obj);
		SceneNode* getReferenceNode() const {return _pRef;}

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
			if(_pRef == NULL)
				_modelTransform = Matrix4f().makeTranslation(_translation) * Matrix4f().makeAxisRotation(_rotation.axis(), _rotation.angle()) * Matrix4f().makeScale(_scale);
			else
				_modelTransform = _pRef->getModelMatrix() * Matrix4f().makeTranslation(_translation) * Matrix4f().makeAxisRotation(_rotation.axis(), _rotation.angle()) * Matrix4f().makeScale(_scale);
		}

		void updateTransform()
		{
			setTransform(_scale, _rotation, _translation);
		}

		void multiply(Matrix4f& transform)
		{
			_modelTransform = _modelTransform * transform;
		}


		void setPosition(const Vec3f& position);
		Vec3f getPosition() const;
		void setOrientation(const Quaternionf& orientation);
		Quaternionf getOrientation() const;


		Vec3f computeLocalPosition(const Vec3f& globalposition) const; 
		Vec3f localCoordinatesOfReference(const Vec3f& globalposition) const;
		Vec3f computeGlobalPosition(const Vec3f& localposition) const;
		Vec3f coordinatesOfReference(const Vec3f& local) const;

		//AxisAlignedBoundingBoxf* getAABB(){return &_aabb;}

		void addChildNode(SceneNode* node){_children.push_back(node); node->setReferenceNode(this);}
		std::vector<SceneNode*>& getChildrenNodes(){return _children;}
		void attachMovableObject(MovableObject* obj);
		std::vector<MovableObject*>& getMovableObjects(){return _movableObjects;}
		void setID(long id)
		{
			_id = id;
		}

		long getID(){return _id;}
		std::string getStringID()
		{
			std::string number;
			std::stringstream strstream;
			strstream << _id;
			strstream >> number;
			return number;
		}
		void generateID()
		{
			_id = rand();
		}

	protected:
		Vec3f _scale;
		Vec3f _translation;  //local
		Quaternionf _rotation;  //local
		Matrix4f _modelTransform;
		//AxisAlignedBoundingBoxf _aabb;
		SceneNode* _pRef;
		std::vector<SceneNode*> _children;
		std::vector<MovableObject*> _movableObjects;
		long _id;
	};

}