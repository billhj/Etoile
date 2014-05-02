/**
* Copyright(C) 2009-2012                
* @author Jing HUANG
* @file BipedSkeleton.h
* @brief 
* @date 1/2/2011
*/
#pragma once
#ifndef BIPED_SKELETON_H
#define BIPED_SKELETON_H

#include "Skeleton.h"

namespace Etoile
{
	class BipedSkeleton : public Skeleton
	{
	public:
		BipedSkeleton(const std::string& name = "");
		void init();
	};
}
#endif //BIPED_SKELETON_H