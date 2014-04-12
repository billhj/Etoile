/**
* Copyright(C) 2009-2012                
* @author Jing HUANG
* @file HMMBaumWelch.cpp
* @brief 
* @date 1/2/2011
*/

#include "HMMBaumWelch.h"
#include <iostream>

namespace Etoile
{
	HMMBaumWelch::HMMBaumWelch(void)
	{
	}


	HMMBaumWelch::~HMMBaumWelch(void)
	{
	}

	void HMMBaumWelch::forward_backward(HMMModel* model, std::vector<int>& observation, double* pi_forward, double* pi_backward, double** gamma, double ***xi)
	{
		std::cout<<std::endl<<"start HMM Baum-Welch method:"<<std::endl;
		unsigned int number = observation.size();
		unsigned int statesSize = model->getStates().size();
		double** alpha = new double*[number];
		double** beta = new double*[number];
		for(unsigned int t = 0; t < number; ++t)
		{
			alpha[t] = new double[statesSize];
			beta[t] = new double[statesSize];
		}
		forward(model, observation, pi_forward, alpha);
		backward(model, observation, pi_backward, beta);
		computeGamma(number, statesSize, alpha, beta, gamma);
		computeXi(model, observation, number, statesSize, alpha, beta, xi);
		std::cout<<std::endl<<"End HMM Baum-Welch method:"<<std::endl;
	}

	void HMMBaumWelch::computeGamma(int observation_number, int statesSize, double** alpha, double** beta, double** gamma)
	{
		for(unsigned int t = 0; t < observation_number; ++t)
		{
			for(unsigned int i = 0; i < statesSize; ++i)
			{
				double sum = 0;
				for(unsigned int j = 0; j < statesSize; ++j)
				{
					sum += alpha[t][j] * beta[t][j];
				}
				gamma[t][i] = (alpha[t][i] * beta[t][i]) / sum;
			}
		}

	}


	void HMMBaumWelch::computeXi(HMMModel* model, std::vector<int>& observation, int observation_number, int statesSize, double **alpha, double **beta, double ***xi)
	{
		int i, j;
		int t;
		double sum;
		HMMStateObservationMatrix& stateObservationMatrix = model->getStateObservationMatrix();
		HMMStateTransitionMatrix& stateTransitionMatrix = model->getStateTransitionMatrix();
		for (int t = 1; t <= observation_number - 1; t++) 
		{
			sum = 0.0;	
			for (i = 1; i <= statesSize; i++) 
			{
				for (j = 1; j <= statesSize; j++) 
				{
					xi[t][i][j] = alpha[t][i]*beta[t+1][j]
					*(stateTransitionMatrix.getTransitionProbability(i, j).getProbability())
						*(stateObservationMatrix.getStateObservationProbability(j,observation[t+1]).getProbability());
					sum += xi[t][i][j];
				}
			}
			for (i = 1; i <= statesSize; i++) 
			{
				for (j = 1; j <= statesSize; j++) 
				{
					xi[t][i][j]  /= sum;
				}
			}
		}
	}

	
	void HMMBaumWelch::BaumWelch(HMMModel* model, std::vector<int>& observation, double **alpha, double **beta, double **gamma, int *pniter, double *plogprobinit, double *plogprobfinal)
	{
		HMMStateObservationMatrix& stateObservationMatrix = model->getStateObservationMatrix();
		HMMStateTransitionMatrix& stateTransitionMatrix = model->getStateTransitionMatrix();
	  int   i, j, k;
	  int   t, l = 0;
	  double DELTA = 0.001;
	  double    logprobf, logprobb,  threshold;
	  double    numeratorA, denominatorA;
	  double    numeratorB, denominatorB;

	  double ***xi, *scale;
	  double delta, deltaprev, logprobprev;

	  deltaprev = 10e-70;
	  unsigned int number_observation = observation.size();
	  unsigned int statesSize = model->getStates().size();

	  double * pi_forward; double * pi_backward;
	  logprobf = forward(model, observation, pi_forward, alpha);
	  *plogprobinit = logprobf; /* log P(O |intial model) */
	  logprobb = backward(model, observation, pi_backward, beta);
	  computeGamma(number_observation, statesSize, alpha, beta, gamma);
	  computeXi(model,observation, number_observation, statesSize, alpha, beta, xi);
	  logprobprev = logprobf;

	  do  
	  { 

		/* reestimate frequency of state i in time t=1 */
		for (i = 1; i <= statesSize; i++) 
		  pi_forward[i] = .001 + .999*gamma[1][i];

		/* reestimate transition matrix  and symbol prob in
			each state */
		for (i = 1; i <= statesSize; i++) 
		{ 
		  denominatorA = 0.0;
		  for (t = 1; t <= number_observation - 1; t++) 
			denominatorA += gamma[t][i];

		  for (j = 1; j <= statesSize; j++) 
		  {
			numeratorA = 0.0;
			for (t = 1; t <= number_observation - 1; t++) 
			  numeratorA += xi[t][i][j];
			stateTransitionMatrix.getTransitionProbability(i,j).setProbability(.001 +
					 .999*numeratorA/denominatorA);
		  }

		  denominatorB = denominatorA + gamma[number_observation][i]; 
		  for (k = 1; k <= model->getObservations().size(); k++) 
		  {
			numeratorB = 0.0;
			for (t = 1; t <= number_observation; t++) 
			{
			  if (observation[t] == k) 
				numeratorB += gamma[t][i];
			}

			stateObservationMatrix.getStateObservationProbability(i, k).setProbability(.001 +
					 .999*numeratorB/denominatorB);
		  }
		}

		logprobf = forward(model, observation, pi_forward, alpha);
		logprobb = backward(model, observation, pi_backward, beta);
		computeGamma(number_observation, statesSize, alpha, beta, gamma);
		computeXi(model,observation, number_observation, statesSize, alpha, beta, xi);

		/* compute difference between log probability of 
		  two iterations */
		delta = logprobf - logprobprev; 
		logprobprev = logprobf;
		l++;

	  }
	  while (delta > DELTA); /* if log probability does not 
				  change much, exit */ 
 
	  *pniter = l;
	  *plogprobfinal = logprobf; /* log P(O|estimated model) */
	  
	}

	double HMMBaumWelch::forward(HMMModel* model, std::vector<int>& observation, double* pi, double** alpha)
	{
		std::cout<<std::endl<<"start HMM Forward process:"<<std::endl;
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

	double HMMBaumWelch::backward(HMMModel* model, std::vector<int>& observation, double* pi, double** beta )
	{
		std::cout<<std::endl<<"start HMM Backward process:"<<std::endl;
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