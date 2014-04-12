/**
* Copyright(C) 2009-2012                
* @author Jing HUANG
* @file SocketConnection.h
* @brief 
* @date 1/2/2011
*/

#pragma once
#include "SocketNode.h"

namespace Etoile
{

	class SocketConnection 
	{
	private:
		/**
		* @brief Startpoint of the connection
		*/
		OutputSocket *_pfromSocket;

		/**
		* @brief Endpoint of the connection
		*/
		InputSocket *_ptoSocket;

	public:
		SocketConnection();

		/**
		* @brief set the startpoint of the connection
		* @param fromsocket
		*/
		void setFromSocket(OutputSocket *fromsocket);

		/**
		* @brief get the startpoint of the connection
		* @return from OutputSocket
		*/
		OutputSocket *getFromSocket() const;

		/**
		* @brief set the endpoint of the connection
		* @param tosocket
		*/
		void setToSocket(InputSocket *tosocket);

		/**
		* @brief get the endpoint of the connection
		* @return to InputSocket
		*/
		InputSocket *getToSocket() const;

		/**
		* @brief check if this connection is valid
		*/
		bool isValid() const;

		/**
		* @brief return the Node where this connection is connected from
		*/
		SocketNode *getFromNode() const;

		/**
		* @brief return the Node where this connection is connected to
		*/
		SocketNode *getToNode() const;

	};

	typedef std::vector<SocketConnection*> SocketConnections;
}