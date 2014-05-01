/**
* Copyright(C) 2009-2012                
* @author Jing HUANG
* @file XMLMeshLoader.cpp
* @brief 
* @date 1/2/2011
*/

#include "XMLMeshLoader.h"

/**
* @brief For tracking memory leaks under windows using the crtdbg
*/
#if ( defined( _DEBUG ) || defined( DEBUG ) ) && defined( _MSC_VER )
#define _CRTDBG_MAP_ALLOC
#include <crtdbg.h>

#define DEBUG_NEW new( _NORMAL_BLOCK, __FILE__, __LINE__ )
#define new DEBUG_NEW
#endif

namespace Etoile{
	XMLMeshLoader::XMLMeshLoader(): MeshLoader(),_pDoc(0)
	{

	}

	XMLMeshLoader::~XMLMeshLoader()
	{
		delete _pDoc;
	}


	bool XMLMeshLoader::loadFromFile(const std::string& fileName, Mesh* mesh)
	{
		_fileName = fileName;
		_pMesh = mesh;
		_pDoc = new TiXmlDocument(_fileName.c_str());


		if(!_pDoc->LoadFile()){
			std::cout << "erreur while loading" << std::endl;
			std::cout << "error #" << _pDoc->ErrorId() << " : " << _pDoc->ErrorDesc() << std::endl;
			return false;
		}

		bool b = loadMeshes();
		//_pMesh->createVBO(usage, eInternalType);
		//meshToUnitCube();
		//creatGpuData();
		_pMesh->initResource();
		_pMesh->computeAABB();
		return b;
	}

	void XMLMeshLoader::readSubMesh(TiXmlElement *elemSubMesh)
	{

		std::string material = elemSubMesh->Attribute("material");
		//std::string usesharedvertices = elemSubMesh->Attribute("usesharedvertices");
		//std::string use32bitindexes = elemSubMesh->Attribute("use32bitindexes");
		//std::string operationtype = elemSubMesh->Attribute("operationtype");
		//Material* m = new Material(material);
		SubMesh* submesh = _pMesh->creatNewSubMesh(material);
		//_pMesh->_materialGroup.push_back(m);
#if defined(_DEBUG) || defined(DEBUG)
		std::cout<<"read faces start"<<std::endl;
#endif

		TiXmlElement *elemFaces = elemSubMesh->FirstChildElement("faces");
		while (elemFaces)
		{			
			readFaces(elemFaces, submesh);
			elemFaces = elemFaces->NextSiblingElement("faces"); // iteration
		}
#if defined(_DEBUG) || defined(DEBUG)
		std::cout<<"read faces end"<<std::endl;
#endif

		TiXmlElement *elemGeometry = elemSubMesh->FirstChildElement("geometry");
		while (elemGeometry)
		{			
			readGeometry(elemGeometry, submesh);
			elemGeometry = elemGeometry->NextSiblingElement("geometry"); // iteration
		}	
#if defined(_DEBUG) || defined(DEBUG)
		std::cout<<"read Geometry end"<<std::endl;
#endif

		TiXmlElement *elemBoneAssignment = elemSubMesh->FirstChildElement("boneassignments");
		while (elemBoneAssignment)
		{			
			readBoneAssignment(elemBoneAssignment, submesh);
			elemBoneAssignment = elemBoneAssignment->NextSiblingElement("boneassignments"); // iteration
		}	

		//meshToUnitCube(submesh);

	}

	void XMLMeshLoader::readFaces(TiXmlElement *elemFaces, SubMesh* submesh)
	{
		int count = 0;
		int returnvalue = elemFaces->QueryIntAttribute("count", &count);
		if( !returnvalue == TIXML_SUCCESS)
			return;
		submesh->_numberOfFaces = count;
		int size = count * 3;
		submesh->getVertexIndexForFaces().resize(size);

		TiXmlElement *elemFace = elemFaces->FirstChildElement("face");
		int n = 0;
		while (elemFace)
		{
			int v1 = 0, v2 = 0, v3 = 0;
			elemFace->QueryIntAttribute("v1", &v1);
			elemFace->QueryIntAttribute("v2", &v2);
			elemFace->QueryIntAttribute("v3", &v3);
			submesh->getVertexIndexForFaces()[n * 3] = v1;
			submesh->getVertexIndexForFaces()[n * 3 + 1] = v2;
			submesh->getVertexIndexForFaces()[n * 3 + 2] = v3;
			n++;
			elemFace = elemFace->NextSiblingElement("face"); // iteration
		}

	}


