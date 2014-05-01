/**
* Copyright(C) 2009-2012                
* @author Jing HUANG
* @file SubMesh.h
* @brief 
* @date 1/2/2011
*/

#pragma once
#include <string>
#include <vector>
#include "math/MathHead.h"
#include "Material.h"
#include "Skeleton.h"
#include "AxisAlignedBoundingBox.h"
#include "BonesAssignment.h"

#ifdef USING_BOOST
#include <boost/serialization/map.hpp>
#endif

namespace Etoile
{

	class SubMesh
	{
	public:
		SubMesh(const std::string& name): _name(name), _nbVerticesPerFace(3)
		{
		}

		SubMesh(SubMesh& sub): _nbVerticesPerFace(3)
		{
			_name = sub._name;
			_pmaterial = sub._pmaterial;
			_nbVerticesPerFace = sub._nbVerticesPerFace;
			_numberOfFaces = sub._numberOfFaces;
			_vertices_index_face = sub._vertices_index_face;
			_normal_index_face = sub._normal_index_face;
			_texcoord_index_face = sub._texcoord_index_face;
			_vdata = sub._vdata;
			_ndata = sub._ndata;
			_tdata = sub._tdata;
			_bonesassignments = sub._bonesassignments;
			_vBonesIndices = sub._vBonesIndices;
			_vBonesWeights = sub._vBonesWeights;
		}

		virtual void release()
		{
			_vertices_index_face.clear();
			_normal_index_face.clear();
			_texcoord_index_face.clear();
			_vdata.clear();
			_ndata.clear();
			_tdata.clear();
			_bonesassignments.clear();
			_vBonesIndices.clear();
			_vBonesWeights.clear();
		}

		const std::string getName(){return _name;}
		void setName(const std::string& name){ _name = name;}

		void setMaterial(Material* material)
		{
			_pmaterial = material;
		}

		Material* getMaterial()
		{
			return _pmaterial;
		}

		void addVertexIndex(int vertices_index)
		{
			_vertices_index_face.push_back(vertices_index);
		}

		void addNormalIndex(int normal_index)
		{
			_normal_index_face.push_back(normal_index);
		}

		void addTextureCoordinateIndex(int texcoord_index)
		{
			_texcoord_index_face.push_back(texcoord_index);
		}

		std::vector<unsigned int>& getVertexIndexForFaces(){return _vertices_index_face;}
		std::vector<unsigned int>& getNormalIndexForFaces(){return _normal_index_face;}
		std::vector<unsigned int>&  getTextureIndexForFaces(){return _texcoord_index_face;}
		std::vector<Vec3f>&  getVertices(){return _vdata;} 
		std::vector<Vec3f>&  getNormals(){return _ndata;}
		std::vector<Vec2f>& getTextureCoords(){return _tdata;}
		std::vector<Vec4f>&  getBonesWeights(){return _vBonesWeights;} 
		std::vector<Vec4i>&  getBonesIndices(){return _vBonesIndices;} 
		std::vector<BonesAssignment>& getBonesAssignment(){return _bonesassignments;}
		void setActiveSkeletonSkin(bool t){ _activeSkinning = t;}
		bool isSkeletonSkinActived(){return _activeSkinning;}

		void setVertice(const std::vector<Vec3f>& data)
		{
			_vdata = data;
		}

		void setNormal(const std::vector<Vec3f>& data)
		{
			_ndata = data;
		}

		
		void setTextureCoords(const std::vector<Vec2f>& data)
		{
			_tdata = data;
		}

		void setVertexIndexForFaces(const std::vector<unsigned int>& data)
		{
			_vertices_index_face = data;
		}

		void computeAABB()
		{
			_aabb.build(_vdata);
		}

		AxisAlignedBoundingBoxf* getAABB(){return &_aabb;}

#ifdef USING_BOOST
		friend class boost::serialization::access;

		template<class Archive>
		void serialize(Archive & ar, const unsigned int version)
		{
			ar & _name;
			ar & _material;
			ar & _nbVerticesPerFace;
			ar & _numberOfFaces;
			ar & _vdata;
			ar & _ndata;
			ar & _tdata;
			ar & _vertices_index_face;
		}
#endif
	private:
		Material* _pmaterial;
		AxisAlignedBoundingBoxf _aabb;
	public:
		int _nbVerticesPerFace;
		int _numberOfFaces;
	private:
		std::vector<unsigned int> _vertices_index_face;
		std::vector<unsigned int> _normal_index_face;
		std::vector<unsigned int> _texcoord_index_face;
		std::vector<BonesAssignment> _bonesassignments;
		bool _activeSkinning;
		std::vector<Vec3f> _vdata, _ndata;
		std::vector<Vec4i> _vBonesIndices;
		std::vector<Vec4f> _vBonesWeights;
		std::vector<Vec2f> _tdata;
		std::string _name;
	};
}