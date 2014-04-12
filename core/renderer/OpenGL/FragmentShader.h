/**
* Copyright(C) 2009-2012                
* @author Jing HUANG
* @file FragmentShader.h
* @brief 
* @date 1/2/2011
*/

#pragma once
#include "ShaderObject.h"

namespace Etoile
{
	class FragmentShader: public ShaderObject
	{
	public:
		FragmentShader();
		FragmentShader(const std::string& shaderFilename);
		virtual std::string getShaderTypeToString() override {return "SHADERTYPE_FRAGMENT";}
	};

}
