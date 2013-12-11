/**
* Copyright(C) 2009-2012                
* @author Jing HUANG
* @file Paths.h
* @brief 
* @date 1/2/2011
*/

#pragma once
#include "UndirectedGraph.h"

namespace Etoile
{
	/*
	*	dfs marks all vertices connected to start S in time propotional to the sum of their degrees.
	**/
	class Paths
	{
	protected:
		int _start;

	public:
		Paths(int start) :_start(start)
		{
		}

		~Paths()
		{
		}

		virtual bool hasPathTo(int v) = 0;
		virtual std::vector<int> pathTo(int v) = 0;
	};

}