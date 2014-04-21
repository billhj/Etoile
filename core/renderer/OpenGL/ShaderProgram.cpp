/**
* Copyright(C) 2009-2012                
* @author Jing HUANG
* @file ShaderProgram.cpp
* @brief 
* @date 1/2/2011
*/

#include "ShaderProgram.h"
#include <assert.h>
#include <iostream>
#include <sstream>
#include <algorithm>
#include "ComputeShader.h"
/**
* @brief For tracking memory leaks under windows using the crtdbg
*/
#if ( defined( _DEBUG ) || defined( DEBUG ) ) && defined( _MSC_VER )
#define _CRTDBG_MAP_ALLOC
#include <crtdbg.h>

#define DEBUG_NEW new( _NORMAL_BLOCK, __FILE__, __LINE__ )
#define new DEBUG_NEW
#endif


#if defined( _MSC_VER )
#pragma warning( disable : 4996 )
#endif

namespace Etoile
{
	ShaderProgram::ShaderProgram(): _id(0)
	{
		_shaders.resize(6);
		_shaders[SHADERTYPE_VERTEX] = NULL;
		_shaders[SHADERTYPE_TESSELLATION_CTRL] = NULL;
		_shaders[SHADERTYPE_TESSELLATION_EVAL] = NULL;
		_shaders[SHADERTYPE_GEOMETRY] = NULL;
		_shaders[SHADERTYPE_FRAGMENT] = NULL;
		_shaders[SHADERTYPE_COMPUTE] = NULL;
	}

	ShaderProgram::~ShaderProgram()
	{
		releaseResource();
	}

	void ShaderProgram::releaseResource()
	{
		_shaders.clear();
		printOpenGLError();
		if(_id != 0)
		{
			glDeleteProgram (_id);
		}
		printOpenGLError();
		_shaders.resize(6);
	}

	void ShaderProgram::addShader(ShaderObject* shader)
	{
		if(shader->getShaderType() == SHADERTYPE_VERTEX)
		{
			_shaders[SHADERTYPE_VERTEX] = shader;
		}
		else if(shader->getShaderType() == SHADERTYPE_TESSELLATION_CTRL)
		{
			_shaders[SHADERTYPE_TESSELLATION_CTRL] = shader;
		}
		else if(shader->getShaderType() == SHADERTYPE_TESSELLATION_EVAL)
		{
			_shaders[SHADERTYPE_TESSELLATION_EVAL] = shader;
		}
		else if(shader->getShaderType() == SHADERTYPE_GEOMETRY)
		{
			_shaders[SHADERTYPE_GEOMETRY] = shader;
		}
		else if(shader->getShaderType() == SHADERTYPE_FRAGMENT)
		{
			_shaders[SHADERTYPE_FRAGMENT] = shader;
		}
		else if(shader->getShaderType() == SHADERTYPE_COMPUTE)
		{
			_shaders[SHADERTYPE_COMPUTE] = shader;
		}
		else if(shader->getShaderType() == SHADERTYPE_INVALID)
		{
			assert(0 && "ShaderProgram class: invalid shader object");
		}
	}

	void ShaderProgram::link()
	{
		printOpenGLError ();
		if(_id != 0)
		{
			glDeleteProgram (_id);
		}
		_id = glCreateProgram();

		for(unsigned int i = 0 ; i < _shaders.size(); ++i)
		{
			ShaderObject* shader = _shaders[i];
			if(shader != NULL)
			{
				shader->reload();
				glAttachShader(_id, shader->getId());
				printOpenGLError();  // Check for OpenGL errors
			}
		}

		printOpenGLError();  // Check for OpenGL errors
		glLinkProgram (_id);
		printOpenGLError();
		GLint linked;
		glGetProgramiv (_id, GL_LINK_STATUS, &linked);
		printOpenGLError ();
		printProgramInfoLog(_id);

		std::cout<<"Check Shader Objects:"<<std::endl;
		for(unsigned int i = 0 ; i < _shaders.size(); ++i)
		{
			ShaderObject* shader = _shaders[i];
			if(shader != NULL)
			{
				shader->checkShaderObject();
			}
		}
	}

	void ShaderProgram::reload()
	{
		link();
	}

	void ShaderProgram::bind () {
		printOpenGLError();
		glUseProgram (_id);
		printOpenGLError();
	}

	void ShaderProgram::unbind () {
		printOpenGLError();
		glUseProgram (0);
		printOpenGLError();
	}


