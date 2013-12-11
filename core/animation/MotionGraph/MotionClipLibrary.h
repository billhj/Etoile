/**
* Copyright(C) 2009-2012                
* @author Jing HUANG
* @file MotionClipLibrary.h
* @brief 
* @date 1/2/2011
*/

#pragma once
#include "MotionClip.h"

namespace Etoile
{

	class MotionClipLibrary
	{
	public:
		MotionClipLibrary(const std::string& name):_name(name){}
		~MotionClipLibrary(){}
		void addMotion(MotionClip* sequence)
		{
			_clipIdx[sequence->getName()] = _clips.size();
			_clips.push_back(sequence);
		}
		MotionClip* getClip(unsigned int idx)
		{
			if(idx >= _clips.size())
			{
				return NULL;
			}
			return _clips[idx];
		}
		int getClipIdx(const std::string name)
		{
			std::map<std::string, int>::iterator itor = _clipIdx.find(name);
			if(itor != _clipIdx.end())
			{
				return itor->second;
			}
			return -1;
		}

		MotionClip* getClip(const std::string name)
		{
			std::map<std::string, int>::iterator itor = _clipIdx.find(name);
			if(itor != _clipIdx.end())
			{
				return getClip(itor->second);
			}
			return NULL;
		}

		MotionClips& getClips(){return _clips;}
	private:
		std::string _name;
		MotionClips _clips;
		std::map<std::string, int> _clipIdx;
	};



}
