/**
* Copyright(C) 2009-2012                
* @author Jing HUANG
* @file DepthFirstPaths.h
* @brief 
* @date 1/2/2011
*/

#pragma once
#include "Paths.h"

namespace Etoile
{
	/*
	*	dfs marks all vertices connected to start S in time propotional to the sum of their degrees.
	*  find all the vertices in the graph that are connected to the S
	**/
	class DepthFirstPaths : public Paths
	{
	protected:
		bool * _marked;
		int * _edgeTo;
	public:
		DepthFirstPaths(UndirectedGraph * undirectedGraph, int start) : Paths(start)
		{
			_marked = new bool[undirectedGraph->numberOfVertices()];
			for(int i = 0; i < undirectedGraph->numberOfVertices(); ++i)
			{
				_marked[i] = false;
			}
			_edgeTo = new int[undirectedGraph->numberOfVertices()];
			for(int i = 0; i < undirectedGraph->numberOfVertices(); ++i)
			{
				_edgeTo[i] = -1;
			}

			dfs(undirectedGraph, start);
		}

		~DepthFirstPaths()
		{
			delete[] _marked;
			delete[] _edgeTo;
		}

		virtual bool hasPathTo(int v) override
		{
			return _marked[v];
		}

		virtual std::vector<int> pathTo(int v) override
		{
			std::vector<int> path;
			if(hasPathTo(v))
			{
				for(int x = v; x != _start; x = _edgeTo[x])
				{
					path.insert(path.begin(), x);
				}
				path.insert(path.begin(), _start);
			}

			return path;
		}

		bool* markedTable()
		{
			return _marked;
		}

		int* edgeToTable()
		{
			return _edgeTo;
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
				if(!_marked[w])
				{
					dfs(undirectedGraph, w);
					_edgeTo[w] = v;
				}
			}
		}
	};

}