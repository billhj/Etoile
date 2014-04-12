/**
* Copyright(C) 2009-2012                
* @author Jing HUANG
* @file RenderMesh.h
* @brief 
* @date 1/2/2011
*/

#pragma once
#include "geometry/Mesh.h"

namespace Etoile
{
	class RenderMesh : public Mesh
	{
	public:
		RenderMesh(const std::string& name): Mesh(name)
		{
		}
		virtual void draw(Matrix4f& transform) = 0;
		virtual void perform(Matrix4f& transform)
		{
			draw(transform);
		}
	};

}