	void XMLMeshLoader::readGeometry(TiXmlElement *elemGeometry, SubMesh* submesh)
	{

		int vertexcount = 0;
		int returnvalue = elemGeometry->QueryIntAttribute("vertexcount", &vertexcount);
		if(returnvalue != TIXML_SUCCESS)
			return;
		submesh->getVertices().resize(vertexcount); 
		submesh->getNormals().resize(vertexcount);
		submesh->getTextureCoords().resize(vertexcount); 
		TiXmlElement *elemvertexbuffer = elemGeometry->FirstChildElement("vertexbuffer");
		while (elemvertexbuffer)
		{
			const char * positionsC = elemvertexbuffer->Attribute("positions");
			const char * normalsC = elemvertexbuffer->Attribute("normals");
			std::string positions, normals;
			if(positionsC != NULL)
				positions = positionsC;
			if(normalsC != NULL)
				normals = normalsC;

			int texture_coord_dimensions_0 = 0;
			int returnvalue = elemvertexbuffer->QueryIntAttribute("texture_coord_dimensions_0", &texture_coord_dimensions_0);
			int texture_coords = 0;
			int returnvalue1 = elemvertexbuffer->QueryIntAttribute("texture_coords", &texture_coords);

			if(positions=="true")
			{
#if defined(_DEBUG) || defined(DEBUG)
				std::cout<<"read positions start"<<std::endl;
#endif

				TiXmlElement *elemvertex = elemvertexbuffer->FirstChildElement("vertex");
				int n = 0;
				while (elemvertex)
				{
					TiXmlElement *elemposition = elemvertex->FirstChildElement("position");

					while (elemposition)
					{
						double x = 0, y = 0, z = 0;
						elemposition->QueryDoubleAttribute("x", &x);
						elemposition->QueryDoubleAttribute("y", &y);
						elemposition->QueryDoubleAttribute("z", &z);

						Vec3f xyz(x*_scale.x(),y*_scale.y(),z*_scale.z());
						submesh->getVertices()[n] = _rotate * xyz + _translate;
						n++;
						elemposition = elemposition->NextSiblingElement("position"); // iteration
					}

					elemvertex = elemvertex->NextSiblingElement("vertex"); // iteration
				}


#if defined(_DEBUG) || defined(DEBUG)
				std::cout<<"read positions end"<<std::endl;
#endif


			}
			if(normals=="true")
			{

#if defined(_DEBUG) || defined(DEBUG)
				std::cout<<"read normals start"<<std::endl;
#endif


				TiXmlElement *elemvertex = elemvertexbuffer->FirstChildElement("vertex");
				int n = 0;
				while (elemvertex)
				{
					TiXmlElement *elemnormal = elemvertex->FirstChildElement("normal");

					while (elemnormal)
					{
						double x = 0, y = 0, z = 0;
						elemnormal->QueryDoubleAttribute("x", &x);
						elemnormal->QueryDoubleAttribute("y", &y);
						elemnormal->QueryDoubleAttribute("z", &z);
						Vec3f xyz(x,y,z);
						submesh->getNormals()[n] = xyz;
						n++;
						elemnormal = elemnormal->NextSiblingElement("normal"); // iteration
					}
					elemvertex = elemvertex->NextSiblingElement("vertex"); // iteration
				}



#if defined(_DEBUG) || defined(DEBUG)
				std::cout<<"read normals end"<<std::endl;
#endif


			}
			if(returnvalue==TIXML_SUCCESS && returnvalue1==TIXML_SUCCESS)
			{

#if defined(_DEBUG) || defined(DEBUG)
				std::cout<<"read textureCoord start"<<std::endl;
#endif

				TiXmlElement *elemvertex = elemvertexbuffer->FirstChildElement("vertex");
				int n = 0;
				while (elemvertex)
				{
					TiXmlElement *elemtexcoord = elemvertex->FirstChildElement("texcoord");

					while (elemtexcoord)
					{
						double u = 0, v = 0;

						elemtexcoord->QueryDoubleAttribute("u", &u);
						elemtexcoord->QueryDoubleAttribute("v", &v);
						Vec2f uv(u,v);
						submesh->getTextureCoords()[n] = uv;
						n++;

						elemtexcoord = elemtexcoord->NextSiblingElement("texcoord"); // iteration
					}
					elemvertex = elemvertex->NextSiblingElement("vertex"); // iteration
				}


#if defined(_DEBUG) || defined(DEBUG)
				std::cout<<"read textureCoord end"<<std::endl;
#endif


			}


			elemvertexbuffer = elemvertexbuffer->NextSiblingElement("vertexbuffer"); // iteration
		}

	}


