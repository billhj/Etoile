/**
* Copyright(C) 2009-2012                
* @author Jing HUANG
* @file MovableObject.h
* @brief 
* @date 1/2/2011
*/

#pragma once
#include <sstream>
#include "math/MathHead.h"
#include "AxisAlignedBoundingBox.h"
namespace Etoile
{
	class SceneNode;
	class MovableObject 
	{
	public:
		MovableObject():_visible(true){generateID();}
		MovableObject(const MovableObject& obj){(*this) = obj;}
		
		void setReferenceNode(SceneNode* obj){_pRef = obj;}
		SceneNode* getReferenceNode() const {return _pRef;}

		AxisAlignedBoundingBoxf* getAABB(){return &_aabb;}
		virtual void computerAABB() = 0;
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
		void setVisible(bool visible)
		{
			_visible = visible;
		}
		bool isVisible(){return _visible;}

		
		virtual void perform(Matrix4f& gltransformation) = 0;
	protected:
		AxisAlignedBoundingBoxf _aabb;
		SceneNode* _pRef;
		long _id;
		bool _visible;
	};

}