/**
* Copyright(C) 2009-2012                
* @author Jing HUANG
* @file TessCtrlShader.h
* @brief 
* @date 1/2/2011
*/

#pragma once
#include "ShaderObject.h"

namespace Etoile
{
	class TessCtrlShader: public ShaderObject
	{
	public:
		TessCtrlShader();
		TessCtrlShader(const std::string& shaderFilename);
		virtual std::string getShaderTypeToString() override {return "SHADERTYPE_TESSELLATION_CTRL";}
	};

}
