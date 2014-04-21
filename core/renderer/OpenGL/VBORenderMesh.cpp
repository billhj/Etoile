/**
* Copyright(C) 2009-2012                
* @author Jing HUANG
* @file VBORenderMesh.cpp
* @brief 
* @date 1/2/2011
*/

#include "VBORenderMesh.h"
#include "GLSLGpuProgram.h"

namespace Etoile
{
	VBORenderMesh::VBORenderMesh(const std::string& name) : GLRenderMesh(name)
	{
		
	}

	
	void VBORenderMesh::initResource()
	{
		GLRenderMesh::initResource();
		_vboUnitList.clear();
		createVBO(this);
	}

	VBORenderMesh::~VBORenderMesh()
	{

		for(unsigned int i = 0; i < _vboUnitList.size(); ++i)
		{
			delete _vboUnitList[i];
		}
		_vboUnitList.clear();
		_vboUnitIndex.clear();
	}

	void VBORenderMesh::drawMesh(Matrix4f& gltransformation)
	{	
		const std::vector<SubMesh*>& submeshlist = this->getSubMeshList();
		for(unsigned int i = 0; i < submeshlist.size(); ++i)
		{
			SubMesh* submesh = submeshlist[i];
			if(submesh->getSkin()._updated)
			{
				updateVBO(submesh, i);
			}
			drawSubMesh(submesh, i, gltransformation);
		}
		drawAABB(gltransformation);
	}

	void VBORenderMesh::drawSubMesh(SubMesh* submesh, int idx, Matrix4f& gltransformation)
	{
		glEnable( GL_TEXTURE_2D );

		Material* material = submesh->getMaterial();
		if(material != NULL){
			applyMaterial(material);
			glPushMatrix();
			glLoadMatrixf(&gltransformation[0][0]);

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
			glDrawElements( GL_TRIANGLES, info->_indexVBO->dataSize(), GL_UNSIGNED_INT, 0 );
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
			assert(0 && "VBORenderMesh: no material");
		}

	}


	void VBORenderMesh::createVBO(Mesh* mesh, GLenum usage)
	{
		const std::vector<SubMesh*>& submeshlist = mesh->getSubMeshList();

		for(unsigned int i = 0; i < submeshlist.size(); ++i)
		{
			SubMesh* submesh = submeshlist[i];

			SubMeshVBOUnit* info = new SubMeshVBOUnit();
			submesh->getSkin()._updated = false;
			size_t sizeComponent = submesh->getOriginalVertices().size();
			size_t sizeTextureCord = submesh->getOriginalTextureCoords().size();
	
			VBOFloat* normalVBO = new VBOFloat(sizeComponent * 3, &(submesh->getSkin()._ndata[0][0]));
			info->_normalVBO._pVBO = normalVBO;
			info->_normalVBO._attributeName = "In_Normal";
			info->_normalVBO._numberComponents = 3;
			info->_normalVBO._primitive = GL_TRIANGLES;

			VBOFloat* texCoordVBO = new VBOFloat(sizeTextureCord * 2, &(submesh->getSkin()._tdata[0][0]));
			info->_texCoordVBO._pVBO = texCoordVBO;
			info->_texCoordVBO._attributeName = "In_TextureCoord";
			info->_texCoordVBO._numberComponents = 2;
			info->_texCoordVBO._primitive = GL_TRIANGLES;

			VBOFloat* vertexVBO = new VBOFloat(sizeComponent * 3, &(submesh->getSkin()._vdata[0][0]));
			info->_vertexVBO._pVBO = vertexVBO;
			info->_vertexVBO._attributeName = "In_Vertex";
			info->_vertexVBO._numberComponents = 3;
			info->_vertexVBO._primitive = GL_TRIANGLES;


			IndexVBO* _indexVBO = new IndexVBO(submesh->getOriginalVertexIndexForFaces().size(), &(submesh->getOriginalVertexIndexForFaces()[0]));
			info->_indexVBO = _indexVBO;
			_vboUnitList.push_back(info);

		}
	}

	void VBORenderMesh::updateVBO(Mesh* mesh)
	{
		const std::vector<SubMesh*>& submeshlist = mesh->getSubMeshList();

		for(unsigned int i = 0; i < submeshlist.size(); ++i)
		{
			SubMesh* submesh = submeshlist[i];
			updateVBO(submesh, i);
		}
	}

	void VBORenderMesh::updateVBO(SubMesh* submesh, int i)
	{
		if(submesh->getSkin()._updated)
		{
			SubMeshVBOUnit* info = _vboUnitList[i];
			int size = submesh->getSkin()._vdata.size() * 3;
			info->_vertexVBO._pVBO->bindData(size, &submesh->getSkin()._vdata[0][0]);
			info->_normalVBO._pVBO->bindData(size, &submesh->getSkin()._ndata[0][0]);
			submesh->getSkin()._updated = false;
		}
	}

	int VBORenderMesh::getVBOUnitIndexByName(const std::string& name)
	{
		std::map<std::string, int>::iterator itor = _vboUnitIndex.find(name);
		if(itor != _vboUnitIndex.end())
		{
			return itor->second;
		}
		return -1;
	}

	GPUBasedVBORenderMesh::GPUBasedVBORenderMesh(const std::string& name) : VBORenderMesh(name)
	{}


	void GPUBasedVBORenderMesh::drawSubMesh(SubMesh* submesh, int idx, Matrix4f& gltransformation)
	{

		glEnable( GL_TEXTURE_2D );

		Material* material = submesh->getMaterial();
		if(material != NULL)
		{
			applyMaterial(material);
			GLSLGpuProgram* gpuprogram = (GLSLGpuProgram*)material->getGpuProgram();

			if(gpuprogram != NULL)
			{
				gpuprogram->setUniformVariable("In_WorldMatrix",  gltransformation);
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
				glLoadMatrixf(&gltransformation[0][0]);
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
				glDrawElements( GL_TRIANGLES, info->_indexVBO->dataSize(), GL_UNSIGNED_INT, 0 );
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
			assert(0 && "GPUBasedVBORenderMesh: no material");
		}

	}


	AvancedGPUBasedVBORenderMesh::AvancedGPUBasedVBORenderMesh(const std::string& name) : GPUBasedVBORenderMesh(name)
	{}


	void AvancedGPUBasedVBORenderMesh::drawSubMesh(SubMesh* submesh, int idx, Matrix4f& gltransformation)
	{
		glEnable( GL_TEXTURE_2D );

		Material* material = submesh->getMaterial();
		if(material != NULL)
		{
			applyMaterial(material);
			GLSLGpuProgram* gpuprogram = (GLSLGpuProgram*)material->getGpuProgram();

			if(gpuprogram != NULL)
			{
				gpuprogram->setUniformVariable("In_WorldMatrix", gltransformation);
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
				std::cout<< "AvancedVBORenderMesh gpuprogram is not available : "<<std::endl;
			}
		}
		else
		{
			assert(0 && "AvancedVBORenderMesh: no material");
		}

	}

}
