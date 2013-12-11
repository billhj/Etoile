/**
* Copyright(C) 2009-2012                
* @author Jing HUANG
* @file VBOMeshRenderUnit.cpp
* @brief 
* @date 1/2/2011
*/

#include "VBOMeshRenderUnit.h"
#include "GLSLGpuProgram.h"

namespace Etoile
{
	VBOMeshRenderUnit::VBOMeshRenderUnit(const std::string& name) : GLMeshRenderUnit(name)
	{
	}

	VBOMeshRenderUnit::~VBOMeshRenderUnit()
	{

		for(unsigned int i = 0; i < _vboUnitList.size(); ++i)
		{
			delete _vboUnitList[i];
		}
		_vboUnitList.clear();
		_vboUnitIndex.clear();
	}

	void VBOMeshRenderUnit::draw()
	{

		drawMesh();
	}


	void VBOMeshRenderUnit::drawMesh()
	{
		if(_pMesh == NULL) return;
		
		const std::vector<SubMesh*>& submeshlist = _pMesh->getSubMeshList();
		for(unsigned int i = 0; i < submeshlist.size(); ++i)
		{
			SubMesh* submesh = submeshlist[i];
			if(submesh->getSkin()._updated)
			{
				updateVBO(submesh, i);
			}
			drawSubMesh(submesh, i);
		}
		drawAABB();
	}

	void VBOMeshRenderUnit::drawSubMesh(SubMesh* submesh, int idx)
	{
		glEnable( GL_TEXTURE_2D );

		Material* material = submesh->getMaterial();
		if(material != NULL){
			applyMaterial(material);

			Matrix4f modelM = submesh->getGLModelMatrix() * _pMesh->getGLModelMatrix();
			glPushMatrix();
			glLoadMatrixf(&modelM[0][0]);

			Texture* t = material->getDiffuseTexture();
			if(t != NULL)
			{
				t->use();
			}

			SubMeshVBOUnit* info = _vboUnitList[idx];

			info->_texCoordVBO._pVBO->use();
			glTexCoordPointer(info->_texCoordVBO._numberComponents, GL_FLOAT, 0, 0);
			info->_normalVBO._pVBO->use();
			glNormalPointer(GL_FLOAT, 0, 0);
			info->_vertexVBO._pVBO->use();
			glVertexPointer(info->_vertexVBO._numberComponents, GL_FLOAT, 0, 0);

			printOpenGLError();

			glEnableClientState(GL_TEXTURE_COORD_ARRAY);
			glEnableClientState(GL_NORMAL_ARRAY);
			glEnableClientState(GL_VERTEX_ARRAY);

			printOpenGLError();

			info->_indexVBO->use();
			glDrawElements( GL_TRIANGLES, info->_indexVBO->getSize(), GL_UNSIGNED_INT, 0 );
			info->_indexVBO->unUse();

			printOpenGLError();

			glDisableClientState(GL_TEXTURE_COORD_ARRAY);
			glDisableClientState(GL_NORMAL_ARRAY);
			glDisableClientState(GL_VERTEX_ARRAY);


			info->_texCoordVBO._pVBO->unUse();
			info->_normalVBO._pVBO->unUse();
			info->_vertexVBO._pVBO->unUse();

			printOpenGLError();

			if(t != NULL)
			{
				t->unUse();
			}
			glPopMatrix();
		}
		else
		{
			assert(0 && "VBOMeshRenderUnit: no material");
		}

	}


	void VBOMeshRenderUnit::setMesh(Mesh* mesh)
	{
		_vboUnitList.clear();
		_pMesh = mesh;
		if(_pMesh != NULL)
			createVBO(_pMesh);

	}

