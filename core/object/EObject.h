/**
* Copyright(C) 2009-2012                
* @author Jing HUANG
* @file EObject.h
* @brief 
* @date 1/2/2011
*/

#pragma once
#include <string>
#include <vector>
#include <map>
#include <assert.h>

namespace Etoile
{
	class EObject;
	class ObjectManager;

	/*
	class EObject
	{
	public:
		EObject()
		{
			_className = initClassName();
			ObjectManager::getInstance()->registerObject(this);
		}

		const std::string getClassName() const{
			return _className;
		}

	protected:
		virtual std::string initClassName() = 0;
	protected:
		std::string _className;
	};

	class ObjectManager
	{
		
	private:
		static ObjectManager _om;
		std::map<std::string, int> _nameIdMap;
		std::vector<EObject*> _objects;
		ObjectManager(){}
	public:
		static ObjectManager* getInstance()
		{
			return &_om;
		}
		void registerObject(EObject* object){
			const std::string name = object->getClassName();
			std::map<std::string, int>::iterator itor = _nameIdMap.find(name);
			if(itor != _nameIdMap.end()){
				int idx = itor->second;
				std::string output = "object " + name + " already exists in the list";
				assert(NULL, output);
				_objects[idx] = object;
			}
			else
			{
				_nameIdMap[name] = _objects.size();
				_objects.push_back(object);
			}
		}

		EObject* getObject(const std::string& className)
		{
			std::map<std::string, int>::iterator itor = _nameIdMap.find(className);
			if(itor != _nameIdMap.end()){
				return _objects[itor->second];
			}
			return NULL;
		}
		
	};
	*/

}