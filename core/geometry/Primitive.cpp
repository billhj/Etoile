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

	void Quad::init()
	{
		SubMesh* submesh = new SubMesh("Quad");
		submesh->setMaterial(new Material("empty"));
		submesh->getMaterial()->setDiffuse(Vec4f(0,0,1,1));
		this->addSubMesh(submesh);
		
		submesh->getOriginalVertices().push_back(Vec3f(  1.0, -1.0, -1));
		submesh->getOriginalVertices().push_back(Vec3f( -1.0, -1.0, -1));
		submesh->getOriginalVertices().push_back(Vec3f(  1.0,  1.0, -1));

		submesh->getOriginalVertices().push_back(Vec3f( -1.0,  1.0, -1));
		submesh->getOriginalVertices().push_back(Vec3f(  1.0,  1.0, -1));
		submesh->getOriginalVertices().push_back(Vec3f( -1.0, -1.0, -1));

		submesh->getOriginalTextureCoords().push_back(Vec2f(1.0, 0.0));
		submesh->getOriginalTextureCoords().push_back(Vec2f(0.0, 0.0));
		submesh->getOriginalTextureCoords().push_back(Vec2f(1.0, 1.0));

		submesh->getOriginalTextureCoords().push_back(Vec2f(0.0, 1.0));
		submesh->getOriginalTextureCoords().push_back(Vec2f(1.0, 1.0));
		submesh->getOriginalTextureCoords().push_back(Vec2f(0.0, 0.0));

		submesh->getOriginalNormals().push_back(Vec3f( 0.0, 0, 1.0));
		submesh->getOriginalNormals().push_back(Vec3f( 0.0, 0, 1.0));
		submesh->getOriginalNormals().push_back(Vec3f( 0.0, 0, 1.0));
		submesh->getOriginalNormals().push_back(Vec3f( 0.0, 0, 1.0));
		submesh->getOriginalNormals().push_back(Vec3f( 0.0, 0, 1.0));
		submesh->getOriginalNormals().push_back(Vec3f( 0.0, 0, 1.0));

		submesh->getOriginalVertexIndexForFaces().push_back(0);
		submesh->getOriginalVertexIndexForFaces().push_back(1);
		submesh->getOriginalVertexIndexForFaces().push_back(2);
		submesh->getOriginalVertexIndexForFaces().push_back(3);
		submesh->getOriginalVertexIndexForFaces().push_back(4);
		submesh->getOriginalVertexIndexForFaces().push_back(5);
		submesh->initSkin();
	}




	void Triangle::init()
	{
		SubMesh* submesh = new SubMesh("Triangle");
		submesh->setMaterial(new Material("empty"));
		submesh->getMaterial()->setDiffuse(Vec4f(0,0,1,1));
		this->addSubMesh(submesh);
		
		submesh->getOriginalVertices().push_back(Vec3f(-1.0, -1.0, 0.0));
		submesh->getOriginalVertices().push_back(Vec3f( 1.0, -1.0, 0.0));
		submesh->getOriginalVertices().push_back(Vec3f( 0.0f,  1.0, 0.0));

		submesh->getOriginalTextureCoords().push_back(Vec2f(1.0f, 0.0f));
		submesh->getOriginalTextureCoords().push_back(Vec2f(0.0f, 0.0f));
		submesh->getOriginalTextureCoords().push_back(Vec2f(1.0f, 1.0f));

		submesh->getOriginalNormals().push_back(Vec3f( 0.0f, 1.0f, 0));
		submesh->getOriginalNormals().push_back(Vec3f( 0.0f, 1.0f, 0));
		submesh->getOriginalNormals().push_back(Vec3f( 0.0f, 1.0f, 0));
		submesh->initSkin();
	}

	void Plane::init()
	{
		SubMesh* submesh = new SubMesh("Plane");
		submesh->setMaterial(new Material("empty"));
		submesh->getMaterial()->setDiffuse(Vec4f(0.9,0.9,0.9,1));
		this->addSubMesh(submesh);
		int index = 0;
		for(int i = -_line; i < _line; ++i)
		{
			for(int j = -_line; j < _line; ++j)
			{
				submesh->getOriginalVertices().push_back(Vec3f(  1.0 + i, 0, j - 1.0));
				submesh->getOriginalVertices().push_back(Vec3f( -1.0 + i, 0, j - 1.0));
				submesh->getOriginalVertices().push_back(Vec3f(  1.0 + i, 0, j + 1.0));

				submesh->getOriginalVertices().push_back(Vec3f( -1.0 + i, 0, j + 1.0));
				submesh->getOriginalVertices().push_back(Vec3f(  1.0 + i, 0, j + 1.0));
				submesh->getOriginalVertices().push_back(Vec3f( -1.0 + i, 0, j - 1.0));

				float x0 = (i + _line) / (2.0 * _line);
				float x1 = (i + _line + 1.0) / (2.0 * _line);
				float z0 = (j + _line) / (2.0 * _line);
				float z1 = (j + _line + 1.0) / (2.0 * _line);

				submesh->getOriginalTextureCoords().push_back(Vec2f(1, 0));
				submesh->getOriginalTextureCoords().push_back(Vec2f(0, 0));
				submesh->getOriginalTextureCoords().push_back(Vec2f(1, 1));

				submesh->getOriginalTextureCoords().push_back(Vec2f(0, 1));
				submesh->getOriginalTextureCoords().push_back(Vec2f(1, 1));
				submesh->getOriginalTextureCoords().push_back(Vec2f(0, 0));

				/*submesh->getOriginalTextureCoords().push_back(Vec2f(x1, z0));
				submesh->getOriginalTextureCoords().push_back(Vec2f(x0, z0));
				submesh->getOriginalTextureCoords().push_back(Vec2f(x1, z1));

				submesh->getOriginalTextureCoords().push_back(Vec2f(x0, z1));
				submesh->getOriginalTextureCoords().push_back(Vec2f(x1, z1));
				submesh->getOriginalTextureCoords().push_back(Vec2f(x0, z0));*/

				submesh->getOriginalNormals().push_back(Vec3f( 0.0, 1.0, 0));
				submesh->getOriginalNormals().push_back(Vec3f( 0.0, 1.0, 0));
				submesh->getOriginalNormals().push_back(Vec3f( 0.0, 1.0, 0));
				submesh->getOriginalNormals().push_back(Vec3f( 0.0, 1.0, 0));
				submesh->getOriginalNormals().push_back(Vec3f( 0.0, 1.0, 0));
				submesh->getOriginalNormals().push_back(Vec3f( 0.0, 1.0, 0));

				submesh->getOriginalVertexIndexForFaces().push_back(index++);
				submesh->getOriginalVertexIndexForFaces().push_back(index++);
				submesh->getOriginalVertexIndexForFaces().push_back(index++);
				submesh->getOriginalVertexIndexForFaces().push_back(index++);
				submesh->getOriginalVertexIndexForFaces().push_back(index++);
				submesh->getOriginalVertexIndexForFaces().push_back(index++);
			}
		}
		submesh->initSkin();
		this->computeAABB();
	}

	void Plane::reinit()
	{
		SubMesh* submesh = this->getSubMesh(0);
		submesh->getOriginalTextureCoords().clear();
		submesh->getOriginalNormals().clear();
		submesh->getOriginalVertexIndexForFaces().clear();
		int index = 0;
		for(int i = -_line; i < _line; ++i)
		{
			for(int j = -_line; j < _line; ++j)
			{
				submesh->getOriginalVertices().push_back(Vec3f(  1.0 + i, 0, j - 1.0));
				submesh->getOriginalVertices().push_back(Vec3f( -1.0 + i, 0, j - 1.0));
				submesh->getOriginalVertices().push_back(Vec3f(  1.0 + i, 0, j + 1.0));

				submesh->getOriginalVertices().push_back(Vec3f( -1.0 + i, 0, j + 1.0));
				submesh->getOriginalVertices().push_back(Vec3f(  1.0 + i, 0, j + 1.0));
				submesh->getOriginalVertices().push_back(Vec3f( -1.0 + i, 0, j - 1.0));

				float x0 = (i + _line) / (2.0 * _line);
				float x1 = (i + _line + 1.0) / (2.0 * _line);
				float z0 = (j + _line) / (2.0 * _line);
				float z1 = (j + _line + 1.0) / (2.0 * _line);

				submesh->getOriginalTextureCoords().push_back(Vec2f(1, 0));
				submesh->getOriginalTextureCoords().push_back(Vec2f(0, 0));
				submesh->getOriginalTextureCoords().push_back(Vec2f(1, 1));

				submesh->getOriginalTextureCoords().push_back(Vec2f(0, 1));
				submesh->getOriginalTextureCoords().push_back(Vec2f(1, 1));
				submesh->getOriginalTextureCoords().push_back(Vec2f(0, 0));

				/*submesh->getOriginalTextureCoords().push_back(Vec2f(x1, z0));
				submesh->getOriginalTextureCoords().push_back(Vec2f(x0, z0));
				submesh->getOriginalTextureCoords().push_back(Vec2f(x1, z1));

				submesh->getOriginalTextureCoords().push_back(Vec2f(x0, z1));
				submesh->getOriginalTextureCoords().push_back(Vec2f(x1, z1));
				submesh->getOriginalTextureCoords().push_back(Vec2f(x0, z0));*/

				submesh->getOriginalNormals().push_back(Vec3f( 0.0, 1.0, 0));
				submesh->getOriginalNormals().push_back(Vec3f( 0.0, 1.0, 0));
				submesh->getOriginalNormals().push_back(Vec3f( 0.0, 1.0, 0));
				submesh->getOriginalNormals().push_back(Vec3f( 0.0, 1.0, 0));
				submesh->getOriginalNormals().push_back(Vec3f( 0.0, 1.0, 0));
				submesh->getOriginalNormals().push_back(Vec3f( 0.0, 1.0, 0));

				submesh->getOriginalVertexIndexForFaces().push_back(index++);
				submesh->getOriginalVertexIndexForFaces().push_back(index++);
				submesh->getOriginalVertexIndexForFaces().push_back(index++);
				submesh->getOriginalVertexIndexForFaces().push_back(index++);
				submesh->getOriginalVertexIndexForFaces().push_back(index++);
				submesh->getOriginalVertexIndexForFaces().push_back(index++);
			}
		}
		submesh->initSkin();
		this->computeAABB();
	}
}
