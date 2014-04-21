/**
* Copyright(C) 2009-2012                
* @author Jing HUANG
* @file GLVBOTriMesh.h
* @brief 
* @date 1/2/2011
*/

#pragma once
#ifndef GLVBO_TRI_MESH_H
#define GLVBO_TRI_MESH_H
#include "geometry/TriMesh.h"
#include "VBO.h"
#include "IBO.h"

namespace Etoile
{
	struct GLVBOTriMesh : public TriMesh
	{
	};

	struct DynamicGLVBOTriMesh : public GLVBOTriMesh
	{
	public:
		void initVBO();

		VBOVec3f _vertex_vbo;
		VBOVec3f _normal_vbo;
		VBOVec2f _texcoord_vbo;
		std::vector<IBO> _ibos;
	};
}

#endif //GLVBO_TRI_MESH_H