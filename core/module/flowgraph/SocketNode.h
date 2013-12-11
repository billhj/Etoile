/**
* Copyright(C) 2009-2012                
* @author Jing HUANG
* @file SocketNode.h
* @brief 
* @date 1/2/2011
*/

#pragma once
#include "BasicGraph.h"
#include "Socket.h"

namespace Etoile
{
	class SocketNode : public BasicNode
	{
	public:
		SocketNode():BasicNode(){}
		virtual ~SocketNode(){clear();}
		void addInputSocket(InputSocket*);
		void addOutputSocket(OutputSocket*);
		InputSockets& getInputSockets(){return _inputs;}
		OutputSockets& getOutputSockets(){return _outputs;}
		void setInputs(InputSockets sockets)
		{
			this->_inputs = sockets;
		}

		void setOutputs(OutputSockets sockets)
		{
			this->_outputs = sockets;
		}
		int getInputSocketIndex(InputSocket*);
		int getOutputSocketIndex(OutputSocket*);
		void clear();
	protected:
		InputSockets _inputs;
		OutputSockets _outputs;
	};

}