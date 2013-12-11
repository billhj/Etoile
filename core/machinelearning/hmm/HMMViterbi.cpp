/**
* Copyright(C) 2009-2012                
* @author Jing HUANG
* @file HMMViterbi.cpp
* @brief 
* @date 1/2/2011
*/

#include "HMMViterbi.h"
#include <iostream>

namespace Etoile
{
	HMMViterbi::HMMViterbi(void)
	{
	}


	HMMViterbi::~HMMViterbi(void)
	{
	}

	double HMMViterbi::viterbi(HMMModel* model, std::vector<int>& observation, double* pi, double** alpha, int **psi, int* idx)
	{
		std::cout<<std::endl<<"start HMM Viterbi method:"<<std::endl;
		unsigned int number = observation.size();
		unsigned int statesSize = model->getStates().size();
		HMMStateObservationMatrix& stateObservationMatrix = model->getStateObservationMatrix();
		HMMStateTransitionMatrix& stateTransitionMatrix = model->getStateTransitionMatrix();

		//init
		for(unsigned int i = 0; i < statesSize; ++i){
			alpha[0][i] = pi[i] * stateObservationMatrix.getStateObservationProbability(i,observation[0]).getProbability();
			psi[0][i] = 0;
		}


		//recursive
		for (unsigned int t = 0; t < number - 1; ++t)
		{
			for (unsigned int j = 0; j < statesSize; ++j)
			{
				int maxvalind = -1;
				double maxval = 0, val = 0;
				for(unsigned int i = 0; i < statesSize; ++i)
				{
					val = alpha[t][i] * stateTransitionMatrix.getTransitionProbability(i, j).getProbability() ;
					if (val > maxval)
					{
						maxval = val;
						maxvalind = i;
					}
				}
				alpha[t + 1][j] = maxval * stateObservationMatrix.getStateObservationProbability(j, observation[t + 1]).getProbability();
				psi[t + 1][j] = maxvalind;
			}
		}

		//computer sequence probability
		double prob = 0;
		idx[number - 1] = -1;
		for (unsigned int i = 0; i < statesSize; ++i)
		{
			if(prob < alpha[number - 1][i])
			{
				prob = alpha[number - 1][i];
				idx[number - 1] = i;
			}
		}

		//backtracking sequence
		for(int i = number - 1; i > 0 ; --i)
		{
			idx[i - 1] = psi[i][idx[i]];
		}

		std::cout<<"sequence: ";
		for(unsigned int i = 0; i < number; ++i)
		{
			std::cout<<idx[i];
		}
		std::cout<<std::endl;
		std::cout<<"the best prob: "<<prob<<std::endl;
		return prob;
	}

}