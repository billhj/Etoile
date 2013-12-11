/**
* Copyright(C) 2009-2012                
* @author Jing HUANG
* @file MeshRenderPass.h
* @brief 
* @date 1/2/2011
*/

#pragma once
#include "glhead.h"
#include "GLMeshRenderUnit.h"

namespace Etoile
{
	class ImmediateMeshRenderUnit : public GLMeshRenderUnit
	{
	public:
		ImmediateMeshRenderUnit(const std::string& name);
		virtual void draw() override;
	};


	class DisplayListMeshRenderUnit : public GLMeshRenderUnit
	{
	public:
		DisplayListMeshRenderUnit(const std::string& name);
		~DisplayListMeshRenderUnit(){  if(glIsList(_calllist)){glDeleteLists(_calllist, 1);} }
		virtual void draw() override;
		virtual void setMesh(Mesh* mesh);
	protected:
		void generateCallList();
		GLuint _calllist;
	};

}
