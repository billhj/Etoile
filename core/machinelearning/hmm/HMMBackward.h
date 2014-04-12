/**
* Copyright(C) 2009-2012                
* @author Jing HUANG
* @file HMMBackward.h
* @brief 
* @date 1/2/2011
*/

#pragma once
#include "HMMModel.h"

namespace Etoile
{
class HMMBackward
{
public:
	HMMBackward(void);
	~HMMBackward(void);
	// the probability of the sequence of observations from end to certain time point
	double backward(HMMModel* model, std::vector<int>& observation, double* pi, double** beta );
};
}
