/**
* Copyright(C) 2009-2012                
* @author Jing HUANG
* @file HMMStateObservationMatrix.h
* @brief 
* @date 1/2/2011
*/

#pragma once
#include <vector>
#include "HMMStateObservation.h"

namespace Etoile
{
	/*
	*
	*	the probabilities between states to observations
	**/

	class HMMStateObservationMatrix
	{
	public:
		HMMStateObservationMatrix(void);
		~HMMStateObservationMatrix(void);
		void initMatrix(int row, int column);
		bool isInitialized(){return _init;}
		void setStateObservationProbability(int state, int observation, HMMStateObservation value);
		HMMStateObservation& getStateObservationProbability(int state, int observation);

	private:
		std::vector<HMMStateObservation> _matrix;
		int _row, _column;
		bool _init;
	};

}