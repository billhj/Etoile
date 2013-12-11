/**
* Copyright(C) 2009-2012                
* @author Jing HUANG
* @file SocketConnection.cpp
* @brief 
* @date 1/2/2011
*/

#include "SocketConnection.h"

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

	SocketConnection::SocketConnection()
	{
		this->_pfromSocket = NULL;
		this->_ptoSocket = NULL;
	}

	void SocketConnection::setFromSocket(OutputSocket *fromsocket)
	{
		if (fromsocket == NULL) {
			throw "ERROR";
		}
		this->_pfromSocket = fromsocket;
		_pfromSocket->setSocketConnection(this);
	}

	OutputSocket *SocketConnection::getFromSocket() const { return this->_pfromSocket; }
	void SocketConnection::setToSocket(InputSocket *tosocket)
	{
		if (tosocket == NULL) {
			throw "ERROR";
		}
		this->_ptoSocket = tosocket;
		_ptoSocket->setSocketConnection(this);
	}

	InputSocket *SocketConnection::getToSocket() const { return this->_ptoSocket; }

	SocketNode *SocketConnection::getFromNode() const
	{
		if (this->getFromSocket() == NULL) {
			return NULL;
		}
		else {
			return this->getFromSocket()->getNode();
		}
	}

	SocketNode *SocketConnection::getToNode() const
	{
		if (this->getToSocket() == NULL) {
			return NULL;
		}
		else {
			return this->getToSocket()->getNode();
		}
	}
	bool SocketConnection::isValid() const
	{
		if ((this->getToSocket() != NULL && this->getFromSocket() != NULL)) {
			//if (this->getFromNode()->isOperation() && this->getToNode()->isOperation()) {
				return true;
			//}
		}
		return false;
	}
}