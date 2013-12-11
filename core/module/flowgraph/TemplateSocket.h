/**
* Copyright(C) 2009-2012                
* @author Jing HUANG
* @file TemplateSocket.h
* @brief 
* @date 1/2/2011
*/

#pragma once
#include <string>
#include <vector>
#include "module/flowgraph/Socket.h"
#include "module/signals/ESignal.h"
#include "GraphScene.h"

namespace Etoile
{
	template<class T>
	class TemplateInputSocket : public InputSocket, public ESignalPerformer<T>
	{
	public:
		TemplateInputSocket(UsageType usageType = INPUT_UNI) : InputSocket(usageType){}
		//needs to rewrite override
		virtual void perform(T* signal) = 0;
		virtual void retrieve(T* signal) = 0;
	};

	template<class T>
	class TemplateOutputSocket : public OutputSocket, public ESignalEmitter<T>, public ESignal<T>
	{
	public:
		virtual void perform(SocketConnectedSignal* signal) override
		{
			SocketConnection * conn = signal->get();
			if(conn == NULL) return;
			TemplateOutputSocket<T> *from = dynamic_cast<TemplateOutputSocket<T>*>(conn->getFromSocket());
			TemplateInputSocket<T> *to = dynamic_cast<TemplateInputSocket<T>*>(conn->getToSocket());
			if(from == NULL || to == NULL ) return;
			if(from == this)
			{
				this->addSignalPerformer(to);
				to->perform(_pt);
			}
		}

		virtual void perform(SocketDisconnectedSignal* signal) override
		{
			SocketConnection * conn = signal->get();
			if(conn == NULL) return;
			TemplateOutputSocket<T> *from = dynamic_cast<TemplateOutputSocket<T>*>(conn->getFromSocket());
			TemplateInputSocket<T> *to = dynamic_cast<TemplateInputSocket<T>*>(conn->getToSocket());
			if(from == NULL || to == NULL ) return;
			if(from == this)
			{
				this->removeSignalPerformer(to);
				to->retrieve(from->get());
			}
		}
	};
}