/**
* Copyright(C) 2009-2012                
* @author Jing HUANG
* @file MeshRenderUnit.h
* @brief 
* @date 1/2/2011
*/

#pragma once
#include "RenderUnit.h"
#include "geometry/Mesh.h"

namespace Etoile
{
	class MeshRenderUnit : public RenderUnit
	{
	public:
		MeshRenderUnit(const std::string& name): RenderUnit(name), _pMesh(NULL)
		{
		}
		virtual void setMesh(Mesh* mesh)
		{
			_pMesh = mesh;
		}
		virtual Mesh* getMesh(){return _pMesh;}
		
	protected:
		Mesh* _pMesh;
	};

}
