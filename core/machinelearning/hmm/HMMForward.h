/**
* Copyright(C) 2009-2012                
* @author Jing HUANG
* @file HMMForward.h
* @brief 
* @date 1/2/2011
*/

#pragma once
#include "HMMModel.h"

namespace Etoile
{
	class HMMForward
	{
	public:
		HMMForward(void);
		~HMMForward(void);
		// the probability of the sequence of observations from begin to certain time point
		double forward(HMMModel* model, std::vector<int>& observation, double* pi, double** alpha );
	};
}