/**
* Copyright(C) 2009-2012                
* @author Jing HUANG
* @file Socket.cpp
* @brief 
* @date 1/2/2011
*/

#include "Socket.h"
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

	bool Socket::verify(Socket *socket)
	{
		if(socket->_type._type.compare(_type._type)==0)
		{
			return true;
		}
		return false;
	}

	void Socket::setNode(SocketNode* node)
	{
		_pnode = node;
	}

	void Socket::initType(std::string type)
	{
		if(type.compare("SOCK_BOOL")==0){
			_type._type = "SOCK_BOOL";
			_type._description = "Socket_bool";
			_type._icon = 0;
			_type._color = SocketType::color(158, 139, 63, 255);//= new int[158, 139, 63, 255];
		}
		else if(type.compare("SOCK_INT")==0){
			_type._type = "SOCK_INT";
			_type._description = "Socket_int";
			_type._icon = 0;
			_type._color = SocketType::color(17, 133, 37, 255);
		}
		else if(type.compare("SOCK_FLOAT")==0){
			_type._type = "SOCK_FLOAT";
			_type._description = "Socket_float";
			_type._icon = 0;
			_type._color = SocketType::color(160, 160, 160, 255);
		}
		else if(type.compare("SOCK_STRING")==0){
			_type._type = "SOCK_STRING";
			_type._description = "Socket_string";
			_type._icon = 0;
			_type._color = SocketType::color(120, 220, 120, 255);
		}
		else if(type.compare("SOCK_RGBA")==0){
			_type._type = "SOCK_RGBA";
			_type._description = "Socket_rgba";
			_type._icon = 0;
			_type._color = SocketType::color(200, 200, 40, 255);
		}
		else if(type.compare("SOCK_SHADER")==0){
			_type._type = "SOCK_SHADER";
			_type._description = "shader";
			_type._icon = 0;
			_type._color = SocketType::color(100, 200, 100, 255);
		}
		else if(type.compare("SOCK_MESH")==0){
			_type._type = "SOCK_MESH";
			_type._description = "Mesh geometry data";
			_type._icon = 0;
			_type._color = SocketType::color(255, 133, 7, 255);
		}
		else if(type.compare("SOCK_SCENEOBJECT")==0){
			_type._type = "SOCK_SCENEOBJECT";
			_type._description = "Scene object data";
			_type._icon = 0;
			_type._color = SocketType::color(255, 133, 7, 255);
		}
		else if(type.compare("SOCK_PLANE")==0){
			_type._type = "SOCK_PLANE";
			_type._description = "Scene plane data";
			_type._icon = 0;
			_type._color = SocketType::color(255, 133, 7, 255);
		}
		else if(type.compare("SOCK_MESH_RENDER_UNIT")==0){
			_type._type = "SOCK_MESH_RENDER_UNIT";
			_type._description = "Mesh geometry data";
			_type._icon = 0;
			_type._color = SocketType::color(255, 140, 10, 255);
		}
		else if(type.compare("SOCK_IMAGE")==0){
			_type._type = "SOCK_IMAGE";
			_type._description = "Image data";
			_type._icon = 0;
			_type._color = SocketType::color(200, 133, 7, 255);
		}else if(type.compare("SOCK_TEXTURE")==0){
			_type._type = "SOCK_TEXTURE";
			_type._description = "Texture data";
			_type._icon = 0;
			_type._color = SocketType::color(200, 166, 7, 255);
		}else if(type.compare("SOCK_MATRIX4F")==0){
			_type._type = "SOCK_MATRIX4F";
			_type._description = "Matrix data";
			_type._icon = 0;
			_type._color = SocketType::color(180, 100, 7, 255);
		}else{
			_type._type = type;
			_type._description = type;
			_type._icon = 0;
			_type._color = SocketType::color(100, 100, 100, 255);
		}
	}

	void Socket::setSocketConnection(SocketConnection *connection)
	{
		_pConnection = connection;
	}

}