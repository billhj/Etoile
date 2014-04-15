/**
* Copyright(C) 2009-2012                
* @author Jing HUANG
* @file DerivedMeshRenderPassPlugin.h
* @brief 
* @date 1/2/2011
*/

#pragma once
#include "EtoileGLMeshRenderPassPlugin.h"

Etoile::EPlugin* loadDrawMeshRenderPassPlugin();

namespace Etoile
{

	class DrawMeshRenderPassPlugin : public EtoileGLMeshRenderPassPlugin 
	{
	public:
		DrawMeshRenderPassPlugin(const std::string& name);
		virtual void init() override;
	};
}

