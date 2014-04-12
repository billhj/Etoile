/**
* Copyright(C) 2009-2012                
* @author Jing HUANG
* @file HMMBackward.cpp
* @brief 
* @date 1/2/2011
*/

#include "HMMBackward.h"
#include <iostream>

namespace Etoile
{
	HMMBackward::HMMBackward(void)
	{
	}


	HMMBackward::~HMMBackward(void)
	{
	}

	double HMMBackward::backward(HMMModel* model, std::vector<int>& observation, double* pi, double** beta )
	{
		std::cout<<std::endl<<"start HMM Backward method:"<<std::endl;
		int number = observation.size();
		unsigned int statesSize = model->getStates().size();
		HMMStateObservationMatrix& stateObservationMatrix = model->getStateObservationMatrix();
		HMMStateTransitionMatrix& stateTransitionMatrix = model->getStateTransitionMatrix();
		//init
		for(unsigned int i = 0; i < statesSize; ++i){
			beta[number-1][i] = 1.0;
		}

		double sum = 0;
		//recursive
		for (int t = number - 2; t >= 0; --t)
		{
			for (unsigned int j = 0; j < statesSize; ++j)
			{
				sum = 0.0;
				for(unsigned int i = 0; i < statesSize; ++i)
				{
					Etoile::HMMStateTransition& st = stateTransitionMatrix.getTransitionProbability(i, j);
					Etoile::HMMStateObservation& so = stateObservationMatrix.getStateObservationProbability(j, observation[t + 1]);
					std::cout<<beta[t + 1][i]<<"*"<<st.getProbability()<<"* " << so.getProbability();
					sum += beta[t + 1][i] * (st.getProbability()) * so.getProbability();
				}
				beta[t][j] = sum ;
				std::cout<<"beta[" << t <<"]["<<j<<"] : "<< sum <<std::endl;
			}
		}
		//computer sequence probability
		double prob = 0;
		for (unsigned int i = 0; i < statesSize; ++i)
		{
			prob += beta[0][i];
		}
		std::cout<<"backward prob: "<<prob<<std::endl;
		return prob;
	}

}