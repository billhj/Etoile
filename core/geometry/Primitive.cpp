/**
* Copyright(C) 2009-2012                
* @author Jing HUANG
* @file Primitive.h
* @brief 
* @date 1/2/2011
*/

#include "Primitive.h"
#include <string.h>

/**
* @brief For tracking memory leaks under windows using the crtdbg
*/
#if ( defined( _DEBUG ) || defined( DEBUG ) ) && defined( _MSC_VER )
#define _CRTDBG_MAP_ALLOC
#include <crtdbg.h>

#define DEBUG_NEW new( _NORMAL_BLOCK, __FILE__, __LINE__ )
#define new DEBUG_NEW
#endif

namespace Etoile
{
	void Primitive::computerAABB()
	{
		_aabb.build(_vdata);
	}

	void Primitive::setMesh(Mesh* mesh)
	{
		if(mesh == NULL) return;
		_pMesh = mesh;
		SubMesh* submesh = new SubMesh("empty");
		submesh->setMaterial(new Material("empty"));
		submesh->getMaterial()->setDiffuse(Vec4f(1,1,1,1));
		mesh->addSubMesh(submesh);
		
		submesh->setOriginalVertice(_vdata);
		submesh->setOriginalNormal(_ndata);
		submesh->setOriginalTextureCoords(_tdata);
		submesh->setOriginalVertexIndexForFaces(_vertices_index_face);
		_pMesh->initResource();
	}

	void Primitive::perform(Matrix4f& gltransformation)
	{
		if(_pMesh != NULL)
		{
			_pMesh->perform(gltransformation);
		}
	}

	void Quad::init()
	{
		_vdata.push_back(Vec3f(  1.0, -1.0, -1));
		_vdata.push_back(Vec3f( -1.0, -1.0, -1));
		_vdata.push_back(Vec3f(  1.0,  1.0, -1));

		_vdata.push_back(Vec3f( -1.0,  1.0, -1));
		_vdata.push_back(Vec3f(  1.0,  1.0, -1));
		_vdata.push_back(Vec3f( -1.0, -1.0, -1));

		_tdata.push_back(Vec2f(1.0, 0.0));
		_tdata.push_back(Vec2f(0.0, 0.0));
		_tdata.push_back(Vec2f(1.0, 1.0));

		_tdata.push_back(Vec2f(0.0, 1.0));
		_tdata.push_back(Vec2f(1.0, 1.0));
		_tdata.push_back(Vec2f(0.0, 0.0));

		_ndata.push_back(Vec3f( 0.0, 0, 1.0));
		_ndata.push_back(Vec3f( 0.0, 0, 1.0));
		_ndata.push_back(Vec3f( 0.0, 0, 1.0));
		_ndata.push_back(Vec3f( 0.0, 0, 1.0));
		_ndata.push_back(Vec3f( 0.0, 0, 1.0));
		_ndata.push_back(Vec3f( 0.0, 0, 1.0));

		_vertices_index_face.push_back(0);
		_vertices_index_face.push_back(1);
		_vertices_index_face.push_back(2);
		_vertices_index_face.push_back(3);
		_vertices_index_face.push_back(4);
		_vertices_index_face.push_back(5);

	}




	void Triangle::init()
	{

		int index = 0;
		_vdata.push_back(Vec3f(-1.0, -1.0, 0.0));
		_vdata.push_back(Vec3f( 1.0, -1.0, 0.0));
		_vdata.push_back(Vec3f( 0.0f,  1.0, 0.0));

		_tdata.push_back(Vec2f(1.0f, 0.0f));
		_tdata.push_back(Vec2f(0.0f, 0.0f));
		_tdata.push_back(Vec2f(1.0f, 1.0f));

		_ndata.push_back(Vec3f( 0.0f, 1.0f, 0));
		_ndata.push_back(Vec3f( 0.0f, 1.0f, 0));
		_ndata.push_back(Vec3f( 0.0f, 1.0f, 0));

		_vertices_index_face.push_back(index++);
		_vertices_index_face.push_back(index++);
		_vertices_index_face.push_back(index++);

	}

