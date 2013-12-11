/**
* Copyright(C) 2009-2012                
* @author Jing HUANG
* @file MeshLoader.h
* @brief 
* @date 1/2/2011
*/

#pragma once
#include <string>
#include <vector>
#include "../Mesh.h"
#include "TextureLoader.h"
namespace Etoile
{
	class MeshLoader
	{
	public:
		MeshLoader(){ _scale = Vec3f(1,1,1); }
		~MeshLoader(){}
		virtual bool loadFromFile(const std::string& fileName, Mesh* mesh) = 0;
		Mesh* getMesh(){return _pMesh;};
		void setTextureLoader(TextureLoader  * loader){_pTextureLoader = loader;}
		TextureLoader * getTextureLoader(){return _pTextureLoader;}
		std::vector<Material*>& getMaterials(){return _materials;}
		std::map<std::string, int> getMaterialNameIdMap(){return _materialNameMap;}
		const std::map<std::string, std::string>& getTexturePathMap() const {return _texturePathMap;}

	protected:
		std::string _fileName;
		std::map<std::string, std::string> _texturePathMap;
		std::map<std::string, int> _materialNameMap;
		std::vector<Material*> _materials;
		Vec3f _scale;
		Vec3f _translate;
		Quaternionf _rotate;
		Mesh* _pMesh;
		TextureLoader * _pTextureLoader;
	};

}
