/**
* Copyright(C) 2009-2012                
* @author Jing HUANG
* @file KosarajuSharirSCC.h
* @brief 
* @date 1/2/2011
*/

#pragma once
#include "DirectedGraph.h"
#include "DepthFirstOrder.h"

namespace Etoile
{
	/*
	*	 KosarajuSharirStrongly Connected Components
	**/
	class KosarajuSharirSCC
	{
	protected:
		bool * _marked;
		int * _sccId;
		int _count;

	public:
		KosarajuSharirSCC(DirectedGraph * directedGraph)
		{
			_count = 0;
			_marked = new bool[directedGraph->numberOfVertices()];
			for(int i = 0; i < directedGraph->numberOfVertices(); ++i)
			{
				_marked[i] = false;
			}
			_sccId = new int[directedGraph->numberOfVertices()];
			for(int i = 0; i < directedGraph->numberOfVertices(); ++i)
			{
				_sccId[i] = -1;
			}
			// first do depth first order
			DepthFirstOrder dfo( &(directedGraph->reverse()));
			std::vector<int> reversePost = dfo.reversePost();

			//depthfirstsearch
			for(int i = 0; i < reversePost.size(); ++i)
			{
				if(!_marked[i])
				{
					dfs(directedGraph, i);
					++_count;
				}
			}
			
		}

		~KosarajuSharirSCC()
		{
		}

		virtual bool stronglyConnected(int v0, int v1)
		{
			return _sccId[v0] == _sccId[v1];
		}
		
		/*
		* cc numbers
		**/
		int count(){ return _count;}
		int connectedComponentIdOfVertex(int v)
		{
			return _sccId[v];
		}
	private:
		/*
		* DepthFirstSearch
		*/
		void dfs(UndirectedGraph * undirectedGraph, int v)
		{
			_marked[v] = true;
			_sccId[v] = _count;
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