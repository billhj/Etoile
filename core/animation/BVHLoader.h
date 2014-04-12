/**
* Copyright(C) 2009-2012                
* @author Jing HUANG
* @file BVHLoader.h
* @brief 
* @date 1/2/2011
*/

#pragma once
#include <string>
#include "KeyFrameSequence.h"
#include "BVHFrame.h"
#include "geometry/Skeleton.h"

namespace Etoile
{
	class BVHLoader{
	public:
		BVHLoader();
		bool loadFromBVHFile(const std::string& filepath, KeyFrameSequence* sequence, Skeleton* skeleton);
		Skeleton* getSkeleton(){return _pSkeleton;}
		KeyFrameSequence* getKeyFrameSequence(){return _skeletonframes;}

	private:
		void loadSkeleton(std::istream& in);
		void loadFrames(std::istream& in, BVHFrame& frame);
		void fromBVHFrameToSkeletonFrame(BVHFrame& frame, KeyFrame& skeletonframe);

		
		std::string _fileName;
		std::string _rootName;
		KeyFrameSequence* _skeletonframes;
		Skeleton* _pSkeleton;

	};

}
