/**
* Copyright(C) 2009-2012                
* @author Jing HUANG
* @file DirectedGraphCycleDetection.h
* @brief 
* @date 1/2/2011
*/

#pragma once
#include "DirectedGraph.h"

namespace Etoile
{
	
	/*
	*	DirectedGraphCycleDetection class
	*	
	**/
	class DirectedGraphCycleDetection
	{
	protected:
		bool * _marked;
		bool _isCycled;
	public:
		DirectedGraphCycleDetection(DirectedGraph * graph): _isCycled(false)
		{
			_marked = new bool[graph->numberOfVertices()];
			for(int i = 0; i < graph->numberOfVertices(); ++i)
			{
				_marked[i] = false;
			}
			_isCycled = detectCycle(graph);
		}

		~DirectedGraphCycleDetection()
		{
		}

		bool isCycled()
		{
			return _isCycled;
		}

		static bool detectCycle(DirectedGraph * graph)
		{
		
		}

	private:
		/*
		* DepthFirstSearch
		*/
		void dfs(UndirectedGraph * undirectedGraph, int v)
		{
			_marked[v] = true;
			Adjacents adj = undirectedGraph->adjacentsOfVertex(v);
			for(int i = 0; i < adj.size(); ++i)
			{
				int w = adj[i];
				if(_marked[w] != true)
				{
					dfs(undirectedGraph, w);	
				}
			}
		}
	};

}