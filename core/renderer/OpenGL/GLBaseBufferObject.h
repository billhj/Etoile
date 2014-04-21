/**
* Copyright(C) 2009-2012                
* @author Jing HUANG
* @file GLBaseBufferObject.h
* @brief a base class of buffer object use glGenBuffer, glBindBuffer
* as my understanding, no much difference than glBufferDataARB,
* only map can directly loadData from file to DMA memory by give its memory address
* with glBufferDataARB, you first read file, put into memory, then copy into DMA using BufferDataARB
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
		virtual~GLBaseBufferObject();
		// bindbufer
		void use();
		// unbindbufer
		void unUse();
		//setBindingBufferTarget
		void setTarget(GLenum target);
		//setWayofBuffer dynamic or static
		// GL_STREAM_DRAW, GL_STREAM_READ, GL_STREAM_COPY, GL_STATIC_DRAW, GL_STATIC_READ, GL_STATIC_COPY, GL_DYNAMIC_DRAW, GL_DYNAMIC_READ, or GL_DYNAMIC_COPY
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
		//return the mapped DMA memory address, 
		//as my understanding, no much difference than glBufferDataARB,
		//only map can directly loadData from file to DMA memory by give its memory address
		//with glBufferDataARB, you first read file, put into memory, then copy into DMA using BufferDataARB
		DataType* useMap(GLenum accessType);
		DataType* useMapForRead();
		DataType* useMapForWrite();
		DataType* useMapForReadAndWrite();
		//partially data map transfer
		DataType* useMapRange(GLint offset, GLsizei datasize);
		void unUseMap();

		GLsizei dataSize();
	protected:
		GLenum _target;
		GLenum _usage;
		GLuint _id;
		GLsizei _size;
	};


	//void TemporaryFunction();
	typedef GLBaseBufferObject<unsigned int> GLBaseBufferObjectUnsignedInt;
	typedef GLBaseBufferObject<float> GLBaseBufferObjectFloat;
}
#include "GLBaseBufferObject.cpp"
#endif //GLBASE_BUFFER_OBJECT_H
