/**
* Copyright(C) 2009-2012                
* @author Jing HUANG
* @file GraphScene.cpp
* @brief 
* @date 1/2/2011
*/

#include "GraphScene.h"
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
	GraphScene::GraphScene()
	{
	}

	bool GraphScene::connect(SocketConnection* conn)
	{
		_connections.push_back(conn);

		//connect edge
		this->connectEdge(conn->getFromNode(), conn->getToNode());

		//emit signal
		ESignalEmitter<GraphScene>::signalEmit(this);
		conn->getFromSocket()->perform(new SocketConnectedSignal(conn));

#if defined(_DEBUG) || defined(DEBUG)
		std::cout<<"SocketConnection: "<<conn->getFromNode()->getType()._name.c_str() <<" "<<conn->getToNode()->getType()._name.c_str()<<" connected!"<<std::endl;
#endif
		return true;
	}

	bool GraphScene::disconnect(SocketConnection* conni)
	{
		for(unsigned int i = 0; i < _connections.size(); ++i)
		{
			SocketConnection* conn = _connections[i];
			if(conn == conni)
			{

				//disconnect edge
				this->disconnectEdge(conn->getFromNode(), conn->getToNode());
#if defined(_DEBUG) || defined(DEBUG)
				std::cout<<"SocketConnection: "<<conn->getFromNode()->getType()._name.c_str() <<" "<<conn->getToNode()->getType()._name.c_str()<<" disconnected!"<<std::endl;
#endif
				//disconnect socket
				_connections.erase(_connections.begin() + i);

				//emit signal
				ESignalEmitter<GraphScene>::signalEmit(this);
				conn->getFromSocket()->perform(new SocketDisconnectedSignal(conn));

				delete conn;
				return true;
			}
		}
		return false;
	}

	bool GraphScene::connect(OutputSocket *output, InputSocket *input)
	{
		if(output->verify(input))
		{
			//connect socket
			SocketConnection* conn = new SocketConnection();
			conn->setFromSocket(output);
			conn->setToSocket(input);
			_connections.push_back(conn);

			//connect edge
			this->connectEdge(output->getNode(), input->getNode());

			//emit signal
			ESignalEmitter<GraphScene>::signalEmit(this);
			conn->getFromSocket()->perform(new SocketConnectedSignal(conn));
#if defined(_DEBUG) || defined(DEBUG)
			std::cout<<"SocketConnection: "<<output->getType()._name.c_str() <<" "<<input->getType()._name.c_str()<<" connected!"<<std::endl;
#endif
			return true;
		}
		return false;
	}

	bool GraphScene::disconnect(OutputSocket *output, InputSocket *input)
	{
		if(output->verify(input))
		{
			for(unsigned int i = 0; i < _connections.size(); ++i)
			{
				SocketConnection* conn = _connections[i];
				if(conn->getFromSocket() == output && conn->getToSocket() == input)
				{
					//disconnect edge
					this->disconnectEdge(output->getNode(), input->getNode());
#if defined(_DEBUG) || defined(DEBUG)
					std::cout<<"SocketConnection: "<<output->getType()._name.c_str() <<" "<<input->getType()._name.c_str()<<" disconnected!"<<std::endl;
#endif
					//disconnect socket
					_connections.erase(_connections.begin() + i);

					//emit signal
					ESignalEmitter<GraphScene>::signalEmit(this);
					conn->getFromSocket()->perform(new SocketDisconnectedSignal(conn));
					delete conn;
					return true;
				}
			}
		}
		return false;
	}

	SocketConnection* GraphScene::getSocketConnection(OutputSocket *output, InputSocket *input)
	{
		if(output->verify(input))
		{
			for(unsigned int i = 0; i < _connections.size(); ++i)
			{
				SocketConnection* conn = _connections[i];
				if(conn->getFromSocket() == output && conn->getToSocket() == input)
				{
					return conn;
				}
			}
		}
		return NULL;
	}

	SocketConnection* GraphScene::getSocketConnection(InputSocket *input)
	{
		for(unsigned int i = 0; i < _connections.size(); ++i)
		{
			SocketConnection* conn = _connections[i];
			if(conn->getToSocket() == input)
			{
				return conn;
			}
		}
		return NULL;
	}

	bool GraphScene::isConnected(OutputSocket *output, InputSocket *input)
	{
		SocketConnection* conn = getSocketConnection(output, input);
		if(conn != NULL)
		{
			return true;
		}
		return false;
	}

	

	void GraphScene::removeNode(BasicNode* node)
	{
		SocketNode *snode = dynamic_cast<SocketNode*>(node);
		if(snode == NULL)
		{
			BasicGraph::removeNode(node);
		}
		else
		{
			int idx = getNodeIndex(node);
			if(idx != -1)
			{
				removeSocketCorrespondingConnections(snode);
				_nodes.erase(_nodes.begin() + idx);
				delete snode;
			}
		}
	}

	void GraphScene::removeSocketCorrespondingConnections(SocketNode* node)
	{
		if(node == NULL) return;
		for(unsigned int i = 0; i < _connections.size();)
		{
			SocketConnection* conn = _connections[i];
			Etoile::OutputSocket *socketo = conn->getFromSocket();
			Etoile::InputSocket *socketi = conn->getToSocket();
			if(socketo->getNode() == node || socketi->getNode() == node){
				disconnect(socketo, socketi);
			}else
			{
				++i;
			}
		}
	}
}