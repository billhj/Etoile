/**
* Copyright(C) 2009-2012                
* @author Jing HUANG
* @file DerivedSceneRenderPassPlugin.h
* @brief 
* @date 1/2/2011
*/

#pragma once
#include "EtoileGLSceneRenderPassPlugin.h"

Etoile::EPlugin* loadDrawSceneRenderPassPlugin();

namespace Etoile
{

	class DrawSceneRenderPassPlugin : public EtoileGLSceneRenderPassPlugin 
	{
	public:
		DrawSceneRenderPassPlugin(const std::string& name);
		virtual void init() override;
	};
}

