/**
* Copyright(C) 2009-2012                
* @author Jing HUANG
* @file MotionClipsAnalysis.cpp
* @brief 
* @date 1/2/2011
*/

#include "MotionClipsAnalysis.h"

/**
* @brief For tracking memory leaks under windows using the crtdbg
*/
#if ( defined( _DEBUG ) || defined( DEBUG ) ) && defined( _MSC_VER )
#define _CRTDBG_MAP_ALLOC
#include <crtdbg.h>

#define DEBUG_NEW new( _NORMAL_BLOCK, __FILE__, __LINE__ )
#define new DEBUG_NEW
#endif

namespace Etoile
{
	MotionClipsAnalysis::MotionClipsAnalysis():_pLib(NULL)
	{

	}

	MotionClipsAnalysis::~MotionClipsAnalysis()
	{

	}

	void MotionClipsAnalysis::generateMatrixMemory()
	{
		MotionClips& clips = _pLib->getClips();
		unsigned int size = clips.size();
		for(unsigned int i = 0; i < size; ++i)
		{
			for(unsigned int j = 0; j < size; ++j)
			{
				_clipsPairMemoIdx.push_back(-1);
			}
		}
	}

	void MotionClipsAnalysis::computeSimilarityMatrixForAllClips()
	{
		generateMatrixMemory();
		MotionClips& clips = _pLib->getClips();
		unsigned int size = clips.size();

		for(unsigned int i = 0; i < size; ++i)
		{
			for(unsigned int j = 0; j < size; ++j)
			{
				if(i <= j)
				{
					_similarities.push_back(computeSimilaritiesBetweenClips(i, j));   //only compute the small + big (or equal) order
					_clipsPairMemoIdx[i * size + j] = _similarities.size() - 1;
				}
				else
				{
					_clipsPairMemoIdx[i * size + j] = _clipsPairMemoIdx[j * size + i];
				}

			}
		}
	}

	MotionClipPairSimilarity* MotionClipsAnalysis::computeSimilaritiesBetweenClips(unsigned int idx0, unsigned int idx1)
	{
		MotionClip* clip0 = _pLib->getClip(idx0);
		MotionClip* clip1 = _pLib->getClip(idx1);
		return computeSimilaritiesBetweenClips(clip0, clip1);
	}

	MotionClipPairSimilarity* MotionClipsAnalysis::computeSimilaritiesBetweenClips(const std::string& name0, const std::string& name1)
	{

		MotionClip* clip0 = _pLib->getClip(name0);
		MotionClip* clip1 = _pLib->getClip(name1);
		return computeSimilaritiesBetweenClips(clip0, clip1);
	}

	MotionClipPairSimilarity*  MotionClipsAnalysis::computeSimilaritiesBetweenClips(MotionClip* clip0, MotionClip* clip1)
	{
		if(clip0 == NULL || clip1 == NULL)
		{
			std::cout<<"MotionClipsAnalysis::computeSimilaritiesBetweenClips : clip0 " <<clip0 <<  " clip1 " <<clip1 <<std::endl;
			return NULL;
		}
		MotionClipPairSimilarity* similarity = new MotionClipPairSimilarity(clip0, clip1);
		similarity->generate();
		return similarity;
	}



}