	void VBOMeshRenderUnit::createVBO(Mesh* mesh, GLenum usage)
	{
		const std::vector<SubMesh*>& submeshlist = mesh->getSubMeshList();

		for(unsigned int i = 0; i < submeshlist.size(); ++i)
		{
			SubMesh* submesh = submeshlist[i];

			SubMeshVBOUnit* info = new SubMeshVBOUnit();
			submesh->getSkin()._updated = false;
			size_t sizeComponent = submesh->getOriginalVertices().size();
			size_t sizeTextureCord = submesh->getOriginalTextureCoords().size();
	
			VBO* normalVBO = new VBO(sizeComponent * 3, &(submesh->getSkin()._ndata[0][0]), usage);
			info->_normalVBO._pVBO = normalVBO;
			info->_normalVBO._attributeName = "In_Normal";
			info->_normalVBO._numberComponents = 3;
			info->_normalVBO._primitive = GL_TRIANGLES;

			VBO* texCoordVBO = new VBO(sizeTextureCord * 2, &(submesh->getSkin()._tdata[0][0]), usage);
			info->_texCoordVBO._pVBO = texCoordVBO;
			info->_texCoordVBO._attributeName = "In_TextureCoord";
			info->_texCoordVBO._numberComponents = 2;
			info->_texCoordVBO._primitive = GL_TRIANGLES;

			VBO* vertexVBO = new VBO(sizeComponent * 3, &(submesh->getSkin()._vdata[0][0]), usage);
			info->_vertexVBO._pVBO = vertexVBO;
			info->_vertexVBO._attributeName = "In_Vertex";
			info->_vertexVBO._numberComponents = 3;
			info->_vertexVBO._primitive = GL_TRIANGLES;


			IndexVBO* _indexVBO = new IndexVBO(submesh->getOriginalVertexIndexForFaces().size(), &(submesh->getOriginalVertexIndexForFaces()[0]), usage);
			info->_indexVBO = _indexVBO;
			_vboUnitList.push_back(info);

		}
	}

	void VBOMeshRenderUnit::updateVBO(Mesh* mesh)
	{
		const std::vector<SubMesh*>& submeshlist = mesh->getSubMeshList();

		for(unsigned int i = 0; i < submeshlist.size(); ++i)
		{
			SubMesh* submesh = submeshlist[i];
			updateVBO(submesh, i);
		}
	}

	void VBOMeshRenderUnit::updateVBO(SubMesh* submesh, int i)
	{
		if(submesh->getSkin()._updated)
		{
			SubMeshVBOUnit* info = _vboUnitList[i];
			int size = submesh->getSkin()._vdata.size() * 3;
			info->_vertexVBO._pVBO->write(0, size, &submesh->getSkin()._vdata[0][0]);
			info->_normalVBO._pVBO->write(0, size, &submesh->getSkin()._ndata[0][0]);
			submesh->getSkin()._updated = false;
		}
	}

	int VBOMeshRenderUnit::getVBOUnitIndexByName(const std::string& name)
	{
		std::map<std::string, int>::iterator itor = _vboUnitIndex.find(name);
		if(itor != _vboUnitIndex.end())
		{
			return itor->second;
		}
		return -1;
	}

	GPUBasedVBOMeshRenderUnit::GPUBasedVBOMeshRenderUnit(const std::string& name) : VBOMeshRenderUnit(name)
	{}


