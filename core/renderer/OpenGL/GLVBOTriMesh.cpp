/**
* Copyright(C) 2009-2012                
* @author Jing HUANG
* @file GLVBOTriMesh.cpp
* @brief 
* @date 1/2/2011
*/

#include "GLVBOTriMesh.h"

namespace Etoile
{
	void applyMaterial(Material* material)
	{
		if(material == NULL) return;
		//glEnable(GL_COLOR_MATERIAL);
		glEnable(GL_LIGHTING);
		Vec4f ambient = material->getAmbientFinal();
		glMaterialfv(GL_FRONT, GL_AMBIENT, &Vec4f((float)ambient[0], (float)ambient[1], (float)ambient[2], (float)ambient[3])[0]);
		Vec4f diffuse = material->getDiffuseFinal();
		glMaterialfv(GL_FRONT, GL_DIFFUSE, &Vec4f((float)diffuse[0], (float)diffuse[1], (float)diffuse[2], (float)diffuse[3])[0]);
		Vec4f specular = material->getSpecularFinal();
		glMaterialfv(GL_FRONT, GL_SPECULAR, &Vec4f((float)specular[0], (float)specular[1], (float)specular[2], (float)specular[3])[0]);
		glMateriali(GL_FRONT, GL_SHININESS, material->getSpecularShininess());
		float tranparencyValue = material->getDissolvedOpacity();
		//make sure need to use  applying glEnable(GL_POLYGON_STIPPLE);
		glEnable(GL_POLYGON_STIPPLE);
		glPolygonStipple(__stippleMask[int(tranparencyValue * __screenDoorMaskRange)]);
	}

	void DynamicGLVBOSharedTriMesh::initVBO()
	{
		_vdata_buffer = _vdata;
		_ndata_buffer = _ndata;
		_tdata_buffer = _tdata;

		_vertex_vbo.setUsage(GL_DYNAMIC_DRAW);
		_normal_vbo.setUsage(GL_DYNAMIC_DRAW);
		_texcoord_vbo.setUsage(GL_DYNAMIC_DRAW);
		_vertex_vbo.bindData(_vdata_buffer.size(), &_vdata_buffer[0]);
		_normal_vbo.bindData(_ndata_buffer.size(), &_ndata_buffer[0]);
		_texcoord_vbo.bindData(_tdata_buffer.size(), &_tdata_buffer[0]);

		_ibos.resize(_indices.size());
		for(unsigned int i = 0; i < _indices.size(); ++i)
		{
			std::vector<unsigned int>& indices = _indices[i];
			_ibos[i].bindData(indices.size(), &indices[0]);
		}
	}

	void DynamicGLVBOSharedTriMesh::drawElements()
	{
		_texcoord_vbo.use();
		glTexCoordPointer(2, GL_FLOAT, 0, 0);
		_normal_vbo.use();
		glNormalPointer(GL_FLOAT, 0, 0);
		_vertex_vbo.use();
		glVertexPointer(3, GL_FLOAT, 0, 0);

		glEnableClientState(GL_TEXTURE_COORD_ARRAY);
		glEnableClientState(GL_NORMAL_ARRAY);
		glEnableClientState(GL_VERTEX_ARRAY);

		printOpenGLError();

		for(unsigned int i = 0; i < _indices.size(); ++i)
		{
			applyMaterial(&_materials[i]);
			Texture* t = _materials[i].getDiffuseTexture();
			if(t != NULL)
			{
				t->use();
			}
			_ibos[i].use();
			glDrawElements( GL_TRIANGLES, _ibos[i].dataSize(), GL_UNSIGNED_INT, 0);
			_ibos[i].unUse();
			if(t != NULL)
			{
				t->unUse();
			}
		}

		printOpenGLError();

		glDisableClientState(GL_TEXTURE_COORD_ARRAY);
		glDisableClientState(GL_NORMAL_ARRAY);
		glDisableClientState(GL_VERTEX_ARRAY);
		_texcoord_vbo.unUse();
		_normal_vbo.unUse();
		_vertex_vbo.unUse();
	}


