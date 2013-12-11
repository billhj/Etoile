/**
* Copyright(C) 2009-2012                
* @author Jing HUANG
* @file ImmediateMeshRenderUnit.cpp
* @brief 
* @date 1/2/2011
*/

#include "ImmediateMeshRenderUnit.h"

namespace Etoile
{
	ImmediateMeshRenderUnit::ImmediateMeshRenderUnit(const std::string& name) : GLMeshRenderUnit(name)
	{
	}


	void ImmediateMeshRenderUnit::draw()
	{
		drawMesh();
	}

	DisplayListMeshRenderUnit::DisplayListMeshRenderUnit(const std::string& name) : GLMeshRenderUnit(name)
	{
	}


	void DisplayListMeshRenderUnit::draw()
	{
		glCallList(_calllist);
	}


	void DisplayListMeshRenderUnit::setMesh(Mesh* mesh)
	{
		_pMesh = mesh;
		generateCallList();
	}

	void DisplayListMeshRenderUnit::generateCallList()
	{
		_calllist = glGenLists(1);
		glNewList(_calllist, GL_COMPILE);
		drawMesh();
		glEndList();
	}

}
