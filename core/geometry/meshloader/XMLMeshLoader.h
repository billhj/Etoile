/**
* Copyright(C) 2009-2012                
* @author Jing HUANG
* @file XMLMeshLoader.h
* @brief 
* @date 1/2/2011
*/

#pragma once
#include <string>
#include "tinyxml/tinyxml.h"
#include "../Mesh.h"

#include "MeshLoader.h"
namespace Etoile
{
	class XMLMeshLoader : public MeshLoader
	{
	public:
		XMLMeshLoader();
		~XMLMeshLoader();
		virtual bool loadFromFile(const std::string& fileName, Mesh* mesh) override;
	private:
		bool loadMeshes();
		bool loadPoses();
		void meshToUnitCube(SubMesh* submesh);
		void checkSpecifiedPose(const std::string& nameOfPose);
		void readSubMesh(TiXmlElement *elemSubMesh);
		void readFaces(TiXmlElement *elemFaces, SubMesh* submesh);
		void readGeometry(TiXmlElement *elemGeometry, SubMesh* submesh);
		void readBoneAssignment(TiXmlElement *elemBoneAssignment, SubMesh* submesh);
		void readPoses(TiXmlElement *elemPoses);
		void readAnimations(TiXmlElement *elemAnimations);
		void readTracks(TiXmlElement *elemTracks);

		TiXmlDocument* _pDoc;
	};

}
