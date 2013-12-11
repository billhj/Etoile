/**
* Copyright(C) 2009-2012                
* @author Jing HUANG
* @file HMMFileLoader.h
* @brief 
* @date 1/2/2011
*/

#pragma once
#include <string>
#include <fstream>
#include "HMMModel.h"
#include "HMMForward.h"
#include "HMMBackward.h"
#include "HMMViterbi.h"

namespace Etoile
{
	class HMMFileLoader
	{
	public:
		HMMFileLoader(void);
		~HMMFileLoader();
		bool loadFromFile(const std::string& filepath, HMMModel* model);


		void test()
		{
			Etoile::HMMFileLoader loader;
			Etoile::HMMModel model;

			bool result = loader.loadFromFile("test.hmm",&model);
			Etoile::HMMForward forward;
			Etoile::HMMBackward backward;
			Etoile::HMMViterbi viterbi;
			double** alpha = new double*[model.getObservationList().size()];
			double** beta = new double*[model.getObservationList().size()];
			double** viterb = new double*[model.getObservationList().size()];
			int** psi = new int*[model.getObservationList().size()];
			int* idx = new int[model.getObservationList().size()];
			for(unsigned int i = 0; i < model.getObservationList().size(); ++i)
			{
				alpha[i] = new double[model.getStates().size()];
				beta[i] = new double[model.getStates().size()];
				viterb[i] = new double[model.getStates().size()];
				psi[i] = new int[model.getStates().size()];
			}

			double prob = forward.forward(&model, model.getObservationList(), &model.getInitialProbabilityPi()[0], alpha);
			double prob2 = backward.backward(&model, model.getObservationList(), &model.getInitialProbabilityPi()[0], beta);
			double prob3 = viterbi.viterbi(&model, model.getObservationList(), &model.getInitialProbabilityPi()[0], viterb, psi, idx);
		}
	protected:
		bool read(std::istream& in, HMMModel* model);
	};

}