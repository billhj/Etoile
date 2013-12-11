/**
* Copyright(C) 2009-2012                
* @author Jing HUANG
* @file VertexShader.h
* @brief 
* @date 1/2/2011
*/

#pragma once
#include "ShaderObject.h"

namespace Etoile
{
	class VertexShader: public ShaderObject
	{
	public:
		VertexShader();
		VertexShader(const std::string& shaderFilename);
		virtual std::string getShaderTypeToString() override {return "SHADERTYPE_VERTEX";}
	};

}
