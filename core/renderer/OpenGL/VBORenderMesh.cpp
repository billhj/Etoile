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
			/*if(submesh->getSkin()._updated)
			{
			updateVBO(submesh, i);
			}*/
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
			//submesh->getSkin()._updated = false;
			size_t sizeComponent = submesh->getVertices().size();
			size_t sizeTextureCord = submesh->getTextureCoords().size();

			VBO<Vec3f>* normalVBO = new VBO<Vec3f>(sizeComponent, &(submesh->getNormals()[0]));
			info->_normalVBO._pVBO = normalVBO;
			info->_normalVBO._attributeName = "In_Normal";
			info->_normalVBO._numberComponents = 3;
			info->_normalVBO._primitive = GL_TRIANGLES;

			VBO<Vec2f>* texCoordVBO = new VBO<Vec2f>(sizeTextureCord, &(submesh->getTextureCoords()[0]));
			info->_texCoordVBO._pVBO = texCoordVBO;
			info->_texCoordVBO._attributeName = "In_TextureCoord";
			info->_texCoordVBO._numberComponents = 2;
			info->_texCoordVBO._primitive = GL_TRIANGLES;

			VBO<Vec3f>* vertexVBO = new VBO<Vec3f>(sizeComponent, &(submesh->getVertices()[0]));
			info->_vertexVBO._pVBO = vertexVBO;
			info->_vertexVBO._attributeName = "In_Vertex";
			info->_vertexVBO._numberComponents = 3;
			info->_vertexVBO._primitive = GL_TRIANGLES;

			if(submesh->isSkeletonSkinActived())
			{
				VBO<Vec4f>* weightVBO = new VBO<Vec4f>(submesh->getBonesWeights().size(), &(submesh->getBonesWeights()[0]));
				info->_bonesWeightsVBO._pVBO = weightVBO;
				info->_bonesWeightsVBO._attributeName = "BonesWeights";
				info->_bonesWeightsVBO._numberComponents = 4;
				info->_bonesWeightsVBO._primitive = GL_TRIANGLES;

				VBO<Vec4i>* indicesVBO = new VBO<Vec4i>(submesh->getBonesIndices().size(), &(submesh->getBonesIndices()[0]));
				info->_bonesIndicesVBO._pVBO = indicesVBO;
				info->_bonesIndicesVBO._attributeName = "BonesIndices";
				info->_bonesIndicesVBO._numberComponents = 4;
				info->_bonesIndicesVBO._primitive = GL_TRIANGLES;
			}

			IBO* _indexVBO = new IBO(submesh->getVertexIndexForFaces().size(), &(submesh->getVertexIndexForFaces()[0]));
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
			//updateVBO(submesh, i);
		}
	}

	void VBORenderMesh::updateVBO(SubMesh* submesh, int i)
	{
		/*if(submesh->getSkin()._updated)
		{
		SubMeshVBOUnit* info = _vboUnitList[i];
		int size = submeshgetVertices().size() * 3;
		info->_vertexVBO._pVBO->bindData(size, &submeshgetVertices()[0][0]);
		info->_normalVBO._pVBO->bindData(size, &submesh->getNormals()[0][0]);
		submesh->getSkin()._updated = false;
		}*/
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
				//gpuprogram->drawIBO(GL_TRIANGLES, info->_vertexVBO, info->_normalVBO, info->_texCoordVBO, info->_indexVBO);
				gpuprogram->use();
				printOpenGLError();
				size_t nComponentPerVertex = 3;
				size_t nTextureCoordComponentPerVertex = 2;

				GLint locationTex = gpuprogram->getAttributLocation(info->_texCoordVBO._attributeName);
				if(locationTex != -1)
				{
					info->_texCoordVBO._pVBO->use();
					glVertexAttribPointer( locationTex, info->_texCoordVBO._numberComponents, GL_FLOAT, GL_FALSE, 0, 0);
					printOpenGLError();
					glEnableVertexAttribArray(locationTex);
				}

				printOpenGLError();

				GLint locationNormal = gpuprogram->getAttributLocation(info->_normalVBO._attributeName);
				if(locationNormal != -1)
				{
					info->_normalVBO._pVBO->use();
					glVertexAttribPointer(locationNormal, info->_normalVBO._numberComponents, GL_FLOAT, GL_FALSE, 0, 0);
					printOpenGLError();
					glEnableVertexAttribArray(locationNormal);
				}

				printOpenGLError();

				GLint locationVertex = gpuprogram->getAttributLocation(info->_vertexVBO._attributeName);
				if(locationVertex != -1)
				{
					info->_vertexVBO._pVBO->use();
					glVertexAttribPointer(locationVertex, info->_vertexVBO._numberComponents, GL_FLOAT, GL_FALSE, 0, 0);
					printOpenGLError();
					glEnableVertexAttribArray(locationVertex);
				}

				
				GLint locationBoneWeights = gpuprogram->getAttributLocation(info->_bonesWeightsVBO._attributeName);
				GLint locationBoneIndices = gpuprogram->getAttributLocation(info->_bonesIndicesVBO._attributeName);
				if(submesh->isSkeletonSkinActived())
				{
					if(locationBoneWeights != -1)
					{
						info->_bonesWeightsVBO._pVBO->use();
						glVertexAttribPointer(locationBoneWeights, info->_bonesWeightsVBO._numberComponents, GL_FLOAT, GL_FALSE, 0, 0);
						printOpenGLError();
						glEnableVertexAttribArray(locationBoneWeights);
					}

					if(locationBoneIndices != -1)
					{
						info->_bonesIndicesVBO._pVBO->use();
						glVertexAttribPointer(locationBoneIndices, info->_bonesIndicesVBO._numberComponents, GL_INT, GL_FALSE, 0, 0);
						printOpenGLError();
						glEnableVertexAttribArray(locationBoneIndices);
					}
				}

				printOpenGLError();

				info->_indexVBO->use();
				if(gpuprogram->isTessellationGpuProgram())
				{
					//TODO : tesselation is difficult
					glPatchParameteri(GL_PATCH_VERTICES, 3);
					glDrawElements( GL_PATCHES, info->_indexVBO->dataSize(), GL_UNSIGNED_INT, 0 );

				}else
				{
					glDrawElements(GL_TRIANGLES, info->_indexVBO->dataSize(), GL_UNSIGNED_INT, 0 );
				}

				printOpenGLError();

				info->_indexVBO->unUse();
				printOpenGLError();



				if(locationTex != -1)
				{
					glDisableVertexAttribArray(locationTex);
					printOpenGLError();
					info->_texCoordVBO._pVBO->unUse();
					printOpenGLError();
				}
				if(locationNormal != -1)
				{
					glDisableVertexAttribArray(locationNormal);
					printOpenGLError();
					info->_normalVBO._pVBO->unUse();
					printOpenGLError();
				}
				if(locationVertex != -1)
				{
					glDisableVertexAttribArray(locationVertex);
					printOpenGLError();
					info->_vertexVBO._pVBO->unUse();
					printOpenGLError();
				}
				if(submesh->isSkeletonSkinActived())
				{
					if(locationBoneWeights != -1)
					{
						glDisableVertexAttribArray(locationBoneWeights);
						printOpenGLError();
						info->_bonesWeightsVBO._pVBO->unUse();
						printOpenGLError();
					}
					if(locationBoneIndices != -1)
					{
						glDisableVertexAttribArray(locationBoneIndices);
						printOpenGLError();
						info->_bonesIndicesVBO._pVBO->unUse();
						printOpenGLError();
					}
				}
				gpuprogram->unUse();


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
				//gpuprogram->drawIBO(GL_TRIANGLES, info->_vertexVBO, info->_normalVBO, info->_texCoordVBO, info->_indexVBO);
				gpuprogram->use();
				printOpenGLError();
				size_t nComponentPerVertex = 3;
				size_t nTextureCoordComponentPerVertex = 2;

				GLint locationTex = gpuprogram->getAttributLocation(info->_texCoordVBO._attributeName);
				if(locationTex != -1)
				{
					info->_texCoordVBO._pVBO->use();
					glVertexAttribPointer( locationTex, info->_texCoordVBO._numberComponents, GL_FLOAT, GL_FALSE, 0, 0);
					printOpenGLError();
					glEnableVertexAttribArray(locationTex);
				}

				printOpenGLError();

				GLint locationNormal = gpuprogram->getAttributLocation(info->_normalVBO._attributeName);
				if(locationNormal != -1)
				{
					info->_normalVBO._pVBO->use();
					glVertexAttribPointer(locationNormal, info->_normalVBO._numberComponents, GL_FLOAT, GL_FALSE, 0, 0);
					printOpenGLError();
					glEnableVertexAttribArray(locationNormal);
				}

				printOpenGLError();

				GLint locationVertex = gpuprogram->getAttributLocation(info->_vertexVBO._attributeName);
				if(locationVertex != -1)
				{
					info->_vertexVBO._pVBO->use();
					glVertexAttribPointer(locationVertex, info->_vertexVBO._numberComponents, GL_FLOAT, GL_FALSE, 0, 0);
					printOpenGLError();
					glEnableVertexAttribArray(locationVertex);
				}

				printOpenGLError();

				GLint locationBoneWeights = gpuprogram->getAttributLocation(info->_bonesWeightsVBO._attributeName);
				GLint locationBoneIndices = gpuprogram->getAttributLocation(info->_bonesIndicesVBO._attributeName);
				if(submesh->isSkeletonSkinActived())
				{
					
					if(locationBoneWeights != -1)
					{
						info->_bonesWeightsVBO._pVBO->use();
						glVertexAttribPointer(locationBoneWeights, info->_bonesWeightsVBO._numberComponents, GL_FLOAT, GL_FALSE, 0, 0);
						printOpenGLError();
						glEnableVertexAttribArray(locationBoneWeights);
					}
					
					if(locationBoneIndices != -1)
					{
						info->_bonesIndicesVBO._pVBO->use();
						glVertexAttribPointer(locationBoneIndices, info->_bonesIndicesVBO._numberComponents, GL_INT, GL_FALSE, 0, 0);
						printOpenGLError();
						glEnableVertexAttribArray(locationBoneIndices);
					}
				}

				info->_indexVBO->use();
				if(gpuprogram->isTessellationGpuProgram())
				{
					//TODO : tesselation is difficult
					glPatchParameteri(GL_PATCH_VERTICES, 3);
					glDrawElements( GL_PATCHES, info->_indexVBO->dataSize(), GL_UNSIGNED_INT, 0 );

				}else
				{
					glDrawElements(GL_TRIANGLES, info->_indexVBO->dataSize(), GL_UNSIGNED_INT, 0 );
				}

				printOpenGLError();

				info->_indexVBO->unUse();
				printOpenGLError();



				if(locationTex != -1)
				{
					glDisableVertexAttribArray(locationTex);
					printOpenGLError();
					info->_texCoordVBO._pVBO->unUse();
					printOpenGLError();
				}
				if(locationNormal != -1)
				{
					glDisableVertexAttribArray(locationNormal);
					printOpenGLError();
					info->_normalVBO._pVBO->unUse();
					printOpenGLError();
				}
				if(locationVertex != -1)
				{
					glDisableVertexAttribArray(locationVertex);
					printOpenGLError();
					info->_vertexVBO._pVBO->unUse();
					printOpenGLError();
				}
				if(submesh->isSkeletonSkinActived())
				{
					if(locationBoneWeights != -1)
					{
						glDisableVertexAttribArray(locationBoneWeights);
						printOpenGLError();
						info->_bonesWeightsVBO._pVBO->unUse();
						printOpenGLError();
					}
					if(locationBoneIndices != -1)
					{
						glDisableVertexAttribArray(locationBoneIndices);
						printOpenGLError();
						info->_bonesIndicesVBO._pVBO->unUse();
						printOpenGLError();
					}
				}
				gpuprogram->unUse();
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
