/**
* Copyright(C) 2009-2012                
* @author Jing HUANG
* @file UndirectedGraph.h
* @brief 
* @date 1/2/2011
*/

#pragma once
#include <vector>

namespace Etoile
{
	
	/*
	*	UndirectedGraph class
	*	
	**/
	typedef std::vector<int> Adjacents;

	class UndirectedGraph
	{
	protected:
		int _numberOfVertices;
		std::vector<Adjacents> _adj;
	public:
		UndirectedGraph(int nb_v)
		{
			_numberOfVertices = nb_v;
			_adj.resize(nb_v);
		}
		~UndirectedGraph()
		{
		}

		virtual void addEdge(int v0, int v1)
		{
			_adj[v0].push_back(v1);
			_adj[v1].push_back(v0);
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
				std::vector<int>& adj = adjacentsOfVertex(v);
				count += adj.size();
			}
			return count;
		}
		
		Adjacents adjacentsOfVertex(int v)
		{
			return _adj[v];
		}

		int degreeOfVertex(int v)
		{
			return adjacentsOfVertex(v).size();
		}

		int maxDegree()
		{
			int max = 0;
			for(int i = 0 ; i < numberOfVertices(); ++i)
			{
				int d = degreeOfVertex(i);
				if(d > max)
				{
					max = d;
				}
			}
			return max;
		}

		virtual float averageDegree()
		{
			return (float)2.0 * (float)numberOfEdges() / (float)numberOfVertices();
		}

		int numberOfSelfLoopOFVertex(int v)
		{
			int count = 0;
			Adjacents& adj = adjacentsOfVertex(v);
			for(int i = 0; i < adj.size(); ++i)
			{
				int vd  = adj[i];
				if(vd == v)
				{
					count++;
				}
			}
			return count;
		}

		int numberOfSelfLoop()
		{
			int count = 0;
			for(int i = 0 ; i < numberOfVertices(); ++i)
			{
				count += numberOfSelfLoopOFVertex(i);
			}
			return count;
		}
	};

}