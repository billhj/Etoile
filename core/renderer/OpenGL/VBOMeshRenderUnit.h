/**
* Copyright(C) 2009-2012                
* @author Jing HUANG
* @file VBOMeshRenderUnit.h
* @brief 
* @date 1/2/2011
*/

#pragma once
#include "GLMeshRenderUnit.h"
#include "VBO.h"
#include "IndexVBO.h"

namespace Etoile
{
	struct SubMeshVBOUnit
	{
		VBOUnit _vertexVBO;
		VBOUnit _normalVBO;
		VBOUnit _texCoordVBO;
		IndexVBO* _indexVBO;
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

	class VBOMeshRenderUnit : public GLMeshRenderUnit
	{
	public:
		VBOMeshRenderUnit(const std::string& name);
		~VBOMeshRenderUnit();
		virtual void draw() override;
		virtual void drawMesh() override;
		virtual void drawSubMesh(SubMesh* submesh, int idx);
		virtual void setMesh(Mesh* mesh) override;
		virtual void updateVBO(Mesh* mesh);
		virtual void updateVBO(SubMesh* submesh, int idx);
	protected:
		int getVBOUnitIndexByName(const std::string& name);
		virtual void createVBO(Mesh* mesh, GLenum usage = GL_STATIC_DRAW_ARB);
		std::vector<SubMeshVBOUnit*> _vboUnitList;
		std::map<std::string, int> _vboUnitIndex;
	};


	class GPUBasedVBOMeshRenderUnit : public VBOMeshRenderUnit
	{
	public:
		GPUBasedVBOMeshRenderUnit(const std::string& name);
		virtual void drawSubMesh(SubMesh* submesh, int idx);
	};

	class AvancedGPUBasedVBOMeshRenderUnit : public GPUBasedVBOMeshRenderUnit
	{
	public:
		AvancedGPUBasedVBOMeshRenderUnit(const std::string& name);
		virtual void drawSubMesh(SubMesh* submesh, int idx);
	};
}