	void ShaderProgram::printProgramInfoLog(GLuint program) {
		int infologLength = 0;
		int charsWritten  = 0;
		GLchar *infoLog;

		glGetProgramiv (program, GL_INFO_LOG_LENGTH, &infologLength);
		printOpenGLError();  // Check for OpenGL errors

		if (infologLength > 0) {
			infoLog = new GLchar[infologLength];
			if (infoLog == NULL) {
				printf("ERROR: Could not allocate InfoLog buffer\n");
				exit(1);
			}
			glGetProgramInfoLog (program, infologLength, &charsWritten, infoLog);
			std::cerr<<"ShaderProgram InfoLog:["<<program<<"]"<<std:: endl << infoLog <<std:: endl << std::endl;
			delete [] infoLog;
		}
		printOpenGLError();  // Check for OpenGL errors
	}


	void ShaderProgram::setGeometryShaderInOutputType(GLint input, GLint output)
	{
		printOpenGLError();  // Check for OpenGL errors
		glProgramParameteriEXT(_id, GL_GEOMETRY_INPUT_TYPE_EXT, input);
		printOpenGLError();  // Check for OpenGL errors
		glProgramParameteriEXT(_id, GL_GEOMETRY_OUTPUT_TYPE_EXT, output);
		printOpenGLError();  // Check for OpenGL errors
		/*int temp;
		glGetIntegerv(GL_MAX_GEOMETRY_OUTPUT_VERTICES_EXT,&temp);
		printOpenGLError ();
		glProgramParameteriEXT( shaderProgram,GL_GEOMETRY_VERTICES_OUT_EXT,temp);*/
		printOpenGLError();
		glLinkProgram (_id);
		printOpenGLError();
	}


	/**uniform buffer object
	*/
	GLuint ShaderProgram::getUniformBlockIndex(const std::string& blockName)
	{
		return glGetUniformBlockIndex(_id, blockName.c_str());
	}

	GLint ShaderProgram::getUniformBlockSize(GLuint blockindex)
	{
		GLint iUniformBlockSize;
		glGetActiveUniformBlockiv(_id, blockindex, GL_UNIFORM_BLOCK_DATA_SIZE,&iUniformBlockSize);
		printOpenGLError();  // Check for OpenGL errors
		return iUniformBlockSize;
	}

	GLint*  ShaderProgram::getUniformBlock(GLuint blockindex)
	{
		GLint*  params;
		glGetActiveUniformBlockiv(_id, blockindex, GL_UNIFORM_BLOCK_BINDING, params);
		printOpenGLError();  // Check for OpenGL errors
		return params;
	}

	void ShaderProgram::uniformBlockBinding(GLuint uniformBlockIndex, GLuint uniformBlockBinding)
	{
		glUniformBlockBinding(_id, uniformBlockIndex, uniformBlockBinding);
		printOpenGLError();  // Check for OpenGL errors
	}

	/**transformFeedback
	*/
	void ShaderProgram::transformFeedbackVaryingsSetting(size_t count, const char **varyings, GLenum bufferMode)
	{
		glTransformFeedbackVaryings(_id, count, varyings, bufferMode);
		glLinkProgram (_id);
	}
	/**subroutine
	*/
	GLint ShaderProgram::getSubroutineUniLoc (GLuint program, GLenum shadertype, const GLchar *name)
	{
		printOpenGLError();
		GLint loc = glGetSubroutineUniformLocation (program, shadertype, name);
		printOpenGLError();
		return loc;
	}

	GLint ShaderProgram::getVertexShaderSubroutineUniLoc (GLuint program, const GLchar *name)
	{
		return getSubroutineUniLoc(program, GL_VERTEX_SHADER, name);
	}

	GLint ShaderProgram::getFragmentShaderSubroutineUniLoc (GLuint program, const GLchar *name)
	{
		return getSubroutineUniLoc(program, GL_FRAGMENT_SHADER, name);
	}

	GLuint ShaderProgram::getSubroutineIndex(GLuint program, GLenum shadertype, const char *name)
	{
		printOpenGLError();
		GLuint index = glGetSubroutineIndex(program, shadertype, name);
		printOpenGLError();
		return index;
	}


	/**uniform
	*/
	GLint ShaderProgram::getUniLoc (GLuint program, const GLchar *name)
	{
		printOpenGLError();
		GLint loc = glGetUniformLocation (program, name);
		printOpenGLError();
		return loc;
	}
	/**attribut
	*/
	GLint ShaderProgram::getAttributLoc(GLuint program, const GLchar *name)
	{
		GLint loc = glGetAttribLocation(program, name);
		printOpenGLError();
		return loc;
	}

}