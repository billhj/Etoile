/**
* Copyright(C) 2009-2012                
* @author Jing HUANG
* @file KeyFrameSequence.cpp
* @brief 
* @date 1/2/2011
*/

#include "KeyFrameSequence.h"

/**
* @brief For tracking memory leaks under windows using the crtdbg
*/
#if ( defined( _DEBUG ) || defined( DEBUG ) ) && defined( _MSC_VER )
#define _CRTDBG_MAP_ALLOC
#include <crtdbg.h>

#define DEBUG_NEW new( _NORMAL_BLOCK, __FILE__, __LINE__ )
#define new DEBUG_NEW
#endif

#ifndef EPS
#define EPS 0.00001
#endif

namespace Etoile
{
	void KeyFrameSequence::clear()
	{
		_keyframes.clear();
	}

	int KeyFrameSequence::getKeyFrameIndex(float time)
	{
		for(unsigned int i = 0; i < _keyframes.size(); ++i)
		{
			KeyFrame *keyframe = _keyframes[i];
			if (fabs(time - keyframe->getTime()) < EPS)
			{
				return i;
			}
			else if(time > keyframe->getTime())
			{
				if(i + 1 == _keyframes.size()){
					return i;
				} 
				else if(time < _keyframes[i + 1]->getTime())
				{
					return i;
				}
			}
		}
		return -1;
	}

	KeyFrame* KeyFrameSequence::getKeyFrameByIndex(unsigned int index)
	{
		if(index >= 0 && index < _keyframes.size())
		{
			return _keyframes[index];
		}
		return NULL;
	}

	KeyFrame* KeyFrameSequence::getKeyFrameByTime(float time)
	{
		int idx = getKeyFrameIndex(time);
		return getKeyFrameByIndex(idx);
	}

	void KeyFrameSequence::insertKeyFrame(KeyFrame* keyframe)
	{
		if (_keyframes.size() == 0) 
		{
			_keyframes.push_back(keyframe);
			return;
		}

		KeyFrame* first = _keyframes[0];
		KeyFrame* last = _keyframes[_keyframes.size() - 1];

		if (keyframe->getTime() > last->getTime()) 
		{
			// insert at the end
			_keyframes.push_back(keyframe);
		}
		else if (keyframe->getTime() < first->getTime())
		{
			// insert in the beginning
			_keyframes.insert(_keyframes.begin(), keyframe);
		}
		else if (fabs(keyframe->getTime() - last->getTime()) < EPS)
		{
			// replace the last one
			_keyframes[_keyframes.size() - 1] = keyframe;
		}
		else if (fabs(keyframe->getTime() - first->getTime()) < EPS)
		{
			// replace the first one
			_keyframes[0] = keyframe;
		}
		else
		{
			int index;
			for(unsigned int i = 0; i < _keyframes.size(); ++i)
			{
				if(keyframe->getTime() < _keyframes[i]->getTime())
				{
					index = i;
					break;
				}
			}
			// put it between kf->previous and kf but first
			// check to see if it should override ont of the two

			if (fabs(keyframe->getTime() - _keyframes[index - 1]->getTime()) < EPS)
			{
				_keyframes[index - 1] = keyframe;	
			}
			else if (fabs(keyframe->getTime() - _keyframes[index]->getTime()) < EPS)
			{
				_keyframes[index] = keyframe;
			}
			else
			{
				// put it between kf->getPrevious() and kf
				_keyframes.insert(_keyframes.begin() + index, keyframe);
			}
		}
	}


	void KeyFrameSequence::replaceKeyFrame(KeyFrame* keyframeOld, KeyFrame* keyframeNew)
	{
		if (keyframeOld == NULL)
			return;
		deleteKeyFrame(keyframeOld);
		insertKeyFrame(keyframeNew);
	}

	void KeyFrameSequence::deleteKeyFrameByIndex(int index)
	{
		_keyframes.erase(_keyframes.begin() + index);
	}

	void KeyFrameSequence::deleteKeyFrameByTime(float time)
	{
		deleteKeyFrameByIndex(getKeyFrameIndex(time));
	}

	void KeyFrameSequence::deleteKeyFrame(KeyFrame* keyframe)
	{
		for(unsigned int i = 0; i < _keyframes.size(); ++i)
		{
			KeyFrame* kf = _keyframes[i];
			if(kf == keyframe)
			{
				_keyframes.erase(_keyframes.begin() + i);
				return;
			}
		}
	}

	void KeyFrameSequence::deleteKeyFramesBefore(float time)
	{
		for(unsigned int i = 0; i < _keyframes.size(); ++i)
		{
			KeyFrame* keyframe = _keyframes[i];
			if(keyframe->getTime() > time)
			{
				if(i == 0) return;
				_keyframes.erase(_keyframes.begin(), _keyframes.begin() + i - 1);
			}
		}

	}

	void KeyFrameSequence::deleteKeyFramesAfter(float time)
	{
		for(unsigned int i = 0; i < _keyframes.size(); ++i)
		{
			KeyFrame* keyframe = _keyframes[i];
			if(keyframe->getTime() > time)
			{
				_keyframes.erase(_keyframes.begin() + i, _keyframes.end());
			}
		}
	}

	void KeyFrameSequence::deleteKeyFrames()
	{
		_keyframes.clear();
	}

	void KeyFrameSequence::resetTime()
	{
		for(unsigned int i = 0; i < _keyframes.size(); ++i)
		{
			KeyFrame* keyframe = _keyframes[i];
			keyframe->setTime(keyframe->getTime() - _keyframes[0]->getTime());
		}
	}

}
