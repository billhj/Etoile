/**
* Copyright(C) 2009-2012                
* @author Jing HUANG
* @file MotionClipsAnalysis.h
* @brief 
* @date 1/2/2011
*/

#pragma once
#include "MotionClipLibrary.h"
#include "MotionClipPairSimilarity.h"

namespace Etoile
{

	class MotionClipsAnalysis
	{
	public:
		MotionClipsAnalysis();
		~MotionClipsAnalysis();
		void setLibrary(MotionClipLibrary* lib){ _pLib = lib;}
		MotionClipLibrary* getLibrary(){return _pLib;}
		void generateMatrixMemory();
		void computeSimilarityMatrixForAllClips();

		/**
		* 0 ~ 1
		*/
		MotionClipPairSimilarity* computeSimilaritiesBetweenClips(unsigned int idx0, unsigned int idx1);
		MotionClipPairSimilarity* computeSimilaritiesBetweenClips(const std::string& name0, const std::string& name1);
		MotionClipPairSimilarity* computeSimilaritiesBetweenClips(MotionClip*, MotionClip*);

		MotionClipPairSimilarity* getMotionClipPairSimilarity(unsigned int idx0, unsigned int idx1)
		{
			if(idx0 <= idx1)
			{
				MotionClips& clips = _pLib->getClips();
				unsigned int size = clips.size();
				int idx = _clipsPairMemoIdx[idx0 * size + idx1];
				return _similarities[idx];
			}
			else
			{
				return NULL;
			}
		}

	private:
		MotionClipLibrary* _pLib;
		std::vector<int> _clipsPairMemoIdx;
		std::vector<MotionClipPairSimilarity*> _similarities;

	};



}
