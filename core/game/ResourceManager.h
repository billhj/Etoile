/**
* Copyright(C) 2009-2012                
* @author Jing HUANG
* @file ResourceManager.h
* @brief 
* @date 1/2/2011
*/

#pragma once
#ifndef RESOURCE_MANAGER_H
#define RESOURCE_MANAGER_H
#include <assert.h>
#include <vector>

namespace Etoile
{
	template <class DataType>
	class ResourceManager
	{
		DataType* getByIndex(unsigned int idx)
		{
			assert(idx < _datas.size());
			return _datas[idx];
		}

		std::string getNameByIndex(unsigned int idx)
		{
			assert(idx < _names.size());
			return _names[idx];
		}

		unsigned int add(DataType* data, const std::string& name = "")
		{
			unsigned int i = _datas.size();
			assert( i == _names.size());
			_datas.push_back(data);
			_names.push_back(name);
			return i;
		}

	protected:
		std::vector<DataType*> _datas;
		std::vector<std::string> _names;
	};
}


#endif //RESOURCE_MANAGER_H