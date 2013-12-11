/**
* Copyright(C) 2009-2012                
* @author Jing HUANG
* @file HMMStateTransitionMatrix.cpp
* @brief 
* @date 1/2/2011
*/

#include "HMMStateTransitionMatrix.h"
#include <iostream>
#include <assert.h>

namespace Etoile
{
	HMMStateTransitionMatrix::HMMStateTransitionMatrix(void)
	{
		_init = false;
	}


	HMMStateTransitionMatrix::~HMMStateTransitionMatrix(void)
	{
	}


	void HMMStateTransitionMatrix::initMatrix(int row, int column)
	{
		int size = row * column;
		_row = row;
		_column = column;
		for(int i = 0; i < size; ++i)
		{
			_matrix.push_back(HMMStateTransition());
		}
		_init = true;
	}


	void HMMStateTransitionMatrix::setTransitionProbability(int startState, int endState, HMMStateTransition value)
	{
		if(_init == true)
		{
			_matrix[startState * _column + endState] = value;
		}
		else
			std::cout<<"StateTransitionMatrix has not been initialized !"<<std::endl;
	}

	HMMStateTransition& HMMStateTransitionMatrix::getTransitionProbability(int startState, int endState)
	{
		if(startState >= _row || endState >= _column)
		{
			assert(0 && "getTransitionProbability out of boundry");
		}
		return _matrix[startState * _column + endState];
	}
}