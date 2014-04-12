/**
* Copyright(C) 2009-2012                
* @author Jing HUANG
* @file HMMStateTransitionMatrix.h
* @brief 
* @date 1/2/2011
*/

#pragma once
#include "HMMStateTransition.h"
#include <vector>

namespace Etoile
{
	/*
	*
	*	the probabilities between states to states by transitions
	**/

	class HMMStateTransitionMatrix
	{
	public:
		HMMStateTransitionMatrix(void);
		~HMMStateTransitionMatrix(void);
		void initMatrix(int row, int column);
		bool isInitialized(){return _init;}
		void setTransitionProbability(int startState, int endState, HMMStateTransition value);
		HMMStateTransition& getTransitionProbability(int startState, int endState);

	private:
		std::vector<HMMStateTransition> _matrix;
		int _row, _column;
		bool _init;
	};
}
