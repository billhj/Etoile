/**
* Copyright(C) 2009-2012                
* @author Jing HUANG
* @file HMMBaumWelch.h
* @brief 
* @date 1/2/2011
*/

#pragma once
#include "HMMModel.h"

namespace Etoile
{
	/*
	*	Forward-backward algorithm
	*
	**/
	class HMMBaumWelch  //
	{
	public:
		HMMBaumWelch(void);
		~HMMBaumWelch(void);

		void forward_backward(HMMModel* model, std::vector<int>& observation, double* pi_forward, double* pi_backward, double** lamda, double ***xi);
		void BaumWelch(HMMModel* model, std::vector<int>& observation, double **alpha, double **beta, double **gamma, int *pniter, double *plogprobinit, double *plogprobfinal);
	protected:
		// the probability of the sequence of observations from begin to certain time point
		double forward(HMMModel* model, std::vector<int>& observation, double* pi, double** alpha );
		// the probability of the sequence of observations from end to certain time point
		double backward(HMMModel* model, std::vector<int>& observation, double* pi, double** beta );

		void computeGamma(int observation_number, int statesSize, double** alpha, double** beta, double** gamma);
		void computeXi(HMMModel* model, std::vector<int>& observation, int observation_number, int statesSize, double **alpha, double **beta, double ***xi);
	};
}