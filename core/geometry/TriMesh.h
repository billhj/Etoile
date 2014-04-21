/**
* Copyright(C) 2009-2012                
* @author Jing HUANG
* @file TriMesh.h
* @brief 
* @date 1/2/2011
*/

#pragma once
#ifndef TRI_MESH_H
#define TRI_MESH_H
#include "math/MathHead.h"
#include "Material.h"

namespace Etoile
{
	struct TriMesh
	{
	public:
		std::vector<Vec3f> _vdata;
		std::vector<Vec3f> _ndata;
		std::vector<Vec2f> _tdata;
		std::vector<std::vector<unsigned int>> _indices;
		std::vector<Material> _materials;
	};
}

#endif //TRI_MESH_H