	void XMLMeshLoader::readBoneAssignment(TiXmlElement *elemBoneAssignment, SubMesh* submesh)
	{

		TiXmlElement *vertexboneassignment = elemBoneAssignment->FirstChildElement("vertexboneassignment");

		//if(vertexboneassignment)
		//	submesh->getSkin()._vertexBoneAssignmentData.resize(submesh->getVertices().size());

		//while (vertexboneassignment)
		//{			
		//	int vertexIndex = -1;
		//	vertexboneassignment->QueryIntAttribute("vertexindex", &vertexIndex);
		//	int boneIndex = -1;
		//	vertexboneassignment->QueryIntAttribute("boneindex", &boneIndex);
		//	double weight = 0;
		//	vertexboneassignment->QueryDoubleAttribute("weight", &weight);

		//	if(vertexIndex < 0) continue;
		//	submesh->getSkin()._vertexBoneAssignmentData[vertexIndex].setIdx(vertexIndex);
		//	submesh->getSkin()._vertexBoneAssignmentData[vertexIndex].addBoneWeight(boneIndex, weight);
		//	submesh->getSkin()._vertexBoneAssignmentData[vertexIndex].setPosition(submesh->getVertices()[vertexIndex]);

		//	vertexboneassignment = vertexboneassignment->NextSiblingElement("vertexboneassignment"); // iteration
		//}
	}


	void XMLMeshLoader::readPoses(TiXmlElement *elemPoses)
	{
		TiXmlElement *elemPose = elemPoses->FirstChildElement("pose");
		while (elemPose)
		{
			std::string target = elemPose->Attribute("target");
			std::string name = elemPose->Attribute("name");
			//int index = 0;
			//int returnvalue = elemPose->QueryIntAttribute("index", &index);
			//AnimatedMesh* animatedMesh = dynamic_cast<AnimatedMesh*>(_pMesh);
			//Pose* pose = new Pose(index, name,target);
			this->checkSpecifiedPose(name);
			//		animatedMesh->_poses.push_back(pose);
			TiXmlElement *elemPoseoffset = elemPose->FirstChildElement("poseoffset");
			while (elemPoseoffset)
			{
				int index;
				double x, y, z;
				elemPoseoffset->QueryIntAttribute("index", &index);
				elemPoseoffset->QueryDoubleAttribute("x", &x);
				elemPoseoffset->QueryDoubleAttribute("y", &y);
				elemPoseoffset->QueryDoubleAttribute("z", &z);
				//			pose->addOffset(index, Vec3f(x,y,z));
				elemPoseoffset = elemPoseoffset->NextSiblingElement("poseoffset"); // iteration
			}

			elemPose = elemPose->NextSiblingElement("pose"); // iteration
		}

	}

	void XMLMeshLoader::readAnimations(TiXmlElement *elemAnimations)
	{
		TiXmlElement *elemAnimation = elemAnimations->FirstChildElement("animation");
		while (elemAnimation)
		{

			TiXmlElement *elemTracks = elemAnimation->FirstChildElement("tracks");
			while (elemTracks)
			{
				readTracks(elemTracks);
				elemTracks = elemTracks->NextSiblingElement("tracks"); // iteration
			}

			elemAnimation = elemAnimation->NextSiblingElement("animation"); // iteration
		}
	}

	void XMLMeshLoader::readTracks(TiXmlElement *elemTracks)
	{
		TiXmlElement *elemTrack = elemTracks->FirstChildElement("track");
		while (elemTrack)
		{
			std::string target = elemTrack->Attribute("target");
			std::string type = elemTrack->Attribute("type");
			int index = 0 ;
			elemTrack->QueryIntAttribute("index", &index);


			TiXmlElement *elemKeyframes = elemTrack->FirstChildElement("keyframes");
			while (elemKeyframes)
			{
				elemKeyframes = elemKeyframes->NextSiblingElement("keyframes"); // iteration
			}

			elemTrack = elemTrack->NextSiblingElement("track"); // iteration
		}

	}


