/**
* Copyright(C) 2009-2012                
* @author Jing HUANG
* @file OBJMeshLoader.h
* @brief 
* @date 1/2/2011
*/

#pragma once
#define NOMINMAX
#include <algorithm>
#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <istream>
#include <deque>
#include "MeshLoader.h"
#include "../Mesh.h"

namespace Etoile
{


	struct Face
	{
		int _indexVertices[4];
		int _indexTextureCoordinates[4];
		int _indexNormals[4];
		int _vertexCount;
		int _materialIndex;
		bool _hasTextureCoordIndex;
		bool _hasNormalIndex;

		Face()
		{
			_hasTextureCoordIndex = false;
			_hasNormalIndex = false;
		}
	};


	class OBJMeshLoader : public MeshLoader
	{
	public:
		OBJMeshLoader(): MeshLoader(), _smoothThreshold(0.6f){}
		~OBJMeshLoader(){}
		virtual bool loadFromFile(const std::string& fileName, Mesh* mesh) override;

	protected:

		void handle_mtllib(std::stringstream& stream);

		std::string extractSubString(const std::string& input, int first, int last);

		Face handle_face(const std::string& lineString);

		int readMaterial(std::fstream& in);

		bool read(std::istream& in);

		std::vector<Face> triangulate(std::vector<Face> faces);

		void createConnectivities();
		void calculateFaceNormals();

		void meshToUnitCube();

		void trimString( std::string& _string) ;

		std::string _path;

		std::vector<Face> _faces;
		std::vector<Vec3f> _faceNormals;
		std::vector<std::vector<int> > _neighbourfacesIndices;
		std::vector<Vec3f> _vertices;
		std::vector<Vec3f> _normals;
		std::vector<Vec2f> _texcoords;
		double _smoothThreshold;

	};

}
