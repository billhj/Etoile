/**
* Copyright(C) 2009-2012                
* @author Jing HUANG
* @file BinaryMeshLoader.h
* @brief 
* @date 1/2/2011
*/

#pragma once

#include <string>
#include "../Mesh.h"
#include "MeshLoader.h"
namespace Etoile
{
	class BinaryMeshLoader : public MeshLoader
	{
	public:
		BinaryMeshLoader(){}
		~BinaryMeshLoader(){}
		virtual bool loadFromFile(const std::string& fileName, Mesh* mesh) override
		{
			_pMesh = mesh;
			std::ifstream in_file(fileName.c_str(), std::ios_base::binary);
#ifdef USING_BOOST
			boost::archive::binary_iarchive ia(in_file);
			//boost::archive::text_iarchive ia(in_file);
			ia & *_pMesh;
#endif
			in_file.close();
#if defined(_DEBUG) || defined(DEBUG)
			std::cout<<" binary mesh loaded"<<std::endl;
#endif
			return true;
		}

	};

}
