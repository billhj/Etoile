/**
* Copyright(C) 2009-2012                
* @author Jing HUANG
* @file KeyFrame.h
* @brief 
* @date 1/2/2011
*/

#pragma once
#include <string>
#include <map>
#include "Frame.h"

namespace Etoile
{
	class KeyFrame : public Frame
	{
	public:
		KeyFrame(float time) : Frame(), _time(time)
		{
		}

		KeyFrame() : Frame(), _time(0)
		{
		}

		void setTime(float time)
		{
			_time = time;
		}

		float getTime()
		{
			return _time;
		}

	protected:
		float _time;
	};

	typedef std::vector<KeyFrame*> KeyFrames;
}
