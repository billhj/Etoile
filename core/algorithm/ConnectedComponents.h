/**
* Copyright(C) 2009-2012                
* @author Jing HUANG
* @file ConnectedComponents.h
* @brief 
* @date 1/2/2011
*/

#pragma once
#include "UndirectedGraph.h"

namespace Etoile
{
	/*
	*	
	**/
	class ConnectedComponents
	{
	protected:
		bool * _marked;
		int * _ccId;
		int _count;

	public:
		ConnectedComponents(UndirectedGraph * undirectedGraph)
		{
			_count = 0;
			_marked = new bool[undirectedGraph->numberOfVertices()];
			for(int i = 0; i < undirectedGraph->numberOfVertices(); ++i)
			{
				_marked[i] = false;
			}
			_ccId = new int[undirectedGraph->numberOfVertices()];
			for(int i = 0; i < undirectedGraph->numberOfVertices(); ++i)
			{
				_ccId[i] = -1;
			}

			for(int i = 0; i < undirectedGraph->numberOfVertices(); ++i)
			{
				if(!_marked[i])
				{
					dfs(undirectedGraph, i);
					++_count;
				}
			}
			
		}

		~ConnectedComponents()
		{
		}

		virtual bool connected(int v0, int v1)
		{
			return _ccId[v0] == _ccId[v1];
		}
		
		/*
		* cc numbers
		**/
		int count(){ return _count;}
		int connectedComponentIdOfVertex(int v)
		{
			return _ccId[v];
		}
	private:
		/*
		* DepthFirstSearch
		*/
		void dfs(UndirectedGraph * undirectedGraph, int v)
		{
			_marked[v] = true;
			_ccId[v] = _count;
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