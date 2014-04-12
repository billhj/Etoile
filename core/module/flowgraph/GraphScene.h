/**
* Copyright(C) 2009-2012                
* @author Jing HUANG
* @file GraphScene.h
* @brief 
* @date 1/2/2011
*/

#pragma once
#include "SocketConnection.h"
#include "module/signals/ESignal.h"

namespace Etoile
{
	class SocketConnectedSignal : public ESignal<SocketConnection>
	{
	public:
		SocketConnectedSignal(SocketConnection* conn): ESignal<SocketConnection>(conn)
		{
		}

		void setConnection(SocketConnection* conn)
		{
			this->set(conn);
		}

		SocketConnection* getConnection(){return this->get();}
	};

	class SocketDisconnectedSignal : public ESignal<SocketConnection>
	{
	public:
		SocketDisconnectedSignal(SocketConnection* conn): ESignal<SocketConnection>(conn)
		{
		}
		void setConnection(SocketConnection* conn)
		{
			this->set(conn);
		}

		SocketConnection* getConnection(){return this->get();}
	};

	typedef ESignal<BasicNode> NodeSignal;
	typedef ESignalPerformer<BasicNode> NodeSignalPerformer;
	typedef ESignalEmitter<BasicNode> NodeSignalEmitter;

	class GraphScene : public BasicGraph, public ESignalEmitter<GraphScene>
	{
	protected:
		std::vector<SocketConnection*> _connections;
	public:
		GraphScene();
		bool connect(SocketConnection*);
		bool disconnect(SocketConnection*);
		bool connect(OutputSocket*, InputSocket*);
		bool disconnect(OutputSocket*, InputSocket*);
		SocketConnection* getSocketConnection(OutputSocket*, InputSocket*);
		SocketConnection* getSocketConnection(InputSocket*);
		bool isConnected(OutputSocket*, InputSocket*);
		std::vector<SocketConnection*>& getSocketConnections(){return _connections;}
		
		virtual void removeNode(BasicNode* node) override;
		void removeSocketCorrespondingConnections(SocketNode* node);
		void addGraphSceneSignalPerformer(ESignalPerformer<GraphScene>* performer)
		{
			ESignalEmitter<GraphScene>::addSignalPerformer(performer);
		}
	};

}