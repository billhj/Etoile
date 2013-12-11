/**
* Copyright(C) 2009-2012                
* @author Jing HUANG
* @file MotionGraph.h
* @brief 
* @date 1/2/2011
*/

#pragma once
#include "MotionNode.h"
#include "MotionEdge.h"

namespace Etoile
{

	class MotionGraph : public Graph
	{
	public:
		MotionGraph(const std::string& name);
		~MotionGraph();
		void addMotionNode(MotionNode* node);
		void addMotionEdge(const std::string& start, const std::string& end, float cost);
		
		/**
		* using Tarjan to simplify the graph
		*/
		void pruningGraph();

	private:
		std::string _name;
		std::vector<GraphNodes> _scc;  //strongly connected component
	};

	typedef std::vector<MotionGraph*> MotionGraphes;


}
