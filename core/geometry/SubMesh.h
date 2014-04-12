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
#include "VertexBoneAssignment.h"
#include "Material.h"
#include "Skeleton.h"
#include "AxisAlignedBoundingBox.h"

#ifdef USING_BOOST
#include <boost/serialization/map.hpp>
#endif

namespace Etoile
{

	struct Skin
	{
		bool _updated;
		std::vector<Vec3f> _vdata, _ndata;
		std::vector<Vec2f> _tdata;
		std::vector<VertexBoneAssignment> _vertexBoneAssignmentData;
		Skeleton *_pskeleton;
		Skin()
		{
			_pskeleton = NULL;
			_updated = false;
		}
		void bindSkeleton(Skeleton* skeleton){ _pskeleton = skeleton;}

		bool isUpdated(){return _updated;}
		void updateSkeleton(std::map<std::string, Quaternionf> rotations)
		{
			if(_pskeleton == NULL) return;
			std::map<std::string, Quaternionf>::iterator itor = rotations.begin();
			for(; itor != rotations.end(); ++itor)
			{
				Joint* joint = _pskeleton->getJoint(itor->first);
				joint->setLocalRotation(itor->second);
			}
			_pskeleton->update();
			computeSkinBySkeleton();
		}

		void computeSkinBySkeleton()
		{
			for(unsigned int i = 0; i < _vertexBoneAssignmentData.size(); ++i)
			{
				VertexBoneAssignment& dt = _vertexBoneAssignmentData[i];
				int vertex = dt.getIdx();
				Vec3f posO = dt.getOriginalPosition();
				Vec3f normalO = dt.getOriginalNormal();
				Vec3f newPos, newNormal;
				std::vector<BoneAssignment>& boneAssignment = dt.getBoneAssignments();
				for(unsigned int j = 0; j < boneAssignment.size(); ++j)
				{
					BoneAssignment& b = boneAssignment[j];
					int id = b._boneIdx;
					float weight = b._boneweight;
					Joint* joint = _pskeleton->getJoint(j);
					//Vec3f p = joint->->transformFromOriginalGlobalToGlobalPosition(posO);
					//Vec3f n = joint->transformFromLocalToGlobalOrienation(normalO);
					//newPos = newPos + p;
					//newNormal = newNormal + n;
				}
				newPos = newPos / boneAssignment.size();
				newNormal = newNormal / boneAssignment.size();
				newNormal.normalize();
				_vdata[vertex] = newPos;
				_ndata[vertex] = newNormal;
			}
			_updated = true;
		}

	};

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
		}

		virtual void release()
		{
			_vertices_index_face.clear();
			_normal_index_face.clear();
			_texcoord_index_face.clear();
			_vdata.clear();
			_ndata.clear();
			_tdata.clear();
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

		std::vector<int>& getOriginalVertexIndexForFaces(){return _vertices_index_face;}
		std::vector<int>& getOriginalNormalIndexForFaces(){return _normal_index_face;}
		std::vector<int>&  getOriginalTextureIndexForFaces(){return _texcoord_index_face;}
		std::vector<Vec3f>&  getOriginalVertices(){return _vdata;} 
		std::vector<Vec3f>&  getOriginalNormals(){return _ndata;}
		std::vector<Vec2f>& getOriginalTextureCoords(){return _tdata;}

		void setOriginalVertice(const std::vector<Vec3f>& data)
		{
			_vdata = data;
		}

		void setOriginalNormal(const std::vector<Vec3f>& data)
		{
			_ndata = data;
		}

		
		void setOriginalTextureCoords(const std::vector<Vec2f>& data)
		{
			_tdata = data;
		}

		void setOriginalVertexIndexForFaces(const std::vector<int>& data)
		{
			_vertices_index_face = data;
		}

		void initResource()
		{
			_skin._ndata = _ndata;
			_skin._vdata = _vdata;
			_skin._tdata = _tdata;
			_skin._updated = true;
		}
		Skin& getSkin(){return _skin;}
		void computeAABB()
		{
			_aabb.build(_skin._vdata);
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
		std::vector<int> _vertices_index_face;
		std::vector<int> _normal_index_face;
		std::vector<int> _texcoord_index_face;
		std::vector<Vec3f> _vdata, _ndata;
		std::vector<Vec2f> _tdata;
		std::string _name;
		Skin _skin;
	};
}