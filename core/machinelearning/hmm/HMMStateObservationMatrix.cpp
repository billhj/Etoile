/**
* Copyright(C) 2009-2012                
* @author Jing HUANG
* @file HMMStateObservationMatrix.cpp
* @brief 
* @date 1/2/2011
*/

#include "HMMStateObservationMatrix.h"
#include <iostream>
#include <assert.h>

namespace Etoile
{
	HMMStateObservationMatrix::HMMStateObservationMatrix(void)
	{
		_init = false;
	}


	HMMStateObservationMatrix::~HMMStateObservationMatrix(void)
	{
	}


	void HMMStateObservationMatrix::initMatrix(int row, int column)
	{
		int size = row * column;
		_row = row;
		_column = column;
		for(int i = 0; i < size; ++i)
		{
			_matrix.push_back(HMMStateObservation());
		}
		_init = true;
	}


	void HMMStateObservationMatrix::setStateObservationProbability(int state, int observation, HMMStateObservation value)
	{
		if(_init == true)
			_matrix[state * _column + observation] = value;
		else
			std::cout<<"StateObservationMatrix has not been initialized !"<<std::endl;
	}

	HMMStateObservation& HMMStateObservationMatrix::getStateObservationProbability(int state, int observation)
	{
		if(state >= _row || observation >= _column)
		{
			assert(0 && "getStateObservationProbability out of boundry");
		}
		return _matrix[state * _column + observation];
	}
}