	void Plane::init()
	{
	
		int index = 0;
		for(int i = -_line; i < _line; ++i)
		{
			for(int j = -_line; j < _line; ++j)
			{
				_vdata.push_back(Vec3f(  1.0 + i, 0, j - 1.0));
				_vdata.push_back(Vec3f( -1.0 + i, 0, j - 1.0));
				_vdata.push_back(Vec3f(  1.0 + i, 0, j + 1.0));

				_vdata.push_back(Vec3f( -1.0 + i, 0, j + 1.0));
				_vdata.push_back(Vec3f(  1.0 + i, 0, j + 1.0));
				_vdata.push_back(Vec3f( -1.0 + i, 0, j - 1.0));

				float x0 = (i + _line) / (2.0 * _line);
				float x1 = (i + _line + 1.0) / (2.0 * _line);
				float z0 = (j + _line) / (2.0 * _line);
				float z1 = (j + _line + 1.0) / (2.0 * _line);

				_tdata.push_back(Vec2f(1, 0));
				_tdata.push_back(Vec2f(0, 0));
				_tdata.push_back(Vec2f(1, 1));

				_tdata.push_back(Vec2f(0, 1));
				_tdata.push_back(Vec2f(1, 1));
				_tdata.push_back(Vec2f(0, 0));

				/*_tdata.push_back(Vec2f(x1, z0));
				_tdata.push_back(Vec2f(x0, z0));
				_tdata.push_back(Vec2f(x1, z1));

				_tdata.push_back(Vec2f(x0, z1));
				_tdata.push_back(Vec2f(x1, z1));
				_tdata.push_back(Vec2f(x0, z0));*/

				_ndata.push_back(Vec3f( 0.0, 1.0, 0));
				_ndata.push_back(Vec3f( 0.0, 1.0, 0));
				_ndata.push_back(Vec3f( 0.0, 1.0, 0));
				_ndata.push_back(Vec3f( 0.0, 1.0, 0));
				_ndata.push_back(Vec3f( 0.0, 1.0, 0));
				_ndata.push_back(Vec3f( 0.0, 1.0, 0));

				_vertices_index_face.push_back(index++);
				_vertices_index_face.push_back(index++);
				_vertices_index_face.push_back(index++);
				_vertices_index_face.push_back(index++);
				_vertices_index_face.push_back(index++);
				_vertices_index_face.push_back(index++);
			}
		}

	}

	void Plane::reinit()
	{

		_tdata.clear();
		_ndata.clear();
		_vertices_index_face.clear();
		int index = 0;
		for(int i = -_line; i < _line; ++i)
		{
			for(int j = -_line; j < _line; ++j)
			{
				_vdata.push_back(Vec3f(  1.0 + i, 0, j - 1.0));
				_vdata.push_back(Vec3f( -1.0 + i, 0, j - 1.0));
				_vdata.push_back(Vec3f(  1.0 + i, 0, j + 1.0));

				_vdata.push_back(Vec3f( -1.0 + i, 0, j + 1.0));
				_vdata.push_back(Vec3f(  1.0 + i, 0, j + 1.0));
				_vdata.push_back(Vec3f( -1.0 + i, 0, j - 1.0));

				float x0 = (i + _line) / (2.0 * _line);
				float x1 = (i + _line + 1.0) / (2.0 * _line);
				float z0 = (j + _line) / (2.0 * _line);
				float z1 = (j + _line + 1.0) / (2.0 * _line);

				_tdata.push_back(Vec2f(1, 0));
				_tdata.push_back(Vec2f(0, 0));
				_tdata.push_back(Vec2f(1, 1));

				_tdata.push_back(Vec2f(0, 1));
				_tdata.push_back(Vec2f(1, 1));
				_tdata.push_back(Vec2f(0, 0));

				/*_tdata.push_back(Vec2f(x1, z0));
				_tdata.push_back(Vec2f(x0, z0));
				_tdata.push_back(Vec2f(x1, z1));

				_tdata.push_back(Vec2f(x0, z1));
				_tdata.push_back(Vec2f(x1, z1));
				_tdata.push_back(Vec2f(x0, z0));*/

				_ndata.push_back(Vec3f( 0.0, 1.0, 0));
				_ndata.push_back(Vec3f( 0.0, 1.0, 0));
				_ndata.push_back(Vec3f( 0.0, 1.0, 0));
				_ndata.push_back(Vec3f( 0.0, 1.0, 0));
				_ndata.push_back(Vec3f( 0.0, 1.0, 0));
				_ndata.push_back(Vec3f( 0.0, 1.0, 0));

				_vertices_index_face.push_back(index++);
				_vertices_index_face.push_back(index++);
				_vertices_index_face.push_back(index++);
				_vertices_index_face.push_back(index++);
				_vertices_index_face.push_back(index++);
				_vertices_index_face.push_back(index++);
			}
		}
	}
}
