/**
* Copyright(C) 2009-2012                
* @author Jing HUANG
* @file Edge.h
* @brief 
* @date 1/2/2011
*/

#pragma once
#include <vector>

namespace Etoile
{
	
	/*
	*	Edge class
	*	
	**/

	class Edge
	{
	protected:
		int _v;
		int _w;
		float _weight;
	public:
		Edge(int v, int w, float weight): _v(v), _w(w), _weight(weight)
		{
			
		}
		~Edge()
		{
		}

		int either(){return _v;}
		int other(){return _w;}
		float weight(){return _weight;}
		bool operator <(const Edge& e) const
		{
			return _weight < e._weight;
		}
	
	};

	typedef std::vector<Edge*> Edges;
}