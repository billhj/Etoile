/**
* Copyright(C) 2009-2012                
* @author Jing HUANG
* @file ImmediateRenderMesh.cpp
* @brief 
* @date 1/2/2011
*/

#include "ImmediateRenderMesh.h"

namespace Etoile
{
	ImmediateRenderMesh::ImmediateRenderMesh(const std::string& name) : GLRenderMesh(name)
	{
	}

	DisplayListRenderMesh::DisplayListRenderMesh(const std::string& name) : GLRenderMesh(name)
	{
		generateCallList();
	}


	void DisplayListRenderMesh::drawMesh(Matrix4f& gltransformation)
	{
		glPushMatrix();
		glLoadMatrixf(&gltransformation[0][0]);
		glCallList(_calllist);
		glPopMatrix();
	}

	void DisplayListRenderMesh::generateCallList()
	{
		_calllist = glGenLists(1);
		glNewList(_calllist, GL_COMPILE);
		GLRenderMesh::drawMesh(Matrix4f());
		glEndList();
	}

}
