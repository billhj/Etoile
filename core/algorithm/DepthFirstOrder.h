/**
* Copyright(C) 2009-2012                
* @author Jing HUANG
* @file DepthFirstOrder.h
* @brief 
* @date 1/2/2011
*/

#pragma once
#include "DirectedGraph.h"

namespace Etoile
{
	/*
	*	topoligical sorting using depth first ordering, then reverse it  (make sure no cycle)
	*	dfs marks all vertices connected to start S in time propotional to the sum of their degrees.
	**/
	class DepthFirstOrder
	{
	protected:
		bool * _marked;
		std::vector<int> _reversePost;
	public:
		DepthFirstOrder(DirectedGraph * graph)
		{
			_marked = new bool[graph->numberOfVertices()];
			for(int i = 0; i < graph->numberOfVertices(); ++i)
			{
				_marked[i] = false;
			}
			
			for(int i = 0; i < graph->numberOfVertices(); ++i)
			{
				if(!_marked[i])
				{
					dfs(graph, i);
				}
			}
		}

		~DepthFirstOrder()
		{
			delete[] _marked;
		}


		std::vector<int> reversePost()
		{
			return _reversePost;
		}

		std::vector<int> order()
		{
			std::vector<int> order;
			int size = _reversePost.size();
			for(int i = 0; i < size; ++i)
			{
				order.push_back(_reversePost[size - 1- i]);
			}
			return order;
		}
		
	private:
		/*
		* DepthFirstSearch
		*/
		void dfs(DirectedGraph * directedGraph, int v)
		{
			_marked[v] = true;
			Adjacents adj = directedGraph->adjacentsOfVertex(v);
			for(int i = 0; i < adj.size(); ++i)
			{
				int w = adj[i];
				if(!_marked[w])
				{
					dfs(directedGraph, w);
				}
			}
			_reversePost.push_back(w);
		}
	};

}