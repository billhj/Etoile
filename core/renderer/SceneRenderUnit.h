/**
* Copyright(C) 2009-2012                
* @author Jing HUANG
* @file SceneRenderUnit.h
* @brief 
* @date 1/2/2011
*/

#pragma once
#include "../RenderUnit.h"
#include "geometry/Scene.h"

namespace Etoile
{
	class SceneRenderUnit : public RenderUnit
	{
	protected:
		Scene* _pscene;
	public:
		SceneRenderUnit(const std::string& name): RenderUnit(name), _pscene(NULL)
		{
		}
		void setScene(Scene* node){ _pscene = node; }
		Scene* getScene(){return _pscene;}
	};
}
