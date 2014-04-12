/**
* Copyright(C) 2009-2012                
* @author Jing HUANG
* @file RgbaSocket.h
* @brief 
* @date 1/2/2011
*/

#pragma once
#include "TemplateSocket.h"
#include "module/signals/RgbaSignal.h"

namespace Etoile
{

	class RgbaInputSocket : public TemplateInputSocket<Rgbaf>
	{
	public:
		RgbaInputSocket(const std::string& name)
		{
			initType("SOCK_RGBA");
			this->setName(name);
		}
	};


	class RgbaOutputSocket : public TemplateOutputSocket<Rgbaf>
	{
	public:
		RgbaOutputSocket(const std::string& name)
		{
			initType("SOCK_RGBA");
			this->setName(name);
		}
	};
}