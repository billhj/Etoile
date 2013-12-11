/**
* Copyright(C) 2009-2012                
* @author Jing HUANG
* @file EPlugin.h
* @brief 
* @date 1/2/2011
*/

#pragma once
#include <string>
#include <vector>
#include <map>
#include <windows.h>
#include <iostream>

namespace Etoile
{
#define PLUGIN_DLL extern "C" __declspec(dllexport) 

	
	class EPlugin
	{
	protected:
		std::string _name;
		long _deviceID;
	public:
		EPlugin();
		virtual ~EPlugin();
		virtual void init();
		virtual void apply();
		virtual void release();
		long getDeviceID(){return _deviceID;}
	};


	typedef EPlugin* (*loadPlugin)();

	struct EPluginsDescription
	{
		EPluginsDescription()
		{
			menu = "Plugins";
		}
		std::string menu;
		std::vector<std::string> plugins_names;
		std::vector<loadPlugin> plugins_load_functions;
		std::string name;
	};

	typedef void (*loadDescription)(EPluginsDescription**);

	class EPluginManager
	{
		
	public:
		/*static EPluginLoader<T>* getEPluginLoader()
		{
			static EPluginLoader<T> plug;
			return &plug;
		}*/
		EPluginManager() ;
		virtual ~EPluginManager();

		/**
		@param szName                           - the name of the dll to load
		@return void                             - the plugin
		**/
		void load(const char* path, const char* filename, const char* descriptionFunctionName);

		/**
		@brief Releases the library from memory
		**/
		void release();
		void add(EPluginsDescription*);

		std::vector<EPluginsDescription*>& getDescriptions(){return _descriptions;}
		std::map<std::string, int>& getNameIndex(){return _idx;}
		EPluginsDescription* getDescription(const std::string& name);
	private:
		std::vector<EPluginsDescription*> _descriptions;
		std::map<std::string, int> _idx;
		// platform specific
		HMODULE _hLibrary;
	};
	typedef std::vector<EPlugin*> EPlugins;
}