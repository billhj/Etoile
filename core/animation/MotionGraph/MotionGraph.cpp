/**
* Copyright(C) 2009-2012                
* @author Jing HUANG
* @file MotionGraph.cpp
* @brief 
* @date 1/2/2011
*/

#include "MotionGraph.h"
#include <math.h>
#include <iostream>
/**
* @brief For tracking memory leaks under windows using the crtdbg
*/
#if ( defined( _DEBUG ) || defined( DEBUG ) ) && defined( _MSC_VER )
#define _CRTDBG_MAP_ALLOC
#include <crtdbg.h>

#define DEBUG_NEW new( _NORMAL_BLOCK, __FILE__, __LINE__ )
#define new DEBUG_NEW
#endif

namespace Etoile
{
	MotionGraph::MotionGraph(const std::string& name) : Graph(), _name(name)
	{

	}

	MotionGraph::~MotionGraph()
	{

	}

	/*void MotionGraph::addMotion(MotionClip* sequence)
	{
	int clipId = _motionClips.size();
	_motionClips.push_back(sequence);

	int nodeIdStart = _motionNodes.size();
	MotionNode* nodeStart = new MotionNode(nodeIdStart, clipId, 0);
	_motionNodes.push_back(nodeStart);

	int nodeIdEnd = _motionNodes.size();
	MotionNode* nodeEnd = new MotionNode(nodeIdEnd, clipId, sequence->getFrameSequence().size() - 1);
	_motionNodes.push_back(nodeEnd);

	int edgeId = _motionEdges.size();
	MotionEdge* edge = new MotionEdge(edgeId, nodeIdStart, nodeIdEnd);
	_motionEdges.push_back(edge);
	nodeStart->addOutGoingEdge(edgeId);

	std::cout<<"add motion clip: "<<sequence->getName()<<std::endl;
	}*/

	void MotionGraph::addMotionNode(MotionNode* node)
	{
		this->addNode(node);
	}


	void MotionGraph::addMotionEdge(const std::string& start, const std::string& end, float cost)
	{
		std::map<std::string, GraphNode*>::iterator itor = _graphNode.find(start);
		std::map<std::string, GraphNode*>::iterator itor2 = _graphNode.find(end);
		if(itor != _graphNode.end() && itor2 != _graphNode.end())
		{
			this->addEdge(itor->second, itor2->second, cost);
		}

	}


	void MotionGraph::pruningGraph()
	{
		TarjanAlgorithm tAlgo;
		tAlgo.doTarjan(_graphNode);
		_scc = tAlgo.getStronglyConnectedComponents();
		unsigned int size = 0;
		int idx = -1;
		for(unsigned int i = 0; i < _scc.size(); i++)
		{
			if(_scc[i].size() > size)
			{
				size = _scc[i].size();
				idx = i;
			}
		}
		GraphNodes nodes = _scc[idx];

		//reset Node index and clear edges
	/*	for(unsigned int i = 0; i < nodes.size(); ++i)
		{
			nodes[i]->getOutGoingEdge().clear();
			nodes[i]->setId(i);
		}

		//reset Edge new node index, and new edge index
		MotionEdges newEdges;
		for(unsigned int i = 0; i < _motionEdges.size(); ++i)
		{
			MotionEdge* edge = _motionEdges[i];
			int idx0 = MotionNode::getIdx(nodes,_motionNodes[edge->getStartNodeId()]);
			int idx1 = MotionNode::getIdx(nodes,_motionNodes[edge->getEndNodeId()]);
			if( idx0 >= 0 && idx1 >= 0 )
			{
				newEdges.push_back(edge);
				edge->setId(newEdges.size() - 1);
				edge->setStartNode(idx0);
				edge->setEndNode(idx1);
				nodes[idx0]->addOutGoingEdge(newEdges.size() - 1);
			}
			else
			{
				std::cout<<"pruningGraph edge delete :"<<i<<std::endl;
			}
		}*/


	}


	/*void MotionGraph::computeNodesDistance(unsigned int idx1, unsigned int idx2)
	{
	unsigned int size = _motionNodes.size();
	float value = 100;
	_transitionMatrix[size * idx1 + idx2] = value;
	}

	void MotionGraph::createTransitionMatrix()
	{
	unsigned int size = _motionNodes.size();
	for(unsigned int i = 0; i < size; ++i)
	{
	for(unsigned int j = 0; j < size; ++j)
	{
	_transitionMatrix.push_back(1000000);
	computeNodesDistance(i,j);
	}

	}
	}*/

}
