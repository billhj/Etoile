/**
* Copyright(C) 2009-2012                
* @author Jing HUANG
* @file BVHFrame.h
* @brief 
* @date 1/2/2011
*/

#pragma once
#include <vector>

namespace Etoile
{
	enum  ChannelType
	{
		X_ROTATION, Y_ROTATION, Z_ROTATION,
		X_POSITION, Y_POSITION, Z_POSITION
	};

	struct  Channel
	{
		ChannelType _type;
		float _value;
	};

	struct JointMotion
	{
		std::string _jointName;
		std::vector<Channel> _channels;
	};
	typedef std::vector<JointMotion> JointMotions;

	class BVHFrame
	{
	public:
		void addJointMotion(JointMotion& jointMotion)
		{
			_motions.push_back(jointMotion);
		}

		void setJointMotion(unsigned int idx, JointMotion& jointMotion)
		{
			_motions[idx] = jointMotion;
		}

		JointMotions& getJointMotions()
		{
			return _motions;
		}
		JointMotion& getJointMotionByIndex(unsigned int idx)
		{
			return _motions[idx];
		}

	private:
		JointMotions _motions;

	};
	class BVHFrames
	{
	public:
		BVHFrames()
		{
			_interval = 0.1f;
		}
		void setInterval(float value){_interval = value;}
		float getInterval(){ return _interval;}
		void addFrame(BVHFrame& frame)
		{
			_frames.push_back(frame);
		}
		std::vector<BVHFrame>& getFrames(){return _frames;}
	private:
		float _interval;
		std::vector<BVHFrame> _frames;
	};

}
