/**
* Copyright(C) 2009-2012                
* @author Jing HUANG
* @file TextureSocket.h
* @brief 
* @date 1/2/2011
*/

#pragma once
#include "TemplateSocket.h"
#include "module/signals/TextureSignal.h"

namespace Etoile
{
	class TextureInputSocket : public TemplateInputSocket<Texture>
	{
	public:
		TextureInputSocket(const std::string& name)
		{
			initType("SOCK_TEXTURE");
			this->setName(name);
		}
	};


	class TextureOutputSocket : public TemplateOutputSocket<Texture>
	{
	public:
		TextureOutputSocket(const std::string& name)
		{
			initType("SOCK_TEXTURE");
			this->setName(name);
		}
	};
}