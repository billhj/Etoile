/**
* Copyright(C) 2009-2012                
* @author Jing HUANG
* @file Primitive.h
* @brief 
* @date 1/2/2011
*/

#pragma once
#include <vector>
#include "MovableObject.h"
#include "Mesh.h"
namespace Etoile
{
	class Primitive : public MovableObject
	{
	protected:
		Mesh* _pMesh;
		std::vector<Vec3f> _vdata, _ndata;
		std::vector<Vec2f> _tdata;
		std::vector<int> _vertices_index_face;
	public:
		Primitive() : MovableObject(), _pMesh(NULL){}
		virtual void computerAABB() override;
		void setMesh(Mesh* mesh);
		Mesh* getMesh(){return _pMesh;}
		virtual void perform(Matrix4f& gltransformation) override;
	};


	class Quad : public Primitive
	{
	public:
		Quad(): Primitive() { init();}
		virtual void init();
	protected:

	};

	class Triangle : public Primitive
	{
	public:
		Triangle(): Primitive() { init();}
		virtual void init();
	protected:
	};


	class Plane :  public Primitive
	{
	public:
		Plane(): Primitive(), _line(10){init();}
		void setLine(int line){_line = line;}
		virtual void init();
		void reinit();
	protected:
		int _line;
	};
}

