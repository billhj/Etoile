/**
* Copyright(C) 2009-2012                
* @author Jing HUANG
* @file Socket.h
* @brief 
* @date 1/2/2011
*/

#pragma once
#include <string>
#include <vector>
#include "module/signals/ESignal.h"

namespace Etoile
{
	/*enum SocketTYPE
	{
		SOCK_BOOL,
		SOCK_INT,
		SOCK_FLOAT,
		SOCK_STRING,
		SOCK_RGBA,
		SOCK_SHADER,
		SOCK_MESH,
		SOCK_IMAGE,
		SOCK_TEXTURE,
	};*/

	enum UsageType
	{
		INPUT_UNI,
		INPUT_MUL,
		OUTPUT,
		NODEFINED,
	};

	struct SocketType
	{
		SocketType()
		{
			_xPos = 0;
			_yPos = 0;
			_w = 20, _h = 10;
		}

		std::string _type;
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
		float _xPos;
		float _yPos;
		float _w, _h;
	};

	class SocketNode;
	class SocketConnection;
	class SocketConnectedSignal;
	class SocketDisconnectedSignal;

	class Socket
	{
	public:
		void initType(std::string type); 
		void setNode(SocketNode*);
		SocketNode* getNode(){return _pnode;}
		bool verify(Socket*);
		SocketType& getType(){return _type;}
		void setSocketConnection(SocketConnection *connection);
		SocketConnection* getSocketConnection(){return _pConnection;}
		virtual UsageType getUsageType() = 0;
		const std::string getName(){return this->_type._name;}
		void setName(const std::string& name){_type._name = name;}
	protected:
		SocketType _type;
		SocketNode* _pnode;
		SocketConnection *_pConnection;
	};

	

	class InputSocket : public Socket
	{
		UsageType _usageType;
	public:
		InputSocket(UsageType usageType = INPUT_UNI): _usageType(usageType)
		{
		}

		virtual UsageType getUsageType()
		{
			return _usageType;
		}
	};

	class OutputSocket : public Socket, public ESignalPerformer<SocketConnectedSignal>, public ESignalPerformer<SocketDisconnectedSignal>
	{
	public:
		virtual UsageType getUsageType()
		{
			return OUTPUT;
		}
		//needs to rewrite override
		virtual void perform(SocketConnectedSignal* signal){}
		virtual void perform(SocketDisconnectedSignal* signal){}
	};

	typedef std::vector<InputSocket*> InputSockets;
	typedef std::vector<OutputSocket*> OutputSockets;
}