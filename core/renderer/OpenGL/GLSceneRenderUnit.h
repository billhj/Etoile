/**
* Copyright(C) 2009-2012                
* @author Jing HUANG
* @file GLSceneRenderUnit.h
* @brief 
* @date 1/2/2011
*/

#pragma once
#include "../SceneRenderUnit.h"

namespace Etoile
{
	class GLSceneRenderUnit : public SceneRenderUnit
	{
	public:
		GLSceneRenderUnit(const std::string& name): SceneRenderUnit(name)
		{
		}
		virtual void draw() override;
		
	};
}
