/**
* Copyright(C) 2009-2012                
* @author Jing HUANG
* @file HMMForward.cpp
* @brief 
* @date 1/2/2011
*/

#include "HMMForward.h"
#include <iostream>

namespace Etoile
{
	HMMForward::HMMForward(void)
	{
	}


	HMMForward::~HMMForward(void)
	{
	}

	double HMMForward::forward(HMMModel* model, std::vector<int>& observation, double* pi, double** alpha )
	{
		std::cout<<std::endl<<"start HMM Forward method:"<<std::endl;
		unsigned int number = observation.size();
		unsigned int statesSize = model->getStates().size();
		HMMStateObservationMatrix& stateObservationMatrix = model->getStateObservationMatrix();
		HMMStateTransitionMatrix& stateTransitionMatrix = model->getStateTransitionMatrix();
		//init
		for(unsigned int i = 0; i < statesSize; ++i){
			alpha[0][i] = pi[i] * stateObservationMatrix.getStateObservationProbability(i,observation[0]).getProbability();
			std::cout<<"alpha[0]["<<i<<"] : "<< alpha[0][i]<<std::endl;
		}

		double sum = 0;
		//recursive
		for (unsigned int t = 0; t < number - 1; ++t)
		{
			for (unsigned int j = 0; j < statesSize; ++j)
			{
				sum = 0.0;
				for(unsigned int i = 0; i < statesSize; ++i)
				{
					Etoile::HMMStateTransition& st = stateTransitionMatrix.getTransitionProbability(i, j);
					sum += alpha[t][i] * (st.getProbability());
					std::cout<<alpha[t][i]<<"*"<<st.getProbability()<<"  ";
				}
				Etoile::HMMStateObservation& so = stateObservationMatrix.getStateObservationProbability(j, observation[t + 1]);
				alpha[t + 1][j] = sum * so.getProbability();
				std::cout<<"alpha[" << t+ 1<<"]["<<j<<"]: "<< alpha[t + 1][j] <<" = "<<sum <<"*"<<so.getProbability()<<std::endl;
			}
		}
		//computer sequence probability
		double prob = 0;
		for (unsigned int i = 0; i < statesSize; ++i)
		{
			double v = alpha[number - 1][i];
			prob += v;
		}
		std::cout<<"forward prob: "<<prob<<std::endl;
		return prob;
	}

}