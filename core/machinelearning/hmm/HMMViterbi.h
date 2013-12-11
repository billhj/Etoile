/**
* Copyright(C) 2009-2012                
* @author Jing HUANG
* @file HMMViterbi.h
* @brief 
* @date 1/2/2011
*/

#pragma once
#include "HMMModel.h"

namespace Etoile
{
	/*	some how  a bit like path finding use Dijkstra algo find the shortest path, but Viterbi is much simpler, since the comparison is in the each state.
	*	The Viterbi algorithm is a dynamic programming algorithm 
	*	for finding the most likely sequence of hidden states 
	*	¨C called the Viterbi path
	**/
	class HMMViterbi
	{
	public:
		HMMViterbi(void);
		~HMMViterbi(void);
		double viterbi(HMMModel* model, std::vector<int>& observation, double* pi, double** alpha, int **psi, int* idx);
	};
}

