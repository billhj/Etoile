/**
* Copyright(C) 2009-2012                
* @author Jing HUANG
* @file BasicGraph.h
* @brief 
* @date 1/2/2011
*/

#pragma once
#include <vector>
#include <iostream>

namespace Etoile
{
	class BasicNode;
	class BasicEdge;
	class BasicGraph;

	struct NodeType
	{
		NodeType()
		{
			_xPos = 0;
			_yPos = 0;
			_w = 80, _h = 120;
		}
		//SocketTYPE _type;
		std::string _name;
		std::string _description;
		int _icon;
		struct color
		{
			color(int r, int g, int b, int a): _r(r), _g(g), _b(b), _a(a)
			{
			
			}
			color(): _r(0), _g(0), _b(0), _a(0){}
			int _r;
			int _g;
			int _b;
			int _a;
		} _color;

		float _xPos, _yPos;
		float _w, _h;
	};

	class BasicNode
	{
	public:
		BasicNode(): _pGraph(NULL)
		{
			_type._color = NodeType::color(180, 180, 180, 255);
		}
		virtual ~BasicNode(){} 
		NodeType& getType(){return _type;}
		virtual void setType(NodeType type)
		{
			_type = type;
		}
		void setGraph(BasicGraph *graph)
		{
			_pGraph = graph;
		}
		BasicGraph * getGraph(){return _pGraph;}
		const std::string getName(){return this->_type._name;}
		void setName(const std::string& name){_type._name = name;}
		//virtual void excute(){}
	protected:
		NodeType _type;
		BasicGraph *_pGraph;
	/*protected:
		std::vector<BasicEdge*> _inedges;
		std::vector<BasicEdge*> _outedges;*/
	};
	typedef std::vector<BasicNode*> Nodes;

	class BasicEdge
	{
	public:
		BasicEdge(): _phead(NULL), _ptail(NULL), _degree(0){}
		BasicEdge(BasicNode* head, BasicNode* tail): _phead(head), _ptail(tail), _degree(0){}
		virtual ~BasicEdge(){}
		void setHead(BasicNode* head){_phead = head;}
		BasicNode* getHead(){return _phead;} 
		void setTail(BasicNode* tail){_ptail = tail;}
		BasicNode* getTail(){return _ptail;} 
		int connect()
		{
			_degree++;
			return _degree;
		}
		int disconnect()
		{
			_degree--;	
			return _degree;
		}
		int getDegree(){return _degree;}
	protected:
		BasicNode *_phead, *_ptail;
		int _degree;
	};

	typedef std::vector<BasicEdge*> Edges;

	class BasicGraph
	{
	public:
		BasicGraph()
		{
		}
		~BasicGraph()
		{
			//clear();
		}
		void addNode(BasicNode* node)
		{
			_nodes.push_back(node);
			node->setGraph(this);
		}

		void connectEdge(BasicNode* from, BasicNode* to)
		{
			for(unsigned int i = 0; i < _edges.size(); ++i)
			{
				BasicEdge *edge = _edges[i];
				if(edge->getHead() == from && edge->getTail() == to)
				{
					edge->connect();
#if defined(_DEBUG) || defined(DEBUG)
					std::cout<<"Edge: "<<(from->getType()._name).c_str()<<" "<<(to->getType()._name).c_str()<<" connected! " <<edge->getDegree() <<std::endl;
#endif
					return;
					//return edge;
				}
			}
			BasicEdge *edge = new BasicEdge(from, to);
			edge->connect();
			_edges.push_back(edge);
#if defined(_DEBUG) || defined(DEBUG)
			std::cout<<"Edge: "<<(from->getType()._name).c_str()<<" "<<(to->getType()._name).c_str()<<" connected! " <<edge->getDegree() <<std::endl;
#endif
			//return edge;
		}

		void disconnectEdge(BasicNode* from, BasicNode* to)
		{
			for(unsigned int i = 0; i < _edges.size(); ++i)
			{
				BasicEdge *edge = _edges[i];
				if(edge->getHead() == from && edge->getTail() == to)
				{
					int degree = edge->disconnect();
#if defined(_DEBUG) || defined(DEBUG)
					std::cout<<"Edge: "<<from->getType()._name.c_str() <<" "<<to->getType()._name.c_str()<<" disconnected! " <<edge->getDegree() <<std::endl;
#endif
					if(degree == 0)
					{
						_edges.erase(_edges.begin() + i);
						delete edge;
						return;
					}
				}
			}
			
		}

		BasicEdge* getEdge(BasicNode* from, BasicNode* to)
		{
			for(unsigned int i = 0; i < _edges.size(); ++i)
			{
				BasicEdge *edge = _edges[i];
				if(edge->getHead() == from && edge->getTail() == to)
				{
					return edge;
				}
			}
			return NULL;
		}

		int getNodeIndex(BasicNode* node)
		{
			for(unsigned int i = 0; i < _nodes.size(); ++i)
			{
				BasicNode *n = _nodes[i];
				if(n == node)
				{
					return i;
				}
			}
			return -1;
		}

		std::vector<BasicNode*>& getNodes(){return _nodes;}
		std::vector<BasicEdge*>& getEdges(){return _edges;}

		virtual void removeNode(BasicNode* node)
		{
			int idx = getNodeIndex(node);
			if(idx != -1)
			{
				removeNodeCorrespondingEdges(node);
				_nodes.erase(_nodes.begin() + idx);
				delete node;
			}
		}

		void removeNodeCorrespondingEdges(BasicNode* node)
		{
			if(node == NULL) return;
			for(unsigned int i = 0; i < _edges.size();)
			{
				BasicEdge* edge = _edges[i];
				BasicNode* node1 = edge->getHead();
				BasicNode* node2 = edge->getTail();
				if(node1 == node || node2 == node)
				{
					disconnectEdge(node1, node2);
				}else{
					 ++i;
				}
			}
		}

		void clear()
		{
			for(unsigned int i = 0; i < _edges.size(); ++i)
			{
				BasicEdge* edge = _edges[i];
				if(edge != NULL) delete edge;
			}
			_edges.clear();

			for(unsigned int i = 0; i < _nodes.size(); ++i)
			{
				BasicNode *n = _nodes[i];
				if(n != NULL)
				{
					delete n;
				}
			}
			_nodes.clear();
		}

	protected:
		std::vector<BasicNode*> _nodes;
		std::vector<BasicEdge*> _edges;
	};

}