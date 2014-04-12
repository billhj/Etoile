/**
* Copyright(C) 2009-2012                
* @author Jing HUANG
* @file EdgeWeightedUndirectedGraph.h
* @brief 
* @date 1/2/2011
*/

#pragma once
#include <vector>
#include "Edge.h"

namespace Etoile
{
	
	/*
	*	EdgeWeightedUndirectedGraph class
	*	
	**/

	class EdgeWeightedUndirectedGraph
	{
	protected:
		int _numberOfVertices;
		std::vector<Edges> _adj;
	public:
		EdgeWeightedUndirectedGraph(int nb_v)
		{
			_numberOfVertices = nb_v;
		}
		~EdgeWeightedUndirectedGraph()
		{
			for(int i = 0 ; i < numberOfVertices(); ++i)
			{
				Edges& adj = edges(i);
				for(int j = 0; j < adj.size(); ++j)
				{
					Edge* e = adj[j];
					if(e != NULL) delete e;
				}
				adj.clear();
			}
		}

		virtual void addEdge(Edge* e)
		{
			int v = e->either();
			int w = e->other();
			_adj[v].push_back(e);
			_adj[w].push_back(e);
		}

		Edges& edges(int v)
		{
			return _adj[v];
		}

		Edges edges()
		{
			Edges edges;
			for(int i = 0 ; i < numberOfVertices(); ++i)
			{
				Edges& adj = this->edges(i);
				for(int j = 0; j < adj.size(); ++j)
				{
					Edge* e = adj[j];
					edges.push_back(e);
				}
			}
			return edges;
		}

		int numberOfVertices()
		{
			return _numberOfVertices;
		}

		int numberOfEdges()
		{
			int count = 0;
			for(int v = 0; v < _numberOfVertices; ++v)
			{
				Edges& adj = edges(v);
				count += adj.size();
			}
			return count;
		}
		
		int degreeOfVertex(int v)
		{
			return _adj[v].size();
		}

	};

}