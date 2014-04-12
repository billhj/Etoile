/**
* Copyright(C) 2009-2012                
* @author Jing HUANG
* @file GeometryShader.h
* @brief 
* @date 1/2/2011
*/

#pragma once
#include "ShaderObject.h"

namespace Etoile
{
	class GeometryShader: public ShaderObject
	{
	public:
		GeometryShader();
		GeometryShader(const std::string& shaderFilename);
		virtual std::string getShaderTypeToString() override {return "SHADERTYPE_GEOMETRY";}
	};

}
