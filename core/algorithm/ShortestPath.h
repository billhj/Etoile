/**
* Copyright(C) 2009-2012                
* @author Jing HUANG
* @file DirectedEdge.h
* @brief 
* @date 1/2/2011
*/

#pragma once
#include <vector>

namespace Etoile
{
	
	/*
	*	DirectedEdge class
	*	
	**/

	class DirectedEdge
	{
	protected:
		int _v;
		int _w;
		float _weight;
	public:
		DirectedEdge(int v, int w, float weight): _v(v), _w(w), _weight(weight)
		{
			
		}
		~DirectedEdge()
		{
		}

		int from(){return _v;}
		int to(){return _w;}
		float weight(){return _weight;}
		bool operator <(const DirectedEdge& e) const
		{
			return _weight < e._weight;
		}
	
	};

	typedef std::vector<DirectedEdge*> DirectedEdges;


	/*
	*	EdgeWeightedDirectedGraph class
	*	
	**/

	class EdgeWeightedDirectedGraph
	{
	protected:
		int _numberOfVertices;
		std::vector<DirectedEdges> _adj;
	public:
		EdgeWeightedDirectedGraph(int nb_v)
		{
			_numberOfVertices = nb_v;
		}
		~EdgeWeightedDirectedGraph()
		{
			for(int i = 0 ; i < numberOfVertices(); ++i)
			{
				DirectedEdges& adj = edges(i);
				for(int j = 0; j < _adj.size(); ++j)
				{
					DirectedEdge* e = adj[j];
					if(e != NULL) delete e;
				}
				adj.clear();
			}
		}

	
		virtual void addEdge(DirectedEdge* e)
		{
			int v = e->from();
			_adj[v].push_back(e);
		}

		//edge pointing from v
		DirectedEdges& edges(int v)
		{
			return _adj[v];
		}

		DirectedEdges edges()
		{
			DirectedEdges edges;
			for(int i = 0 ; i < numberOfVertices(); ++i)
			{
				DirectedEdges& adj = this->edges(i);
				for(int j = 0; j < adj.size(); ++j)
				{
					DirectedEdge* e = adj[j];
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
				DirectedEdges& adj = edges(v);
				count += adj.size();
			}
			return count;
		}
		
		int degreeOfVertex(int v)
		{
			return _adj[v].size();
		}

	};



	class SP
	{
	public:
		SP(EdgeWeightedDirectedGraph& graph, int source)
		{
		
		}

		virtual float distTo(int v) = 0;
		virtual bool hasPathTo(int v) = 0;
		virtual DirectedEdges pathTo(int v) = 0;
	};
}