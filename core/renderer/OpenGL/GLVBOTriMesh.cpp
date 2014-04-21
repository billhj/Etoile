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
	void DynamicGLVBOTriMesh::initVBO()
	{
		_vertex_vbo.setUsage(GL_DYNAMIC_DRAW);
		_normal_vbo.setUsage(GL_DYNAMIC_DRAW);
		_texcoord_vbo.setUsage(GL_DYNAMIC_DRAW);
		_vertex_vbo.bindData(_vdata.size(), &_vdata[0]);
		_normal_vbo.bindData(_ndata.size(), &_ndata[0]);
		_texcoord_vbo.bindData(_tdata.size(), &_tdata[0]);

		_ibos.resize(_indices.size());
		for(unsigned int i = 0; i < _indices.size(); ++i)
		{
			std::vector<unsigned int>& indices = _indices[i];
			_ibos[i].bindData(indices.size(), &indices[0]);
		}
	}

}