/**
* Copyright(C) 2009-2012                
* @author Jing HUANG
* @file Mesh.h
* @brief 
* @date 1/2/2011
*/

#pragma once
#include <string>
#include <vector>
#include "math/MathHead.h"
#include "SubMesh.h"

#ifdef USING_BOOST
#include <boost/serialization/serialization.hpp>
#include <boost/archive/binary_oarchive.hpp>
#include <boost/archive/binary_iarchive.hpp>
#include <boost/archive/text_oarchive.hpp>
#include <boost/archive/text_iarchive.hpp>
#include <boost/serialization/map.hpp>
#endif

namespace Etoile
{
	class Mesh
	{
	public:
		Mesh(const std::string& name): _name(name), _numberOfFaces(0)
		{
		}
	

		Mesh(Mesh& m)
		{
			_name = m._name;
			_subMeshList = m._subMeshList;
			_numberOfFaces = m._numberOfFaces;
		}

		void copy(Mesh& m)
		{
			_name = m._name;
			for(int i = 0; i < m._subMeshList.size(); ++i)
			{
				_subMeshList.push_back(new SubMesh(*m._subMeshList[i]));
			}
			_numberOfFaces = m._numberOfFaces;
			initResource();
		}

		//virtual ~Mesh(){}
		const std::string getName(){return _name;}
		void setName(const std::string& name){ _name = name;}

		virtual void release()
		{
			for(unsigned int i = 0; i < _subMeshList.size(); ++i)
			{
				delete _subMeshList[i];
			}
			_subMeshList.clear();
		}

		void cleanEmptySubMesh()
		{
			std::vector<SubMesh*>::iterator itor;
			std::vector<SubMesh*> newlist;
			for(itor = _subMeshList.begin(); itor != _subMeshList.end(); ++itor)
			{
				newlist.push_back(*itor);
			}
			_subMeshList.clear();
			for(itor = newlist.begin(); itor != newlist.end(); ++itor)
			{
				SubMesh* submesh = (*itor);
				if(submesh->getVertexIndexForFaces().empty())
				{
					delete submesh;
				}else
				{
					_subMeshList.push_back(submesh);
				}
			}
		}

		int getNumberOfFaces() const {return _numberOfFaces;}
		unsigned int addSubMesh(SubMesh* sub)
		{
			_subMeshList.push_back(sub);
			return _subMeshList.size() - 1;
		}
		SubMesh* creatNewSubMesh(const std::string& name = "")
		{
			SubMesh* submesh = new SubMesh(name);
			_subMeshList.push_back(submesh);
			return submesh;
		}

		SubMesh* getSubMesh(int idx)
		{
			return _subMeshList[idx];
		}

		std::vector<SubMesh*>& getSubMeshList(){return _subMeshList;}

		virtual void initResource()
		{
			computeAABB();
		}


		void computeAABB()
		{
			std::vector<AxisAlignedBoundingBoxf*> aabbs;
			for(unsigned int i = 0; i < _subMeshList.size(); ++i)
			{
				_subMeshList[i]->computeAABB();
				aabbs.push_back(_subMeshList[i]->getAABB());
			}
			_aabb.build(aabbs);
		}

		AxisAlignedBoundingBoxf* getAABB(){return &_aabb;}

		virtual void perform(Matrix4f& transform){}

#ifdef USING_BOOST
		friend class boost::serialization::access;

		template<class Archive>
		void serialize(Archive & ar, const unsigned int version)
		{
			ar & _name;
			ar & _numberOfFaces;
			ar & _subMeshList;
		}
#endif
		AxisAlignedBoundingBoxf _aabb;
		int _numberOfFaces;
		std::vector<SubMesh*> _subMeshList;
		std::string _name;		
	};

}
