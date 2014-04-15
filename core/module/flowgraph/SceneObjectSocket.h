/**
* Copyright(C) 2009-2012                
* @author Jing HUANG
* @file SceneObjectSocket.h
* @brief 
* @date 1/2/2011
*/

#pragma once
#include "TemplateSocket.h"
#include "geometry/SceneObject.h"
#include "geometry/Primitive.h"

namespace Etoile
{
	class SceneObjectInputSocket : public TemplateInputSocket<SceneObject>
	{
	public:
		SceneObjectInputSocket(const std::string& name): TemplateInputSocket<SceneObject>(INPUT_MUL)
		{
			initType("SOCK_SCENEOBJECT");
			this->setName(name);
		}
	};


	class SceneObjectOutputSocket : public TemplateOutputSocket<SceneObject>
	{
	public:
		SceneObjectOutputSocket(const std::string& name)
		{
			initType("SOCK_SCENEOBJECT");
			this->setName(name);
		}
	};

	class PlaneInputSocket : public TemplateInputSocket<Plane>
	{
	public:
		PlaneInputSocket(const std::string& name): TemplateInputSocket<Plane>()
		{
			initType("SOCK_PLANE");
			this->setName(name);
		}
	};


	class PlaneOutputSocket : public TemplateOutputSocket<Plane>
	{
	public:
		PlaneOutputSocket(const std::string& name)
		{
			initType("SOCK_PLANE");
			this->setName(name);
		}
	};

}