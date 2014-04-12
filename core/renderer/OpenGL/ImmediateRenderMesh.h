/**
* Copyright(C) 2009-2012                
* @author Jing HUANG
* @file ImmediateRenderMesh.h
* @brief 
* @date 1/2/2011
*/

#pragma once
#include "glhead.h"
#include "GLRenderMesh.h"

namespace Etoile
{
	class ImmediateRenderMesh : public GLRenderMesh
	{
	public:
		ImmediateRenderMesh(const std::string& name);
	};


	class DisplayListRenderMesh : public GLRenderMesh
	{
	public:
		DisplayListRenderMesh(const std::string& name);
		~DisplayListRenderMesh(){  if(glIsList(_calllist)){glDeleteLists(_calllist, 1);} }
		virtual void drawMesh(Matrix4f& gltransformation) override;
	protected:
		void generateCallList();
		GLuint _calllist;
	};

}
