/**
* Copyright(C) 2009-2012                
* @author Jing HUANG
* @file KeyFrameSequence.h
* @brief 
* @date 1/2/2011
*/

#pragma once
#include <string>
#include <map>
#include "KeyFrame.h"

namespace Etoile
{
	class KeyFrameSequence
	{
	public:
		KeyFrameSequence(const std::string& name)
		{
			_name = name;
		}
		KeyFrameSequence(){}
		~KeyFrameSequence(){clear();}
		void clear();
		std::string getName(){return _name;}
		int getKeyFrameIndex(float time);
		KeyFrame* getKeyFrameByIndex(unsigned int index);
		KeyFrame* getKeyFrameByTime(float time);
		KeyFrames& getKeyFrames(){return _keyframes;}
		void insertKeyFrame(KeyFrame* keyframe);
		void replaceKeyFrame(KeyFrame* keyframeOld, KeyFrame* keyframeNew);
		void deleteKeyFrameByIndex(int index);
		void deleteKeyFrameByTime(float time);
		void deleteKeyFrame(KeyFrame* keyframe);
		void deleteKeyFramesBefore(float time);
		void deleteKeyFramesAfter(float time);
		void deleteKeyFrames();
		void resetTime();
	protected:
		std::string _name;
		KeyFrames _keyframes;
	};

	typedef KeyFrameSequence Track;
}