	void XMLMeshLoader::meshToUnitCube(SubMesh* submesh)
	{
		Vec3f min(1e6, 1e6, 1e6);
		Vec3f max(-1e6, -1e6, -1e6);

		for (unsigned int j = 0; j < submesh->getVertices().size(); j++)
		{
			Vec3f p = submesh->getVertices()[j];
			for (int i = 0; i < 3; ++i)
			{
				if (p[i] > max[i]) max[i] = p[i];
				if (p[i] < min[i]) min[i] = p[i];
			}
		}

		// find the longest edge and scale it to 1
		double scale = 1.0f / std::max(std::max(max[0] - min[0], max[1] - min[1]), max[2] - min[2]);

		Vec3f center((max[0] + min[0]) / 2.0f,
			(max[1] + min[1]) / 2.0f,
			(max[2] + min[2]) / 2.0f);

		for (unsigned int j = 0; j < submesh->getVertices().size(); j++)
		{
			submesh->getVertices()[j] -= center;
			submesh->getVertices()[j] *= scale;
		}
	}


	bool XMLMeshLoader::loadMeshes()
	{

		TiXmlHandle hdl(_pDoc);
		TiXmlElement *elemRoot = hdl.FirstChildElement("mesh").Element();
		while (elemRoot)
		{

			TiXmlElement *elemSubmeshes = elemRoot->FirstChildElement("submeshes");
			while (elemSubmeshes)
			{
				TiXmlElement *elemSubMesh = elemSubmeshes->FirstChildElement("submesh");

				while(elemSubMesh){
					readSubMesh(elemSubMesh);
					elemSubMesh = elemSubMesh->NextSiblingElement("submesh");
				}

				elemSubmeshes = elemSubmeshes->NextSiblingElement("submeshes"); // iteration
			}


			elemRoot = elemRoot->NextSiblingElement("mesh"); // iteration
		}

		return true;
	}

	bool XMLMeshLoader::loadPoses()
	{

		TiXmlHandle hdl(_pDoc);
		TiXmlElement *elemRoot = hdl.FirstChildElement("mesh").Element();

#if defined(_DEBUG) || defined(DEBUG)
		std::cout<<"read poses start"<<std::endl;
#endif

		while (elemRoot)
		{
			TiXmlElement *elemPoses = elemRoot->FirstChildElement("poses");
			while (elemPoses)
			{
				readPoses(elemPoses);
				elemPoses = elemPoses->NextSiblingElement("poses"); // iteration
			}


			TiXmlElement *elemAnimations = elemRoot->FirstChildElement("animations");
			while (elemAnimations)
			{
				readAnimations(elemAnimations);
				elemAnimations = elemAnimations->NextSiblingElement("animations"); // iteration
			}


			elemRoot = elemRoot->NextSiblingElement("mesh"); // iteration
		}

#if defined(_DEBUG) || defined(DEBUG)
		std::cout<<"read poses end"<<std::endl;
#endif
		return true;

	}


	void XMLMeshLoader::checkSpecifiedPose(const std::string& nameOfPose)
	{
		if (nameOfPose=="FAP_6")
			;
		else if(nameOfPose=="FAP_7")
			;
		else if(nameOfPose=="FAP_12_0")
			;
		else if(nameOfPose=="FAP_12_1")
			;
		else if(nameOfPose=="FAP_13_0")
			;
		else if(nameOfPose=="FAP_13_1")
			;
		else if(nameOfPose=="FAP_17_")
			;
		else if(nameOfPose=="FAP_31_")
			;
		else if(nameOfPose=="FAP_32_")
			;
		else if(nameOfPose=="FAP_33_")
			;
		else if(nameOfPose=="FAP_34_")
			;
		else if(nameOfPose=="FAP_35_")
			;
		else if(nameOfPose=="FAP_36_")
			;
		else if(nameOfPose=="FAP_39_")
			;
		else if(nameOfPose=="FAP_40_")
			;
		else if(nameOfPose=="FAP_41_")
			;
		else if(nameOfPose=="FAP_42_")
			;
		else if(nameOfPose=="FAP_55_")
			;
		else if(nameOfPose=="FAP_56_")
			;
		else if(nameOfPose=="FAP_59_")
			;
		else if(nameOfPose=="FAP_60_")
			;
		//other FAPs
		else{
			//		AnimatedMesh* animatedMesh = dynamic_cast<AnimatedMesh*>(_pMesh);
			//		animatedMesh->_nameOfPosesNoSpecified.push_back(nameOfPose);
		}
	}
}