	void GPUBasedVBOMeshRenderUnit::drawSubMesh(SubMesh* submesh, int idx)
	{

		glEnable( GL_TEXTURE_2D );

		Material* material = submesh->getMaterial();
		if(material != NULL)
		{
			applyMaterial(material);
			GLSLGpuProgram* gpuprogram = (GLSLGpuProgram*)material->getGpuProgram();

			Matrix4f modelM = submesh->getGLModelMatrix() * _pMesh->getGLModelMatrix();
			if(gpuprogram != NULL)
			{
				gpuprogram->setUniformVariable("In_WorldMatrix",  modelM);
				std::map<std::string, Texture*>& idxs = material->getTextures();
				std::map<std::string, Texture*>::iterator itor;

				for(itor = idxs.begin(); itor != idxs.end(); ++itor)
				{
					std::string bName = itor->first;
					Texture* t = itor->second;
					gpuprogram->bindTexture(bName, t);
				}
				SubMeshVBOUnit* info = _vboUnitList[idx];
				gpuprogram->drawIndexVBO(GL_TRIANGLES, info->_vertexVBO, info->_normalVBO, info->_texCoordVBO, info->_indexVBO);
				gpuprogram->unBindBindingTextures();
			}
			else
			{
				glPushMatrix();
				glLoadMatrixf(&modelM[0][0]);
				Texture* t = material->getDiffuseTexture();
				if(t != NULL)
				{
					t->use();
				}
				SubMeshVBOUnit* info = _vboUnitList[idx];

				info->_texCoordVBO._pVBO->use();
				glTexCoordPointer(info->_texCoordVBO._numberComponents, GL_FLOAT, 0, 0);
				info->_normalVBO._pVBO->use();
				glNormalPointer(GL_FLOAT, 0, 0);
				info->_vertexVBO._pVBO->use();
				glVertexPointer(info->_vertexVBO._numberComponents, GL_FLOAT, 0, 0);

				printOpenGLError();

				glEnableClientState(GL_TEXTURE_COORD_ARRAY);
				glEnableClientState(GL_NORMAL_ARRAY);
				glEnableClientState(GL_VERTEX_ARRAY);

				printOpenGLError();

				info->_indexVBO->use();
				glDrawElements( GL_TRIANGLES, info->_indexVBO->getSize(), GL_UNSIGNED_INT, 0 );
				info->_indexVBO->unUse();

				printOpenGLError();

				glDisableClientState(GL_TEXTURE_COORD_ARRAY);
				glDisableClientState(GL_NORMAL_ARRAY);
				glDisableClientState(GL_VERTEX_ARRAY);

				info->_texCoordVBO._pVBO->unUse();
				info->_normalVBO._pVBO->unUse();
				info->_vertexVBO._pVBO->unUse();

				printOpenGLError();
				if(t != NULL)
				{
					t->unUse();
				}
				glPushMatrix();
			}

		}
		else
		{
			assert(0 && "GPUBasedVBOMeshRenderUnit: no material");
		}

	}


	AvancedGPUBasedVBOMeshRenderUnit::AvancedGPUBasedVBOMeshRenderUnit(const std::string& name) : GPUBasedVBOMeshRenderUnit(name)
	{}


	void AvancedGPUBasedVBOMeshRenderUnit::drawSubMesh(SubMesh* submesh, int idx)
	{
		glEnable( GL_TEXTURE_2D );

		Material* material = submesh->getMaterial();
		if(material != NULL)
		{
			applyMaterial(material);
			GLSLGpuProgram* gpuprogram = (GLSLGpuProgram*)material->getGpuProgram();

			if(gpuprogram != NULL)
			{
				gpuprogram->setUniformVariable("In_WorldMatrix", submesh->getGLModelMatrix() * _pMesh->getGLModelMatrix());
				std::map<std::string, Texture*>& idxs = material->getTextures();
				std::map<std::string, Texture*>::iterator itor;

				for(itor = idxs.begin(); itor != idxs.end(); ++itor)
				{
					std::string bName = itor->first;
					Texture* t = itor->second;
					gpuprogram->bindTexture(bName, t);
				}

				SubMeshVBOUnit* info = _vboUnitList[idx];
				gpuprogram->drawIndexVBO(GL_TRIANGLES, info->_vertexVBO, info->_normalVBO, info->_texCoordVBO, info->_indexVBO);
				printOpenGLError();
				gpuprogram->unBindBindingTextures();
			}else
			{
				std::cout<< "AvancedVBOMeshRenderUnit gpuprogram is not available : "<< gpuprogram->getName()<<std::endl;
			}
		}
		else
		{
			assert(0 && "AvancedVBOMeshRenderUnit: no material");
		}

	}

}
