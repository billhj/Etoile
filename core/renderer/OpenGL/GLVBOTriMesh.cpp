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


	void StaticGLVBOTriMesh::initVBO()
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
}