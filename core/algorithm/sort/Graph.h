/**
* Copyright(C) 2009-2012                
* @author Jing HUANG
* @file Graph.h
* @brief 
* @date 1/2/2011
*/

#pragma once
#include <map>
#include <list>
#include <vector>
#include <assert.h>
#include <string>
#include <iostream>
#include "BinaryHeap.h"

namespace Etoile
{
	class GraphNode;
	class GraphEdge;
	class TarjanAlgorithm;
	class DijkstraAlgorithm;
	class Graph;

	class GraphNode
	{
		std::string _name;
		std::list<GraphEdge*> _edges;
	public:
		GraphNode(const std::string& name): _name(name)
		{	
		}	
		std::string getName(){return _name;}
		std::list<GraphEdge*>& getEdges()
		{
			return _edges;
		}
		void addEdge(GraphEdge* e)
		{
			_edges.push_back(e);
		}
	};
	typedef std::vector<GraphNode*> GraphNodes;

	class GraphEdge
	{
		double _cost;
		GraphNode* _pNext;
		GraphNode* _pPrevious;
	public:
		GraphEdge()
		{
		}
		GraphEdge(GraphNode* previous, GraphNode* nextNode, double cost): _pPrevious(previous), _pNext(nextNode), _cost(cost)
		{
		}
		void connectFromPreviousNode(GraphNode* previous)
		{
			_pPrevious = previous;
		}
		GraphNode* getPreviousNode()
		{
			return _pPrevious;
		}

		void connectToNextNode(GraphNode* next)
		{
			_pNext = next;
		}
		GraphNode* getNextNode()
		{
			return _pNext;
		}
		void setPathCost(double cost)
		{	
			_cost = cost;
		}
		double getPathCost()
		{
			return _cost;
		}
	};

	/*
	*	TarjanAlgorithm: search for strongly contected components
	*	complexity = O( node + edge )
	**/

	class TarjanAlgorithm
	{
		std::map<std::string, int> _tarjanIndices;
		std::map<std::string, int> _tarjanLowLinks;
		std::vector<GraphNode*> _stack;
		std::vector<GraphNodes> _stronglyConnectedComponents;
		int _tarjanIdx;
	public:
		std::vector<GraphNodes> getStronglyConnectedComponents()
		{
			return _stronglyConnectedComponents; 
		}

		/*
		*	check the node for his strongly connected component,  (depth first search)
		*	recursively check all the nextNodes, if a nextNode has a cycle back to its parent
		*	then find a strongly connected component, 
		*	since all the connected nodes will be checked, then could have several cycles in a node check process, 
		*	the root node for such component is the smallest index node.
		**/
		void doTarjan(std::map<std::string, GraphNode*> graphNode)
		{
			_stronglyConnectedComponents.clear();
			_tarjanIndices.clear();
			_tarjanLowLinks.clear();
			_stack.clear();
			_tarjanIdx = 0;
			std::map<std::string, GraphNode*>::iterator itor;
			for(itor = graphNode.begin(); itor != graphNode.end(); ++itor)
			{
				_tarjanIndices[itor->first] = -1;
				_tarjanLowLinks[itor->first] = -1;
			}

			for(itor = graphNode.begin(); itor != graphNode.end(); ++itor)
			{
				if(_tarjanIndices[itor->first] == -1)
				{
					GraphNode* node = itor->second;
					doTarjan(node);
				}
			}
		}

	private:
		bool isContained(std::vector<GraphNode*> list, GraphNode* node)
		{
			for(unsigned int i = 0; i < list.size(); ++i )
			{
				if(list[i] == node)
				{
					return true;
				}
			}
			return false;
		}

		/*
		*	check the node for his strongly connected component,  (depth first search)
		*	recursively check all the nextNodes, if a nextNode has a cycle back to its parent
		*	then find a strongly connected component, 
		*	since all the connected nodes will be checked, then could have several cycles in a node check process, 
		*	the root node for such component is the smallest index node.
		**/
		void doTarjan(GraphNode* node)
		{
			_tarjanIndices[node->getName()] = _tarjanIdx;
			_tarjanLowLinks[node->getName()] = _tarjanIdx;
			_tarjanIdx++;
			_stack.push_back(node);

			std::list<GraphEdge*>& edges = node->getEdges();
			std::list<GraphEdge*>::iterator itor;
			for(itor = edges.begin(); itor != edges.end(); ++itor)
			{
				GraphEdge* edge = *itor;
				GraphNode* nextNode = edge->getNextNode();
				if(_tarjanIndices[nextNode->getName()] == -1)
				{
					doTarjan(nextNode);
					_tarjanLowLinks[node->getName()] = std::min<int>(_tarjanLowLinks[node->getName()], _tarjanLowLinks[nextNode->getName()]);
				}
				else if(isContained(_stack, nextNode))
				{
					_tarjanLowLinks[node->getName()] = std::min<int>(_tarjanLowLinks[node->getName()], _tarjanIndices[nextNode->getName()]);
				}
			}

			if(_tarjanIndices[node->getName()] == _tarjanLowLinks[node->getName()])
			{
				GraphNode* n;
				GraphNodes component;
				do
				{
					n = _stack[_stack.size() - 1];
					_stack.pop_back();
					component.push_back(n);
				}while(n != node);
				if(component.size() > 1)
				{
					_stronglyConnectedComponents.push_back(component);
				}
			}
		}
	};

	/*
	*	DijkstraAlgorithm: search for the shortest path in the graph
	**/
	class DijkstraAlgorithm
	{
		//use to build a heap for marking the small distance node
		struct DistanceNode
		{
			std::map<std::string, double>* _pDistPointer;
			std::string _name;

