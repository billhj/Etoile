/**
* Copyright(C) 2009-2012                
* @author Jing HUANG
* @file Core.h
* @brief 
* @date 1/2/2011
*/

#pragma once
/*#include "renderer/GpuProgramManager.h"
#include "renderer/TextureManager.h"
#include "renderer/MaterialManager.h"
#include "renderer/LightManager.h"
#include "renderer/MeshManager.h"*/
#include "module/EPlugin.h"
#include "util/FileSearch.h"

namespace Etoile
{
	class Core
	{
	protected:
		/*GpuProgramManager* _pGpuProgramManager;
		TextureManager* _pTextureManager;
		MaterialManager* _pMaterialManager;
		LightManager* _pLightManager;
		MeshManager* _pMeshManager;*/
		EPluginManager* _pEPluginManager;
	public:
		Core()
		{
			/*_pGpuProgramManager = new GpuProgramManager("core");
			_pTextureManager = new TextureManager("core");
			_pMaterialManager = new MaterialManager("core");
			_pLightManager = new LightManager("core");
			_pMeshManager = new MeshManager("core");*/
			_pEPluginManager = new EPluginManager();
		}

		virtual ~Core()
		{
		
			clear();
			/*delete _pGpuProgramManager;
			delete _pTextureManager;
			delete _pMaterialManager;
			delete _pLightManager;
			delete _pMeshManager;*/
			delete _pEPluginManager;
		}

		virtual void clear()
		{
			/*_pGpuProgramManager->clear();
			_pTextureManager->clear();
			_pMaterialManager->clear();
			_pLightManager->clear();
			_pMeshManager->clear();*/
			_pEPluginManager->release();
		}

		void loadPlugins()
		{
			FileSearch fs;
			std::vector<FileInfo> filenames = fs.search("./plugins","dll");

			for(unsigned int i = 0; i < filenames.size(); ++i)
			{
#if defined(_DEBUG) || defined(DEBUG)
				_pEPluginManager->load(filenames[i]._path.c_str(),filenames[i]._filename.c_str(), "loadDescription");
#else
				_pEPluginManager->load(filenames[i]._path.c_str(),filenames[i]._filename.c_str(), "loadDescription");
#endif
			}
		}
		/*GpuProgramManager* getGpuProgramManager(){return _pGpuProgramManager;}
		TextureManager* getTextureManager(){return _pTextureManager;}
		MaterialManager* getMaterialManager(){return _pMaterialManager;}
		LightManager* getLightManager(){return _pLightManager;}
		MeshManager* getMeshManager(){return _pMeshManager;}*/
		EPluginManager* getPluginManager(){return _pEPluginManager;}
	};
}
