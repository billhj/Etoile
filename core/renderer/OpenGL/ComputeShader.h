/**
* Copyright(C) 2009-2012                
* @author Jing HUANG
* @file ComputeShader.h
* @brief 
* @date 1/2/2011
*/

#pragma once
#include "ShaderObject.h"

namespace Etoile
{

	class ComputeShader : public ShaderObject
	{
	private:
		GLuint _x_Compute, _y_Compute, _z_Compute;
	public:
		ComputeShader();
		ComputeShader(const std::string& shaderFilename);
		void setSize(unsigned int, unsigned int, unsigned int);
		unsigned int getX(){return _x_Compute;}
		unsigned int getY(){return _y_Compute;}
		unsigned int getZ(){return _z_Compute;}
		virtual std::string getShaderTypeToString() override {return "SHADERTYPE_COMPUTE";}
		virtual void checkShaderObject() override;
	};

}
