/**
* Copyright(C) 2009-2012                
* @author Jing HUANG
* @file GLBaseBufferObject.h
* @brief a base class of buffer object use glGenBuffer, glBindBuffer
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
		// bindbufer
		void use();
		// unbindbufer
		void unUse();
		//setBindingBufferTarget
		void setTarget(GLenum target);
		//setWayofBuffer dynamic or static
		void setUsage(GLenum usage);
		//buffer id/idx
		GLuint id();
		GLenum usage();
		GLenum target();
		//bind data into currentbuffer
		void bindData(GLsizei datasize, DataType* data);
		//bind subdata into currentbuffer
		void bindSubData(GLint offset, GLsizei datasize, DataType* data);

		//GL_READ_ONLY, GL_WRITE_ONLY, or GL_READ_WRITE
		DataType* useMap(GLenum accessType);
		DataType* useMapForRead();
		DataType* useMapForWrite();
		DataType* useMapForReadAndWrite();
		void unUseMap();

	protected:
		GLenum _target;
		GLenum _usage;
		GLuint _id;
	};
}
#endif //GLBASE_BUFFER_OBJECT_H
