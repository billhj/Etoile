/**
* Copyright(C) 2009-2012                
* @author Jing HUANG
* @file GLBaseBufferObject.h
* @brief 
* @date 1/2/2011
*/
#pragma once
#ifndef GLBASE_BUFFER_OBJECT_H
#define GLBASE_BUFFER_OBJECT_H
#include "glhead.h"

namespace Etoile
{
	template <class DataType>
	class GLBaseBufferObject
	{
	public:
		GLBaseBufferObject();
		void use();
		void unUse();
		void setTarget(GLenum target);
		void setUsage(GLenum usage);
		GLuint id();
		GLenum usage();
		GLenum target();
		void bindData(GLsizei datasize, DataType* data);
		void bindSubData(GLint offset, GLsizei datasize, DataType* data);
	protected:
		GLenum _target;
		GLenum _usage;
		GLuint _id;
	};
}
#endif //GLBASE_BUFFER_OBJECT_H
