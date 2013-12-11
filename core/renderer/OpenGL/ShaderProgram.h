/**
* Copyright(C) 2009-2012                
* @author Jing HUANG
* @file ShaderProgram.h
* @brief 
* @date 1/2/2011
*/

#pragma once
#include <map>
#include <vector>
#include "ShaderObject.h"

namespace Etoile
{
	class ShaderProgram
	{
	public:
		ShaderProgram();
		~ShaderProgram();
		void releaseResource();
		void addShader(ShaderObject* shader);

		ShaderObject* getShader(ShaderType type){return _shaders[type];}
		/**
		link all shader to program
		*/
		void link();
		void reload();
		void bind ();
		void unbind ();

		GLuint getId(){return _id;}
		static void printProgramInfoLog(GLuint program);


		inline GLint getUniformLocation(const GLchar*name){return getUniLoc(name); }
		inline GLint getAttributLocation(const GLchar*name){return getAttributLoc(name); }
		void setGeometryShaderInOutputType(GLint input, GLint output);

		/**uniform buffer object
		*/
		GLuint getUniformBlockIndex(const std::string& blockName);
		GLint getUniformBlockSize(GLuint blockindex);
		GLint getUniformBlockBinding(GLuint blockindex);
		void uniformBlockBinding(GLuint uniformBlockIndex, GLuint uniformBlockBinding);

		/**transformFeedback
		*/
		void transformFeedbackVaryingsSetting(size_t count, const char **varyings, GLenum bufferMode);

		/**subroutine
		*/
		//subroutine
		inline GLint getFragmentShaderSubroutineUniformLocation(const GLchar*name){return getFragmentShaderSubroutineUniLoc(_id, name); }
		inline GLint getVertexShaderSubroutineUniformLocation(const GLchar*name){return getVertexShaderSubroutineUniLoc(_id, name); }
		inline GLint getSubroutineUniformLocation( GLenum shadertype, const GLchar*name){return getSubroutineUniLoc(_id, shadertype, name); }

		GLuint getSubroutineIndex(GLenum shadertype, const char *name){return getSubroutineIndex(_id, shadertype, name);}
		GLuint getVertexShaderSubroutineIndex(const char *name){return getSubroutineIndex(_id, GL_VERTEX_SHADER, name);}
		GLuint getFragmentShaderSubroutineIndex(const char *name){return getSubroutineIndex(_id, GL_FRAGMENT_SHADER, name);}

		GLint getSubroutineUniLoc (GLuint program, GLenum shadertype, const GLchar *name);
		GLint getVertexShaderSubroutineUniLoc (GLuint program, const GLchar *name);
		GLint getFragmentShaderSubroutineUniLoc (GLuint program, const GLchar *name);
		GLuint getSubroutineIndex(GLuint program, GLenum shadertype, const char *name);


		/**
		*	COMPUTE SHADER
		*/
		
	private:
		GLint getUniLoc(GLuint program, const GLchar *name);
		inline GLint getUniLoc(const GLchar*name) { return getUniLoc(_id, name); }
		GLint getAttributLoc (GLuint program, const GLchar *name);
		inline GLint getAttributLoc( const GLchar*name) { return getAttributLoc(_id, name); }

		GLuint _id;
		std::vector<ShaderObject*> _shaders;
	}; 


}