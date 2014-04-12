/**
* Copyright(C) 2009-2012                
* @author Jing HUANG
* @file ShaderObject.h
* @brief 
* @date 1/2/2011
*/

#pragma once
#include "glhead.h"
#include <string>

namespace Etoile
{

	enum ShaderType
	{
		SHADERTYPE_VERTEX = 0,				///< Vertex Shader
		SHADERTYPE_TESSELLATION_CTRL,		///< Tessellation Control Shader
		SHADERTYPE_TESSELLATION_EVAL,		///< Tessellation Evaluation Shader
		SHADERTYPE_GEOMETRY,				///< Geometry Shader
		SHADERTYPE_FRAGMENT,				///< Fragment Shader
		SHADERTYPE_COMPUTE,					///< Compute Shader
		SHADERTYPE_INVALID					///< Invalid Shader type
	};

	class ShaderObject 
	{
	public:
		ShaderObject(ShaderType shaderType);
		ShaderObject(ShaderType shaderType, const std::string& shaderFilename);
		virtual ~ShaderObject ();
		void releaseResource();
		void loadShader(const std::string& shaderFilename);
		void loadShader();
		void reload();
		void loadShaderByString(const std::string& shader);
		void compileAttach(GLuint & shader, GLenum type, const std::string& source);
		unsigned int getShaderSize (const std::string& filename);
		char* readShaderSource(const std::string& shaderFilename, unsigned int & shaderSize);
		static void printShaderInfoLog(GLuint shader);
		GLenum getOpenGLShaderType(ShaderType shaderType);
		GLuint getId(){return _id;}
		ShaderType getShaderType(){return _shaderType;}
		virtual std::string getShaderTypeToString() = 0;
		virtual void checkShaderObject()
		{
			std::cout<<"Shader: "<<getShaderTypeToString()<<std::endl;
		}
	protected:
		GLuint _id;
		unsigned int _shaderSize;
		ShaderType _shaderType;
		std::string _filename;
	};

}
