/**
* Copyright(C) 2009-2012                
* @author Jing HUANG
* @file OBJMeshLoader.cpp
* @brief 
* @date 1/2/2011
*/

#include "OBJMeshLoader.h"
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

	bool OBJMeshLoader::loadFromFile(const std::string& fileName, Mesh* mesh)
	{
		_fileName = fileName;
		_pMesh = mesh;
		std::fstream in( fileName.c_str(), std::ios_base::in );

		if (!in.is_open() || !in.good())
		{
			std::cerr << "[OBJReader] : cannot not open file "
				<< fileName
				<< std::endl;
			return false;
		}

		{
#if defined(WIN32)
			std::string::size_type dot = fileName.find_last_of("\\/");
#else
			std::string::size_type dot = fileName.rfind("/");
#endif
			_path = (dot == std::string::npos)
				? "./"
				: std::string(fileName.substr(0,dot+1));
		}

		bool result = read(in);

		in.close();
		return result;

	}


	void OBJMeshLoader::handle_mtllib(std::stringstream& stream)
	{
		std::string matFile;

		// Get the rest of the line, removing leading or trailing spaces
		// This will define the filename of the texture
		std::getline(stream,matFile);
		trimString(matFile);

		matFile = _path + matFile;

		std::cout << "Load material file " << matFile << std::endl;

		std::fstream matStream( matFile.c_str(), std::ios_base::in );

		int nb_material = 0;
		if ( matStream ){
			nb_material = readMaterial( matStream );
			if ( nb_material == 0 )
				std::cout << "  Warning! Could not read file properly!"<<std::endl;
			matStream.close();

		}else
			std::cout << "  Warning! Material file '" << matFile << "' not found!\n";

		std::cout << "  " << nb_material << " materials loaded."<<std::endl;
	}


	std::string OBJMeshLoader::extractSubString(const std::string& input, int first, int last)
	{
		std::string ret;
		for(int i = first; i <= last; ++i)
		{
			ret += input[i];
		}
		return ret;
	}

	Face OBJMeshLoader::handle_face(const std::string& lineString)
	{
		std::stringstream lineData(lineString);
		Face face;
		std::vector<std::string> components;
		while(!lineData.eof())
		{
			std::string tmp;
			lineData >> tmp;
			components.push_back(tmp);
		}
		face._vertexCount = components.size();

		for(unsigned int i = 0; i < components.size(); ++i)
		{
			const std::string& component = components[i];
			int pos0 = component.find("/");
			if( pos0 == -1)
			{
				int value;
				std::stringstream tmp( component );
				tmp >> value;
				face._indexVertices[i] = abs(value) - 1;
			}
			else
			{
				int pos1  = component.find("/", pos0 + 1);
				if( pos1 == -1)
				{
					std::string vertexString = extractSubString(component, 0, pos0 - 1);
					std::string textureCoordString = extractSubString(component, pos0 + 1, component.size() - 1);

					int valueVertex, valueTextureCoord;
					std::stringstream ssvertex(vertexString);
					ssvertex >> valueVertex;

					std::stringstream ssTextureCoord(textureCoordString);
					ssTextureCoord >> valueTextureCoord;

					face._indexVertices[i] = abs(valueVertex) - 1;
					face._indexTextureCoordinates[i] = abs(valueTextureCoord) - 1;
					face._hasTextureCoordIndex = true;
				}
				else if( (pos1 - pos0) == 1) // Vertex/normal case
				{
					std::string vertexString = extractSubString(component, 0, pos0 - 1);
					std::string normalString = extractSubString(component, pos0 + 2, component.size() - 1);

					int valueVertex, valueNormal;
					std::stringstream ssvertex(vertexString);
					ssvertex >> valueVertex;
					std::stringstream ssnormal(normalString);
					ssnormal >> valueNormal;

					face._indexVertices[i] = abs(valueVertex) - 1;
					face._indexNormals[i] = abs(valueNormal) - 1;
					face._hasNormalIndex = true;
				}
				else
				{
					std::string vertexString = extractSubString(component, 0, pos0 - 1);
					std::string textureCoordString = extractSubString(component, pos0 + 1, pos1 - 1);
					std::string normalString = extractSubString(component, pos1 + 1, component.size() - 1);

					int valueVertex, valueTextureCoord, valueNormal;
					std::stringstream ssvertex(vertexString);
					ssvertex >> valueVertex;
					std::stringstream ssnormal(normalString);
					ssnormal >> valueNormal;
					std::stringstream ssTextureCoord(textureCoordString);
					ssTextureCoord >> valueTextureCoord;

					face._indexVertices[i] = abs(valueVertex) - 1;
					face._indexTextureCoordinates[i] = abs(valueTextureCoord) - 1;
					face._hasTextureCoordIndex = true;
					face._indexNormals[i] = abs(valueNormal) - 1;
					face._hasNormalIndex = true;

				}
			}

		}

		return face;
	}

	int OBJMeshLoader::readMaterial(std::fstream& in)
	{

		std::string line;
		std::string keyWrd;
		//std::string textureName;

		std::string key;
		double       f1,f2,f3;
		int nb_material = 0;

		std::string currentMat;

		while( in && !in.eof() )
		{
			std::getline(in,line);
			if ( in.bad() ){
				std::cout << "  Warning! Could not read file properly!"<<std::endl;
				return 0;
			}

			if ( line.empty() )
				continue;

			std::stringstream stream(line);

			stream >> keyWrd;


			// oldcode:
			//if( isspace(line[0]) || line[0] == '#' )
			// isspace might be a tab at the beginning of the line...
			// ignore it
			if( /*( isspace(line[0]) && line[ 0 ] != 0x09 ) ||*/ line[0] == '#' || line.empty())
			{
				if (!key.empty())
				{
					/*_materials[key] = mat;
					mat.cleanup();*/
				}
			}
			else if (keyWrd == "newmtl") // begin new material definition
			{
				stream >> key;
				currentMat = key;
				_materials.push_back(new Material(currentMat));
				Material* mat = _materials.back();
				_materialNameMap[currentMat] = _materials.size() - 1;
				mat->setDiffuseTexture(_pTextureLoader->loadFromFile("emptyMap"));
				mat->setSpecularTexture( _pTextureLoader->loadFromFile("emptyMap"));

				nb_material++;
			}

			else if (keyWrd == "Kd") // diffuse color
			{
				Material* mat = _materials.back();
				stream >> f1; stream >> f2; stream >> f3;

				if( !stream.fail() )
				{
					mat->setDiffuse(f1,f2,f3,1);
					mat->setDiffuseConstant(1);
				}
			}

			else if (keyWrd == "Ka") // ambient color
			{
				Material* mat = _materials.back();
				stream >> f1; stream >> f2; stream >> f3;

				if( !stream.fail() )
				{
					mat->setAmbient(f1,f2,f3,1);
					mat->setAmbientConstant(1);
				}
			}

			else if (keyWrd == "Ks") // specular color
			{
				Material* mat = _materials.back();
				stream >> f1; stream >> f2; stream >> f3;

				if( !stream.fail() )
				{
					mat->setSpecular(f1,f2,f3,1);
					mat->setSpecularConstant(1);
				}
			}

			else if (keyWrd == "illum") // diffuse/specular shading model
			{
				; // just skip this
			}

			else if (keyWrd == "Ns") // Shininess [0..200]
			{
				//            Material& mat = _materials.back();
				//            stream >> f1;
				//            if( !stream.fail() )
				//            {
				//                mat.setSpecularShininess(f1);
				//            } // just skip this
			}

			else if (keyWrd == "map_") // map images
			{
				// map_Ks, specular map
				// map_Ka, ambient map
				// map_Bump, bump map
				// map_d,  opacity map
				; // just skip this
			}

			else if (keyWrd == "map_Kd" ) {
				// Get the rest of the line, removing leading or trailing spaces
				// This will define the filename of the texture
				Material* mat = _materials.back();
				std::string textureName;
				std::getline(stream,textureName);
				trimString(textureName);

				size_t pos = textureName.find("\\");
				if(pos != textureName.npos)
				{
					textureName[pos] = '/';
				}

				if ( ! textureName.empty() )
				{
					std::string s = _path + textureName;
					mat->setDiffuseTexture(_pTextureLoader->loadFromFile(s));
					_texturePathMap[textureName] = s;
				}else
				{
					mat->setDiffuseTexture(_pTextureLoader->loadFromFile("emptyMap"));
				}
			}
			else if (keyWrd == "map_Ks") // map images
			{
				Material* mat = _materials.back();
				std::string textureName;
				std::getline(stream,textureName);
				trimString(textureName);
				if ( ! textureName.empty() )
				{
					std::string s = _path + textureName;
					mat->setSpecularTexture(_pTextureLoader->loadFromFile(s));
					_texturePathMap[textureName] = s;

				}else
				{
					mat->setSpecularTexture(_pTextureLoader->loadFromFile("emptyMap"));
				}
			}
			else if (keyWrd == "map_bump") // map images
			{
				Material* mat = _materials.back();
				std::string textureName;
				//std::getline(stream,textureName);
				//trimString(textureName);
				std::string bm; stream>>bm;
				double nb; stream>>nb;
				stream>>textureName;
				if ( ! textureName.empty() )
				{
					std::string s = _path + textureName;
					mat->setBumpMap(_pTextureLoader->loadFromFile(s));
					_texturePathMap[textureName] = s;

				}else
				{
					mat->setBumpMap( _pTextureLoader->loadFromFile("emptyMap"));
				}
			}
			else if (keyWrd == "Tr") // transparency value
			{
				Material* mat = _materials.back();
				stream >> f1;
				if( !stream.fail() )
				{
					mat->setTransparency(f1);
				}
			}
			else if (keyWrd == "d") // transparency value
			{
				Material* mat = _materials.back();
				stream >> f1;
				if( !stream.fail() )
				{

					mat->setDissolvedOpacity(f1);
				}
			}

			//if ( in && indef /*&& mat.is_valid()*/ && !key.empty())
			//{
			//	_materialMap["default"];
			//	nb_material++;
			//}

		}
		_texturePathMap["emptyMap"] = "emptyMap";
		return nb_material;
	}

	std::vector<Face> OBJMeshLoader::triangulate(std::vector<Face> faces)
	{
		std::vector<Face> newfaces;
		for(unsigned int i = 0; i < faces.size(); ++i)
		{
			Face& face = faces[i];
			const int triangleIndices[][3] = {{0, 1, 2}, {0, 2, 3}};
			const int triangleCount = (face._vertexCount > 3) ? 2 : 1;
			if(triangleCount > 1)
			{
				for(int triangle = 0; triangle < triangleCount; ++triangle )
				{

					Face newface;
					newface._materialIndex = face._materialIndex;
					newface._vertexCount = 3;
					for(unsigned int j = 0; j < 3; ++j)
					{
						int vIndex = triangleIndices[triangle][j];
						newface._indexVertices[j] = face._indexVertices[vIndex];

						if(face._hasNormalIndex)
						{
							newface._indexNormals[j] = face._indexNormals[vIndex];
							newface._hasNormalIndex = face._hasNormalIndex;
						}

						if(face._hasTextureCoordIndex)
						{
							newface._indexTextureCoordinates[j] = face._indexTextureCoordinates[vIndex];
							newface._hasTextureCoordIndex = face._hasTextureCoordIndex;
						}
					}
					newfaces.push_back(newface);
				}

			}
			else
			{
				newfaces.push_back(face);
			}
		}
		return newfaces;
	}

	void OBJMeshLoader::createConnectivities()
	{
		_neighbourfacesIndices.resize(_vertices.size());
		for(unsigned int i = 0; i < _faces.size(); ++i)
		{
			Face& face = _faces[i];
			for(int j = 0; j < face._vertexCount; ++j)
			{
				int index = face._indexVertices[j];
				//std::cout<<"index" <<index<<std::endl;
				std::vector<int>& indices = _neighbourfacesIndices.at(index);
				indices.push_back(i);
			}
		}

	}


	Vec3f calculateNormal(Vec3f v0, Vec3f v1, Vec3f v2)
	{
		Vec3f edge0 = v1 - v0;
		Vec3f edge1 = v2 - v0;
		Vec3f n;
		Vec3f::cross3(n, edge0, edge1);
		n.normalize();
		return n;
	}

	void OBJMeshLoader::calculateFaceNormals()
	{
		for(unsigned int i = 0; i < _faces.size(); ++i)
		{
			Face& face = _faces[i];

			int i0 = face._indexVertices[0];
			int i1 = face._indexVertices[1];
			int i2 = face._indexVertices[2];
			Vec3f v0 = _vertices[i0];
			Vec3f v1 = _vertices[i1];
			Vec3f v2 = _vertices[i2];
			_faceNormals.push_back(calculateNormal(v0, v1, v2));
		}
	}

	bool OBJMeshLoader::read(std::istream& in)
	{


		std::cout << "[OBJReader] : read file"<<std::endl;


		std::string line;
		std::string keyWrd;
		double x, y, z, u, v;


		std::string            matname;
		int nb_face = 0;

		int materialIndex = -1;

		int lineIndex = 0;

		while( in && !in.eof() )
		{
			lineIndex++;
			if( lineIndex % 10000 == 0)
				std::cout << /*lineIndex*/ "."<<std::flush;

			std::getline(in,line);
			if ( in.bad() ){
				std::cout << "  Warning! Could not read file properly!"<<std::endl;
				return false;
			}

			// Trim Both leading and trailing spaces
			trimString(line);

			// comment
			if ( line.size() == 0 || line[0] == '#' || isspace(line[0]) || line.empty() ) {
				continue;
			}

			std::stringstream stream(line);

			stream >> keyWrd;


			// material file
			if (keyWrd == "mtllib")
			{
				handle_mtllib(stream);
			}
			// usemtl
			else if (keyWrd == "usemtl")
			{
				stream >> matname;
				std::map<std::string, int>::iterator itor = _materialNameMap.find(matname);
				if(itor != _materialNameMap.end())
				{
					materialIndex = itor->second;
				}else{
					materialIndex = -1;
				}
			}
			//PB is the files for materials, using usemtl or not, the obj file is not uniform
			else if ( keyWrd == "g")
			{
				/*std::string objectName;
				stream >> objectName;
				materialIndex = -1;
				if(!objectName.empionty())
				{
				std::cout << "found object: " << objectName << std::endl;
				}*/
			}

			// vertex
			else if (keyWrd == "v")
			{

				stream >> x; stream >> y; stream >> z;

				if ( !stream.fail() )
					_vertices.push_back(Vec3f(x,y,z));
				else{
					std::cout << " vertex error"<< std::endl;
					return false;
				}
				//            if( (_vertices.size() + 1 )% 10000 == 0)
				//                std::cout << "Vertex count : " <<_vertices.size()<<std::endl;

			}

			// texture coord
			else if (keyWrd == "vt")
			{
				stream >> u; stream >> v;

				if ( !stream.fail() ){

					_texcoords.push_back(Vec2f(u, v));

				}else{

					std::cout << "Only single 2D texture coordinate per vertex"
						<< "allowed!" << std::endl;
					return false;
				}
				//            if( (_texcoords.size() + 1 )% 10000 == 0)
				//                std::cout << "Tex count : " <<_texcoords.size()<<std::endl;
			}


			// normal
			else if (keyWrd == "vn")
			{
				stream >> x; stream >> y; stream >> z;

				if ( !stream.fail() ){
					_normals.push_back(Vec3f(x,y,z));
				}else{

					std::cout << "normal vector error"<< std::endl;
					return false;
				}
				//            if( (_normals.size() + 1 )% 10000 == 0)
				//                std::cout << "Normal count : " <<_normals.size()<<std::endl;
			}


			// face
			else if (keyWrd == "f")
			{
				Face face;
				nb_face++;
				// read full line after detecting a face
				std::string faceLine;
				std::getline(stream,faceLine);
				face = handle_face(faceLine);
				face._materialIndex = materialIndex;
				_faces.push_back(face);
				//            if( (_faces.size() + 1 ) % 10000 == 0)
				//                std::cout << "Face count : " <<_faces.size()<<std::endl;

			}

		}
		std::cout << std::endl<<
			"finish reading : "<<
			"Vertex count : " <<_vertices.size()<<" "<<
			"Tex count : " <<_texcoords.size()<<" "<<
			"Normal count : " <<_normals.size()<<" "<<
			"Face count : " <<_faces.size()<<std::endl;

		std::vector<Etoile::SubMesh*> submeshes;

		for( unsigned int i = 0; i < _materials.size() + 1; ++i)
		{
			if(i==0)
			{
				SubMesh* submesh = _pMesh->creatNewSubMesh("empty");
				submeshes.push_back(submesh);
				Material* mat = new Material("empty");
				mat->setDiffuseTexture(_pTextureLoader->loadFromFile("emptyMap"));
				mat->setSpecularTexture( _pTextureLoader->loadFromFile("emptyMap"));
				submesh->setMaterial(mat);
			}
			else
			{
				SubMesh* submesh =  _pMesh->creatNewSubMesh(_materials[i - 1]->getName());
				submesh->setMaterial(_materials[i - 1]);
				submeshes.push_back(submesh);
			}
		}
		std::cout<<"submesh created" <<std::endl;

		_faces = triangulate(_faces);
		std::cout<<"Triangulation done" <<std::endl;

		if(_normals.size() == 0)
		{
			createConnectivities();
			std::cout<<"Connectivities done" <<std::endl;

			calculateFaceNormals();
			std::cout<<"face normals done" <<std::endl;
		}

		for(unsigned int i = 0; i < _faces.size(); ++i)
		{
			const Face& face = _faces[i];
			SubMesh* submesh = submeshes[face._materialIndex + 1];
			if(face._vertexCount == 0) continue;

			for(unsigned int j = 0; j < 3; ++j)
			{

				int indexV = face._indexVertices[j];
				submesh->getVertices().push_back(_vertices[indexV]);
				submesh->addVertexIndex(submesh->getVertices().size() - 1);

				if(face._hasNormalIndex)
				{
					submesh->getNormals().push_back(_normals[face._indexNormals[j]]);
				}
				else
				{

					Vec3f currentVertexNormal(0.0f, 0.0f, 0.0f);
					Vec3f originalFaceNormal = _faceNormals[i];
					std::vector<int> neighourfaces = _neighbourfacesIndices[indexV];
					int count = 1;
					for(unsigned int faceIt = 0; faceIt < neighourfaces.size(); ++faceIt)
					{
						int faceIndex = neighourfaces[faceIt];
						Face& faceItor = _faces[faceIndex];
						Vec3f faceNormal = _faceNormals[faceIndex];
						if(originalFaceNormal.dot3(faceNormal) > _smoothThreshold &&  face._materialIndex== faceItor._materialIndex)
						{
							currentVertexNormal += faceNormal;
							++count;
						}
					}
					currentVertexNormal.normalize();
					submesh->getNormals().push_back(currentVertexNormal);
				}

				submesh->addNormalIndex(submesh->getNormals().size() - 1);


				if(face._hasTextureCoordIndex)
				{
					submesh->getTextureCoords().push_back(_texcoords[face._indexTextureCoordinates[j]]);
				}
				else
				{
					submesh->getTextureCoords().push_back(Vec2f());
				}
				submesh->addTextureCoordinateIndex(submesh->getTextureCoords().size() - 1);
			}

		}

		std::cout<<"submesh data recreated" <<std::endl;

		//MxAssert(ndata.size() == tdata.size() && ndata.size() == vdata.size());
		_pMesh->cleanEmptySubMesh();
		meshToUnitCube();
		//_pMesh->buildRuntimeData();
		_pMesh->initResource();
		_pMesh->computeAABB();
		return true;
	}



	void OBJMeshLoader::meshToUnitCube()
	{
		Vec3f minPos(1e6, 1e6, 1e6);
		Vec3f maxPos(-1e6, -1e6, -1e6);
		std::vector<SubMesh*>& list = _pMesh->getSubMeshList();
		std::vector<SubMesh*>::iterator itor;
		for(itor = list.begin(); itor != list.end(); ++itor){
			SubMesh* submesh = (*itor);
			for (unsigned int i = 0; i < submesh->getVertices().size(); ++i)
			{
				const Vec3f& p = submesh->getVertices()[i];
				for (int i = 0; i < 3; ++i)
				{
					if (p[i] > maxPos[i]) maxPos[i] = p[i];
					if (p[i] < minPos[i]) minPos[i] = p[i];
				}
			}
		}

		// find the longest edge and scale it to 1
		//double scale = 1.0f / std::max( std::max(maxPos[0] - minPos[0], maxPos[1] - minPos[1]),  maxPos[2] - minPos[2]);
		double scale = 1.0f / (maxPos - minPos).length();
		Vec3f center((maxPos[0] + minPos[0]) / 2.0f,
			(maxPos[1] + minPos[1]) / 2.0f,
			(maxPos[2] + minPos[2]) / 2.0f);

		for(itor = list.begin(); itor != list.end(); ++itor){
			SubMesh* submesh = (*itor);
			for (unsigned int i = 0; i < submesh->getVertices().size(); ++i)
			{
				Vec3f& p = submesh->getVertices()[i];
				p -= center;
				p *= scale;
			}
		}

	}

	void OBJMeshLoader::trimString( std::string& _string) {
		// Trim Both leading and trailing spaces

		size_t start = _string.find_first_not_of(" \t\r\n");
		size_t end   = _string.find_last_not_of(" \t\r\n");

		if(( std::string::npos == start ) || ( std::string::npos == end))
			_string = "";
		else
			_string = _string.substr( start, end-start+1 );
	}

}

