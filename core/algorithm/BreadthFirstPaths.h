/**
* Copyright(C) 2009-2012                
* @author Jing HUANG
* @file BreadthFirstPaths.h
* @brief 
* @date 1/2/2011
*/

#pragma once
#include "Paths.h"
#include <queue>

namespace Etoile
{
	/*
	*	bfs computes the shortest paths (fewest edges) from start S to the other vertices in time propotional to V + E
	**/
	class BreadthFirstPaths : public Paths
	{
	protected:
		bool * _marked;
		int * _edgeTo;
		int * _distance;
	public:
		BreadthFirstPaths(UndirectedGraph * UndirectedGraph, int start) : Paths(UndirectedGraph, start)
		{
			_marked = new bool[_pUndirectedGraph->numberOfVertices()];
			for(int i = 0; i < _pUndirectedGraph->numberOfVertices(); ++i)
			{
				_marked[i] = false;
			}
			_edgeTo = new int[_pUndirectedGraph->numberOfVertices()];
			for(int i = 0; i < _pUndirectedGraph->numberOfVertices(); ++i)
			{
				_edgeTo[i] = -1;
			}
			_distance = new int[_pUndirectedGraph->numberOfVertices()];
			for(int i = 0; i < _pUndirectedGraph->numberOfVertices(); ++i)
			{
				_distance[i] = -1;
			}
			bfs(UndirectedGraph, start);
		}

		~BreadthFirstPaths()
		{
			delete[] _marked;
			delete[] _edgeTo;
			delete[] _distance;
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

		int* distanceTable()
		{
			return _distance;
		}

	private:
		/*
		* BreadthFirstSearch
		*/
		void bfs(UndirectedGraph * UndirectedGraph, int v)
		{
			std::queue<int> queue;
			queue.push(v);
			_marked[v] = true;
			int dis = 0;
			_distance[v] = dis;
			while(!queue.empty())
			{
				int current = queue.front();
				queue.pop();
				Adjacents adj = UndirectedGraph->adjacentsOfVertex(current);
				for(int i = 0; i < adj.size(); ++i)
				{
					int w = adj[i];
					if(!_marked[w])
					{
						queue.push(w);
						_marked[w] = true;
						_edgeTo[w] = current;
						_distance[w] = _distance[current] + 1;
					}
				}

			}

			
		}
	};

}