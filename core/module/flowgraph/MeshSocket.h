/**
* Copyright(C) 2009-2012                
* @author Jing HUANG
* @file MeshSocket.h
* @brief 
* @date 1/2/2011
*/

#pragma once
#include "TemplateSocket.h"
#include "module/signals/MeshSignal.h"

namespace Etoile
{
	class MeshInputSocket : public TemplateInputSocket<Mesh>
	{
	public:
		MeshInputSocket(const std::string& name): TemplateInputSocket<Mesh>(INPUT_MUL)
		{
			initType("SOCK_MESH");
			this->setName(name);
		}
	};


	class MeshOutputSocket : public TemplateOutputSocket<Mesh>
	{
	public:
		MeshOutputSocket(const std::string& name)
		{
			initType("SOCK_MESH");
			this->setName(name);
		}
	};

}