	void DynamicGLVBOSharedTriMesh::drawCustumElements()
	{

	}

	void StaticGLVBOSharedTriMesh::initVBO()
	{
		_interleavedvbo.setUsage(GL_STATIC_DRAW);

		_interleaveddata.resize(_vdata.size());
		for(unsigned int i = 0; i < _vdata.size(); ++i)
		{
			_interleaveddata[i]._v = _vdata[i];
			_interleaveddata[i]._n = _ndata[i];
			_interleaveddata[i]._t = _tdata[i];
		}
		_interleavedvbo.bindData(_interleaveddata.size(), &_interleaveddata[0]);
		_ibos.resize(_indices.size());
		for(unsigned int i = 0; i < _indices.size(); ++i)
		{
			std::vector<unsigned int>& indices = _indices[i];
			_ibos[i].bindData(indices.size(), &indices[0]);
		}
	}

	void StaticGLVBOSharedTriMesh::drawElements()
	{
		_interleavedvbo.use();
		GLsizei stride = sizeof(VertexData);
		assert(stride==32);
		glVertexPointer(3, GL_FLOAT, stride, &_interleaveddata[0]._v[0]);
		glNormalPointer(GL_FLOAT, stride, &_interleaveddata[0]._n[0]);
		glTexCoordPointer(2, GL_FLOAT, stride, &_interleaveddata[0]._t[0]);

		glEnableClientState(GL_VERTEX_ARRAY);
		glEnableClientState(GL_NORMAL_ARRAY);
		glEnableClientState(GL_TEXTURE_COORD_ARRAY);


		printOpenGLError();

		for(unsigned int i = 0; i < _indices.size(); ++i)
		{
			applyMaterial(&_materials[i]);
			Texture* t = _materials[i].getDiffuseTexture();
			if(t != NULL)
			{
				t->use();
			}
			_ibos[i].use();
			glDrawElements( GL_TRIANGLES, _ibos[i].dataSize(), GL_UNSIGNED_INT, 0);
			_ibos[i].unUse();
			if(t != NULL)
			{
				t->unUse();
			}
		}

		printOpenGLError();

		glDisableClientState(GL_VERTEX_ARRAY);
		glDisableClientState(GL_NORMAL_ARRAY);
		glDisableClientState(GL_TEXTURE_COORD_ARRAY);
		_interleavedvbo.unUse();
	}






	void DynamicGLVBOSeparateTriMesh::initVBO()
	{
		_vdata_buffer = _vdata;
		_ndata_buffer = _ndata;
		_tdata_buffer = _tdata;

		_vertex_vbo.resize(_vdata_buffer.size());
		_normal_vbo.resize(_ndata_buffer.size());
		_texcoord_vbo.resize(_tdata_buffer.size());
		for(unsigned int i = 0; i < _vertex_vbo.size(); ++i){
			_vertex_vbo[i].setUsage(GL_DYNAMIC_DRAW);
			_normal_vbo[i].setUsage(GL_DYNAMIC_DRAW);
			_texcoord_vbo[i].setUsage(GL_DYNAMIC_DRAW);
			_vertex_vbo[i].bindData(_vdata_buffer[i].size(), &_vdata_buffer[i][0]);
			_normal_vbo[i].bindData(_ndata_buffer[i].size(), &_ndata_buffer[i][0]);
			_texcoord_vbo[i].bindData(_tdata_buffer[i].size(), &_tdata_buffer[i][0]);
		}

		_ibos.resize(_indices.size());
		for(unsigned int i = 0; i < _indices.size(); ++i)
		{
			std::vector<unsigned int>& indices = _indices[i];
			_ibos[i].bindData(indices.size(), &indices[0]);
		}
	}

