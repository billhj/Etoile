/**
* Copyright(C) 2009-2012                
* @author Jing HUANG
* @file EPlugin.cpp
* @brief 
* @date 1/2/2011
*/

#include "EPlugin.h"

namespace Etoile
{
	long EPlugin_deviceID = 0;
	EPlugin::EPlugin()
	{
		_deviceID = EPlugin_deviceID;
		++EPlugin_deviceID;
	}
	EPlugin::~EPlugin(){}
	void EPlugin::init(){}
	void EPlugin::apply(){}
	void EPlugin::release(){}



	EPluginManager::EPluginManager() : _hLibrary(NULL){}
	EPluginManager::~EPluginManager(){ release();}

	/**
	@param szName                           - the name of the dll to load
	@return T*                             - the plugin
	**/
	void EPluginManager::load(const char* path, const char* filename, const char* descriptionFunctionName)
	{
		loadDescription pfnDes;
		// load our rendering pluggin
		//LPCTSTR name=(LPCTSTR)(szName);
		int size=MultiByteToWideChar(CP_ACP,0,path,-1,NULL,0);
		wchar_t *name=new wchar_t[size+1];
		if(!MultiByteToWideChar(CP_ACP,0,path,-1,name,size))
		{
			return;
		}
		_hLibrary = LoadLibrary((LPCTSTR)name);
		//_hLibrary = LoadLibrary(L"ImageViewerd.dll");
		// handle errors
		if( _hLibrary == NULL )
		{
			std::cout<<path<<" loading fails"<<std::endl;
			DWORD lastError = GetLastError();
			std::cout << "General failure. GetLastError returned "<< lastError << "."<<std::endl;
			return;
		}

		// get the address of our renderer accessing function
		LPCSTR d = (LPCSTR)descriptionFunctionName;
		pfnDes = (loadDescription)::GetProcAddress(_hLibrary, d);
		EPluginsDescription* pDes = NULL;
		// get our graphics device
		if(pfnDes)
		{
			pfnDes(&pDes);
			size_t nPos = std::string(filename).find_last_of( '.' );
			//std::string name = std::string(filename).substr(0, nPos);
			_descriptions.push_back(pDes);
			_idx[pDes->name] = _descriptions.size() - 1;
		}
	}

	/**
	@brief Releases the library from memory
	**/
	void EPluginManager::release()
	{
		/*for(unsigned int i = 0; i < _descriptions.size(); ++i)
		{
			EPluginsDescription* desc = _descriptions[i];
			if( desc != NULL )
			{
				delete desc;
			}
		}
		for(unsigned int i = 0; i < _plugins.size(); ++i)
		{
			EPlugin* plugin = _plugins[i];
			if( plugin != NULL )
			{
				delete plugin;
			}
		}*/
		::FreeLibrary( _hLibrary );
	}

	void EPluginManager::add(EPluginsDescription* descrip)
	{

		_descriptions.push_back(descrip);
		_idx[descrip->name] = _descriptions.size() - 1;
	}


	EPluginsDescription* EPluginManager::getDescription(const std::string& name)
	{
		std::map<std::string, int>::iterator itor = _idx.find(name);
		if(itor != _idx.end())
		{
			return _descriptions[itor->second];
		}
		return NULL;
	}

}