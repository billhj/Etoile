/**
* Copyright(C) 2009-2012                
* @author Jing HUANG
* @file SocketNode.cpp
* @brief 
* @date 1/2/2011
*/

#include "SocketNode.h"

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
	void SocketNode::addInputSocket(InputSocket *input)
	{
		_inputs.push_back(input);
		input->setNode(this);
	}

	void SocketNode::addOutputSocket(OutputSocket *output)
	{
		_outputs.push_back(output);
		output->setNode(this);
	}

	int SocketNode::getInputSocketIndex(InputSocket* input)
	{
		for(unsigned int i = 0; i < _inputs.size();++i)
		{
			InputSocket* socket = _inputs[i];
			if(socket == input)
			{ 
				return i;
			}
		}
		return -1;
	}

	int SocketNode::getOutputSocketIndex(OutputSocket* output)
	{
		for(unsigned int i = 0; i < _outputs.size(); ++i)
		{
			OutputSocket *socket = _outputs[i];
			if(socket == output)
			{
				return i;
			}
		}
		return -1;
	}

	void SocketNode::clear()
	{
		for(unsigned int i = 0; i < _inputs.size();++i)
		{
			InputSocket* socket = _inputs[i];
			if(socket != NULL)
			{ 
				delete socket;
			}
		}
		_inputs.clear();

		for(unsigned int i = 0; i < _outputs.size(); ++i)
		{
			OutputSocket *socket = _outputs[i];
			if(socket != NULL)
			{
				delete socket;
			}
		}
		_outputs.clear();
	}
}