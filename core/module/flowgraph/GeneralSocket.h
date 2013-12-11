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
#include "math/Matrix4.h"

namespace Etoile
{

	class StringInputSocket : public TemplateInputSocket<std::string>
	{
	public:
		StringInputSocket(const std::string& name)
		{
			initType("SOCK_STRING");
			this->setName(name);
		}
	};


	class StringOutputSocket : public TemplateOutputSocket<std::string>
	{
	public:
		StringOutputSocket(const std::string& name)
		{
			initType("SOCK_STRING");
			this->setName(name);
		}
	};

	class IntInputSocket : public TemplateInputSocket<int>
	{
	public:
		IntInputSocket(const std::string& name)
		{
			initType("SOCK_INT");
			this->setName(name);
		}
	};


	class IntOutputSocket : public TemplateOutputSocket<int>
	{
	public:
		IntOutputSocket(const std::string& name)
		{
			initType("SOCK_INT");
			this->setName(name);
		}
	};


	class FloatInputSocket : public TemplateInputSocket<float>
	{
	public:
		FloatInputSocket(const std::string& name)
		{
			initType("SOCK_FLOAT");
			this->setName(name);
		}
	};


	class FloatOutputSocket : public TemplateOutputSocket<float>
	{
	public:
		FloatOutputSocket(const std::string& name)
		{
			initType("SOCK_FLOAT");
			this->setName(name);
		}
	};


	class Matrix4fInputSocket : public TemplateInputSocket<Matrix4f>
	{
	public:
		Matrix4fInputSocket(const std::string& name)
		{
			initType("SOCK_MATRIX4F");
			this->setName(name);
		}
	};


	class Matrix4fOutputSocket : public TemplateOutputSocket<Matrix4f>
	{
	public:
		Matrix4fOutputSocket(const std::string& name)
		{
			initType("SOCK_MATRIX4F");
			this->setName(name);
		}
	};
}