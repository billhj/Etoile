/**
* Copyright(C) 2009-2012                
* @author Jing HUANG
* @file SceneNodeSocket.h
* @brief 
* @date 1/2/2011
*/

#pragma once
#include "TemplateSocket.h"
#include "geometry/Scene.h"
#include "geometry/Primitive.h"

namespace Etoile
{

	class SceneInputSocket : public TemplateInputSocket<Scene>
	{
	public:
		SceneInputSocket(const std::string& name): TemplateInputSocket<Scene>(INPUT_MUL)
		{
			initType("SOCK_SCENE");
			this->setName(name);
		}
	};


	class SceneOutputSocket : public TemplateOutputSocket<Scene>
	{
	public:
		SceneOutputSocket(const std::string& name)
		{
			initType("SOCK_SCENE");
			this->setName(name);
		}
	};

	class SceneNodeInputSocket : public TemplateInputSocket<SceneNode>
	{
	public:
		SceneNodeInputSocket(const std::string& name): TemplateInputSocket<SceneNode>(INPUT_MUL)
		{
			initType("SOCK_SceneNode");
			this->setName(name);
		}
	};


	class SceneNodeOutputSocket : public TemplateOutputSocket<SceneNode>
	{
	public:
		SceneNodeOutputSocket(const std::string& name)
		{
			initType("SOCK_SceneNode");
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