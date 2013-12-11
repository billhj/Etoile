/**
* Copyright(C) 2009-2012                
* @author Jing HUANG
* @file Frame.h
* @brief 
* @date 1/2/2011
*/

#pragma once
#include <string>
#include <map>
#include "FrameParameters.h"

namespace Etoile
{
	class Frame
	{
	public:
		Frame(){}
		void clear(){ _info.clear();}
		void addFrameParameters(const std::string& name, FrameParameters& jf)
		{
			std::map<std::string, int>::iterator itor = _idx.find(name);
			if(itor != _idx.end())
			{
				_info[itor->second] = jf;
			}else
			{
				_info.push_back(jf);
				_idx[name] = _info.size() - 1;
			}
		}

		std::vector<FrameParameters>& getFrameParameters(){ return _info;}
		std::map<std::string, int>& getIndices(){return _idx;}
		void setRootJoint(const std::string& name){ _root = name;}
		const std::string getRootJoint(){ return _root; }
	protected:
		std::map<std::string, int> _idx;
		std::vector<FrameParameters> _info;
		std::string _root;
	};

}