	void DynamicGLVBOSeparateTriMesh::drawElements()
	{
		for(unsigned int i = 0; i < _indices.size(); ++i)
		{
			_texcoord_vbo[i].use();
			glTexCoordPointer(2, GL_FLOAT, 0, 0);
			_normal_vbo[i].use();
			glNormalPointer(GL_FLOAT, 0, 0);
			_vertex_vbo[i].use();
			glVertexPointer(3, GL_FLOAT, 0, 0);

			glEnableClientState(GL_TEXTURE_COORD_ARRAY);
			glEnableClientState(GL_NORMAL_ARRAY);
			glEnableClientState(GL_VERTEX_ARRAY);

			printOpenGLError();


			applyMaterial(&_materials[i]);
			Texture* t = _materials[i].getDiffuseTexture();
			if(t != NULL)
			{
				t->use();
			}
			_ibos[i].use();
			glDrawElements( GL_TRIANGLES, _ibos[i].dataSize(), GL_UNSIGNED_INT, 0);
			_ibos[i].unUse();
			if(t != NULL)
			{
				t->unUse();
			}


			printOpenGLError();

			glDisableClientState(GL_TEXTURE_COORD_ARRAY);
			glDisableClientState(GL_NORMAL_ARRAY);
			glDisableClientState(GL_VERTEX_ARRAY);
			_texcoord_vbo[i].unUse();
			_normal_vbo[i].unUse();
			_vertex_vbo[i].unUse();
		}
	}


	void DynamicGLVBOSeparateTriMesh::drawCustumElements()
	{

	}

	void StaticGLVBOSeparateTriMesh::initVBO()
	{
		_interleavedvbo.resize(_interleaveddata.size());
		for(unsigned int i = 0; i < _interleavedvbo.size(); ++i)
		{
			_interleavedvbo[i].setUsage(GL_STATIC_DRAW);
			_interleaveddata[i].resize(_vdata[i].size());
			for(unsigned int j = 0; j < _vdata[i].size(); ++j)
			{
				_interleaveddata[i][j]._v = _vdata[i][j];
				_interleaveddata[i][j]._n = _ndata[i][j];
				_interleaveddata[i][j]._t = _tdata[i][j];
			}
			_interleavedvbo[i].bindData(_interleaveddata[i].size(), &_interleaveddata[i][0]);
		}

		_ibos.resize(_indices.size());
		for(unsigned int i = 0; i < _indices.size(); ++i)
		{
			std::vector<unsigned int>& indices = _indices[i];
			_ibos[i].bindData(indices.size(), &indices[0]);
		}
	}

	void StaticGLVBOSeparateTriMesh::drawElements()
	{
		for(unsigned int i = 0; i < _indices.size(); ++i)
		{
			_interleavedvbo[i].use();
			GLsizei stride = sizeof(VertexData);
			assert(stride==32);
			glVertexPointer(3, GL_FLOAT, stride, &_interleaveddata[i][0]._v[0]);
			glNormalPointer(GL_FLOAT, stride, &_interleaveddata[i][0]._n[0]);
			glTexCoordPointer(2, GL_FLOAT, stride, &_interleaveddata[i][0]._t[0]);

			glEnableClientState(GL_VERTEX_ARRAY);
			glEnableClientState(GL_NORMAL_ARRAY);
			glEnableClientState(GL_TEXTURE_COORD_ARRAY);


			printOpenGLError();


			applyMaterial(&_materials[i]);
			Texture* t = _materials[i].getDiffuseTexture();
			if(t != NULL)
			{
				t->use();
			}
			_ibos[i].use();
			glDrawElements( GL_TRIANGLES, _ibos[i].dataSize(), GL_UNSIGNED_INT, 0);
			_ibos[i].unUse();
			if(t != NULL)
			{
				t->unUse();
			}


			printOpenGLError();

			glDisableClientState(GL_VERTEX_ARRAY);
			glDisableClientState(GL_NORMAL_ARRAY);
			glDisableClientState(GL_TEXTURE_COORD_ARRAY);
			_interleavedvbo[i].unUse();
		}
	}
}