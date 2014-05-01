/**
* Copyright(C) 2009-2012                
* @author Jing HUANG
* @file VBORenderMesh.h
* @brief 
* @date 1/2/2011
*/

#pragma once
#include "GLRenderMesh.h"
#include "VBO.h"
#include "IBO.h"

namespace Etoile
{
	struct SubMeshVBOUnit
	{
		VBOUnit<Vec3f> _vertexVBO;
		VBOUnit<Vec3f> _normalVBO;
		VBOUnit<Vec2f> _texCoordVBO;
		VBOUnit<Vec4i> _bonesIndicesVBO;
		VBOUnit<Vec4f> _bonesWeightsVBO;
		IBO* _indexVBO;
		SubMeshVBOUnit()
		{
			_vertexVBO._pVBO = NULL;
			_normalVBO._pVBO = NULL;
			_texCoordVBO._pVBO = NULL;
			_indexVBO = NULL;
		}
		~SubMeshVBOUnit()
		{
			if(_indexVBO != NULL)
				delete _indexVBO;
		}
	};

	class VBORenderMesh : public GLRenderMesh
	{
	public:
		VBORenderMesh(const std::string& name);
		~VBORenderMesh();
		virtual void initResource();
		virtual void drawMesh(Matrix4f& gltransformation) override;
		virtual void drawSubMesh(SubMesh* submesh, int idx, Matrix4f& gltransformation);
		virtual void updateVBO(Mesh* mesh);
		virtual void updateVBO(SubMesh* submesh, int idx);
	protected:
		int getVBOUnitIndexByName(const std::string& name);
		virtual void createVBO(Mesh* mesh, GLenum usage = GL_STATIC_DRAW_ARB);
		std::vector<SubMeshVBOUnit*> _vboUnitList;
		std::map<std::string, int> _vboUnitIndex;
	};


	class GPUBasedVBORenderMesh : public VBORenderMesh
	{
	public:
		GPUBasedVBORenderMesh(const std::string& name);
		virtual void drawSubMesh(SubMesh* submesh, int idx, Matrix4f& gltransformation);
	};

	class AvancedGPUBasedVBORenderMesh : public GPUBasedVBORenderMesh
	{
	public:
		AvancedGPUBasedVBORenderMesh(const std::string& name);
		virtual void drawSubMesh(SubMesh* submesh, int idx, Matrix4f& gltransformation);
	};
}
