/**
* Copyright(C) 2009-2012                
* @author Jing HUANG
* @file MST.h
* @brief 
* @date 1/2/2011
*/

#pragma once
#include "EdgeWeightedUndirectedGraph.h"
#include <algorithm>
#include "UnionFind.h"

namespace Etoile
{
	
	/*
	*	MST class: minimum spanning tree
	*  smallest weights edges
	*	
	**/

	class MST
	{
	protected:
		float _weight;
		Edges _edges;
	public:
		MST(EdgeWeightedUndirectedGraph * graph)
		{
			
		}
		~MST()
		{
		}

		Edges edges()
		{
			return _edges;
		}

		float weight()
		{
			return _weight;
		}
	};


	class KruskalMST : public MST
	{
	public:
		KruskalMST(EdgeWeightedUndirectedGraph * graph) : MST(graph)
		{
			Edges edges = graph->edges();
			std::sort (edges.begin(), edges.end()); 
			WeightedQuickUnionUFWithPathCompression uf(graph->numberOfVertices());

			for(int i = 0; i < edges.size() || _edges.size() < graph->numberOfVertices() - 1; ++i)
			{
				Edge* edge = edges[i];
				int v = edge->either();
				int w = edge->other();
				if(!uf.connected(v,w))
				{
					uf.unite(v,w);
					_edges.push_back(edge);
					_weight += edge->weight();
				}
			}
		}

		~KruskalMST()
		{
		}
	};
}