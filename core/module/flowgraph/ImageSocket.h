/**
* Copyright(C) 2009-2012                
* @author Jing HUANG
* @file ImageSocket.h
* @brief 
* @date 1/2/2011
*/

#pragma once
#include "TemplateSocket.h"
#include "module/signals/ImageSignal.h"

namespace Etoile
{

	class ImageInputSocket : public TemplateInputSocket<Image>
	{
	public:
		ImageInputSocket(const std::string& name)
		{
			initType("SOCK_IMAGE");
			this->setName(name);
		}
	};


	class ImageOutputSocket : public TemplateOutputSocket<Image> 
	{
	public:
		ImageOutputSocket(const std::string& name)
		{
			initType("SOCK_IMAGE");
			this->setName(name);
		}
	};
}