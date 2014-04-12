/**
* Copyright(C) 2009-2012                
* @author Jing HUANG
* @file SkeletonAnimation.h
* @brief 
* @date 1/2/2011
*/

#pragma once
#include <list>
#include "geometry/Skeleton.h"
#include "Frame.h"
namespace Etoile
{

	class SkeletonAnimation
	{
	private:
		Skeleton* _pSkeleton;
		std::list<Frame> _frames;
	public:
		SkeletonAnimation(Skeleton* sk): _pSkeleton(sk)
		{

		}

		~SkeletonAnimation(){}
		void addFrames(std::vector<Frame>& frames)
		{
			_frames.insert(_frames.end(), frames.begin(), frames.end());
		}

		void addFrames(Frame frame)
		{
			_frames.push_back(frame);
		}

		void update()
		{
			if(_frames.size() > 0)
			{
				Frame frame = _frames.front();
				_frames.pop_front();
				Joints& joints =  _pSkeleton->getJoints();
				for(unsigned int i = 0; i < joints.size(); ++i)
				{
					Joint* j = joints[i];
					FrameParameters* fp = frame.getFrameParameter(j->getName());
					if(fp != NULL)
					{
						j->setLocalRotation(fp->_localRotation);
					}
				}
			}

		}


	};
}