/**
* Copyright(C) 2009-2012                
* @author Jing HUANG
* @file MotionEdge.h
* @brief 
* @date 1/2/2011
*/

#pragma once
#include <vector>
#include "../../algo/sort/Graph.h"

namespace Etoile
{

	class MotionEdge : public GraphEdge
	{
	public:
		MotionEdge();
		~MotionEdge();
	private:
	};

	typedef std::vector<MotionEdge*> MotionEdges;


}