			bool operator <(const DistanceNode& v) const
			{
				double distance = (*_pDistPointer)[_name];
				double distance2 = (*v._pDistPointer)[v._name];
				if(distance < distance2)
				{
					return true;
				}
				else
				{
					return false;
				}
			}
		};

		double _maxDistance;
	public:
		DijkstraAlgorithm(): _maxDistance(9999999999){}
		/*
		*	search for the shortest path
		*	each iteration: choose the shortest path node in this step, then remove from stack (the basic idea is to find shortest path for all nodes with accumulation)
		*	which can not get shorter path in the next step by passing through any other node, since through other nodes are further.
		*	when such node is the end node, then the path is found
		**/
		bool doDijkstra(std::map<std::string, GraphNode*> graphNode, std::string start, std::string end, std::list<std::string>& path)
		{
			std::map<std::string, double> dist;
			std::map<std::string, std::string> previous;
			BinaryHeap<DistanceNode> qHeap; // Q: unoptimized node - thus are in Q
			//initiate all distance to MaxDistance, sauf start to start
			std::map<std::string, GraphNode*>::iterator itor;
			for(itor = graphNode.begin(); itor != graphNode.end(); ++itor)
			{
				dist[itor->first] = _maxDistance;
				previous[itor->first] = "";
				DistanceNode dnode;
				dnode._name = itor->first;
				dnode._pDistPointer = &dist;
				qHeap.insertHeapNode(dnode);
			}
			dist[start] = 0;
			GraphNode* current = graphNode[start];
			std::list<GraphEdge*>& edges = current->getEdges();
			std::list<GraphEdge*>::iterator edgeItor;
			for(edgeItor = edges.begin(); edgeItor != edges.end(); ++edgeItor)
			{
				GraphEdge* edge = *edgeItor;
				GraphNode* nextNode = edge->getNextNode();
				dist[nextNode->getName()] = edge->getPathCost();
				previous[nextNode->getName()] = start;
			}

			std::string smallestDistanceNode = start;
			while(!qHeap.empty())
			{
				//reordering the heap : root smallest
				qHeap.buildMinHeap();
				//each iteration removes the shortest path node in this step, 
				//which can not get shorter path in the next step by passing through any other node.
				//TODO can be optimized since deleteHeapRoot will update Once the hierarchy.
				qHeap.deleteHeapRoot(); 

				smallestDistanceNode = qHeap.getBinaryHeapNodeByIndex(0)._name;
				//if the smallestDistanceNode == our target that we find the shortest path
				// then just use previous node to find push in whole path
				if(smallestDistanceNode.compare(end) == 0)
				{
					std::string pathNode = smallestDistanceNode;
					while(pathNode.compare(start) != 0)
					{
						if(pathNode.empty()) assert(0);
						path.push_front(pathNode);
						pathNode = previous[pathNode];
					}
					path.push_front(pathNode);
					return true;
				}

				//if the smallest distance is not smaller than maxDistance, then there is no connexion 
				if(dist[smallestDistanceNode] >= _maxDistance)
				{
					return false; //all remaining vertices are inaccessible from start source node
				}

				//use current smallest node to test if there is shorter combination for its neighbours, update it if the combination is shorter
				GraphNode* current = graphNode[smallestDistanceNode];
				std::list<GraphEdge*>& edges = current->getEdges();
				std::list<GraphEdge*>::iterator edgeItor;
				for(edgeItor = edges.begin(); edgeItor != edges.end(); ++edgeItor)
				{
					GraphEdge* edge = *edgeItor;
					GraphNode* nextNode = edge->getNextNode();
					//new combination Distance compare to original short distance in the dist[]
					double accumulateDistance = dist[smallestDistanceNode] + edge->getPathCost();
					if(accumulateDistance < dist[nextNode->getName()])
					{
						dist[nextNode->getName()] = accumulateDistance;
						previous[nextNode->getName()] = smallestDistanceNode;
					}
				}

			}
			return false;
		}

		double getDistance(GraphNode* start, GraphNode* end)
		{
			std::list<GraphEdge*>& edges = start->getEdges();
			std::list<GraphEdge*>::iterator edgeItor;
			for(edgeItor = edges.begin(); edgeItor != edges.end(); ++edgeItor)
			{
				GraphEdge* edge = *edgeItor;
				GraphNode* nextNode = edge->getNextNode();
				if(end == nextNode)
				{
					return edge->getPathCost();
				}
			}
			return _maxDistance;
		}
	};

	/*
	*	Graph class
	*	
	**/

	class Graph
	{
	protected:
		std::map<std::string, GraphNode*> _graphNode;
	public:
		Graph()
		{
		}
		~Graph()
		{
			std::map<std::string, GraphNode*>::iterator itor;
			for(itor = _graphNode.begin(); itor != _graphNode.end(); ++itor)
			{
				if(itor->second != NULL) delete itor->second;
			}
			_graphNode.clear();
		}

		GraphNode* createNode(const std::string& name)
		{
			GraphNode* node = new GraphNode(name);
			addNode(node);
			return node;
		}

		void addNode(GraphNode* node)
		{
			_graphNode[node->getName()] = node;
		}

		void addEdge(GraphNode* start, GraphNode* end, double cost)
		{
			start->addEdge(new GraphEdge(start, end, cost));
		}

		std::map<std::string, GraphNode*>& getNodes(){return _graphNode;}
	};

	typedef Graph Graphd;
}