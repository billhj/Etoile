/**
* Copyright(C) 2009-2012                
* @author Jing HUANG
* @file Primitive.h
* @brief 
* @date 1/2/2011
*/

#pragma once
#include <vector>
#include "Mesh.h"

namespace Etoile
{
	class Primitive : public Mesh
	{
	public:
		Primitive(const std::string& name) : Mesh(name){}
		Material* getMaterial(){return getSubMesh(0)->getMaterial();}
		void setMaterial(Material* material){getSubMesh(0)->setMaterial(material);}
	};


	class Quad : public Primitive
	{
	public:
		Quad(const std::string& name): Primitive(name) { init();}
		virtual void init();
	protected:

	};

	class Triangle : public Primitive
	{
	public:
		Triangle(const std::string& name): Primitive(name) { init();}
		virtual void init();
	protected:
	};


	class Plane :  public Primitive
	{
	public:
		Plane(const std::string& name): Primitive(name), _line(4){init();}
		void setLine(int line){_line = line;}
		virtual void init();
		void reinit();
	protected:
		int _line;
	};
}

