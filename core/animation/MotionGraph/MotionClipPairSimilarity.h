/**
* Copyright(C) 2009-2012                
* @author Jing HUANG
* @file MotionClipPairSimilarity.h
* @brief 
* @date 1/2/2011
*/

#pragma once
#include <vector>
#include "MotionClip.h"

namespace Etoile
{

	class MotionClipPairSimilarity
	{
	public:
		MotionClipPairSimilarity(MotionClip* clip0, MotionClip* clip1): _pClip0(clip0), _pClip1(clip1){}
		~MotionClipPairSimilarity(){}
		float getSimilarity(unsigned int frameClip0, unsigned int frameClip1)
		{
			return _similarities[frameClip0 * _pClip1->getKeyFrames().size() + frameClip1];
		}
		void generate(unsigned int similarityWindowsSize = 5, std::vector<float> timeWeights = std::vector<float>(), std::map<std::string, float> jointWeights = std::map<std::string, float>())
		{

			std::vector<KeyFrame*>& fs0 = _pClip0->getKeyFrames();
			std::vector<KeyFrame*>& fs1 = _pClip1->getKeyFrames();
			unsigned int size0 = fs0.size();
			unsigned int size1 = fs1.size();

			for(unsigned int i = 0; i < size0; ++i)
			{
				for(unsigned int j = 0; j < size1; ++j)
				{
					std::vector<KeyFrame*> frames0;
					std::vector<KeyFrame*> frames1;
					for(int w = - (int)(similarityWindowsSize * 0.5); w <= (int)(similarityWindowsSize * 0.5); ++w)
					{
						//std::cout<<w<<std::endl;
						int idx0 = i + w;
						if(idx0 < 0) idx0 += size0;
						if(idx0 >= (int)size0) idx0 -= size0;

						int idx1 = j + w;
						if(idx1 < 0) idx1 += size1;
						if(idx1 >= (int)size1) idx1 -= size1;

						KeyFrame* f0 = fs0[idx0];
						KeyFrame* f1 = fs1[idx1];
						frames0.push_back(f0);
						frames1.push_back(f1);
					}
					float result = computeSimilarityBetweenFrames(frames0, frames1, timeWeights, jointWeights);
					_similarities.push_back(result);
					std::cout<<"idx "<< i << "  "<<j<<std::endl;
				}
			}

			//std::cout<<"similarity compute time : "<< t1<<std::endl;
		}
	private:
		float computeSimilarityBetweenFrames(std::vector<KeyFrame*> frames0, std::vector<KeyFrame*> frames1, std::vector<float> timeWeights, std::map<std::string, float> jointWeights)
		{
			unsigned int size = frames0.size();
			if(size != frames1.size())
				return 0;
			float sum = 0;
			float sumWeight = 0;
			for(unsigned int i = 0; i < size; ++i)
			{
				KeyFrame* frame0 = frames0[i]; 
				KeyFrame* frame1 = frames1[i];
				float weight = 1;
				if(timeWeights.size() > i)
				{
					weight = timeWeights[i];
				}
				float similarity = computeSimilarity(frame0, frame1, jointWeights);
				sum += similarity * weight;
				sumWeight += weight;
			}
			sum /= sumWeight;
			return sum;
		}

		float computeSimilarity(KeyFrame* frame0, KeyFrame* frame1, std::map<std::string, float> jointWeights)
		{
			float sum = 0;
			float sumweight = 0;
			std::string rootName = frame0->getRootJoint();
			std::vector<FrameParameters>& jfs0 =frame0->getFrameParameters();
			std::vector<FrameParameters>& jfs1 =frame1->getFrameParameters();
			if(jfs0.size() != jfs1.size())
			{
				return 0;
			}

			std::map<std::string, int>& jfsIdx0 =frame0->getIndices();
			std::map<std::string, int>& jfsIdx1 =frame1->getIndices();

			std::map<std::string, int>::iterator itor = jfsIdx0.begin();
			for(;itor != jfsIdx0.end(); ++itor)
			{
				std::string name = itor->first;
				if(name == rootName) continue;   //if root then for the next, we didnt count the root rotation and translation for the similarity

				float weight = 1;
				std::map<std::string, float>::iterator itweight = jointWeights.find(name);
				if(itweight != jointWeights.end())
				{
					weight = itweight->second;
				}

				FrameParameters& jf0 = jfs0[itor->second];
				FrameParameters& jf1 = jfs1[itor->second];

				Quaternionf r0 = jf0._localRotation;
				Quaternionf r1 = jf1._localRotation;
				Quaternionf q = r0 / r1;
				q.normalize();
				float angle = q.angle();
				sum += weight * (1 - abs(angle / M_PI));

				sumweight += weight;
			}

			sum /= sumweight;
			return sum;
		}


		

	private:
		MotionClip* _pClip0;
		MotionClip* _pClip1;
		std::vector<float> _similarities;
	};



}
