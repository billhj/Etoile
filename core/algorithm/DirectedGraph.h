/**
* Copyright(C) 2009-2012                
* @author Jing HUANG
* @file DirectedGraph.h
* @brief 
* @date 1/2/2011
*/

#pragma once
#include "UndirectedGraph.h"

namespace Etoile
{
	
	/*
	*	DirectedGraph class
	*	
	**/
	class DirectedGraph : public UndirectedGraph
	{
	public:
		DirectedGraph(int nb_v) : UndirectedGraph(nb_v)
		{
		}
		~DirectedGraph()
		{
		}

		virtual void addEdge(int s, int e) override
		{
			_adj[s].push_back(e);
		}

		
		DirectedGraph reverse()
		{
			DirectedGraph reverse(_numberOfVertices);
			for(int i = 0; i < _numberOfVertices; ++i)
			{
				Adjacents adj = _adj[i];
				for(int w = 0; w < adj.size(); ++w)
				{
					reverse.addEdge(w, i);
				}
			}
			return reverse;
		}

		virtual float averageDegree() override
		{
			return (float)numberOfEdges() / (float)numberOfVertices();
		}

	};

}