/**
* Copyright(C) 2009-2012                
* @author Jing HUANG
* @file UniformBufferObject.cpp
* @brief 
* @date 1/2/2011
*/

#include "UniformBufferObject.h"
#include <cstring>

/**
* @brief For tracking memory leaks under windows using the crtdbg
*/
#if ( defined( _DEBUG ) || defined( DEBUG ) ) && defined( _MSC_VER )
#define _CRTDBG_MAP_ALLOC
#include <crtdbg.h>

#define DEBUG_NEW new( _NORMAL_BLOCK, __FILE__, __LINE__ )
#define new DEBUG_NEW
#endif

namespace Etoile
{

	UniformBufferObject::UniformBufferObject(GLsizei sizeofbytes, GLenum usage, const void* data)
	{

#if defined(_DEBUG) || defined(DEBUG)
		GLint UniformBufferMax = 0;
		glGetIntegerv(GL_MAX_UNIFORM_BUFFER_BINDINGS, &UniformBufferMax);
		std::cout<<"max UniformBuffer:"<<UniformBufferMax<<std::endl;
#endif
		_size = sizeofbytes;
		_usage = usage;
		printOpenGLError();
		glGenBuffersARB(1, &_UniformBufferObjectId);
		glBindBufferARB(GL_UNIFORM_BUFFER, _UniformBufferObjectId);
		glBufferDataARB(GL_UNIFORM_BUFFER, sizeofbytes, data, usage);
		printOpenGLError();
	}


	UniformBufferObject::~UniformBufferObject()
	{
		printOpenGLError();
		glDeleteBuffersARB(1, &_UniformBufferObjectId);
		printOpenGLError();

	}


	//donot use, useSubData,   wonde only for init
	void UniformBufferObject::writeData( GLsizei sizeofbytes, const void* data )
	{
		_size = sizeofbytes;
		printOpenGLError();
		// bind the current buffer by ID
		glBindBufferARB( GL_UNIFORM_BUFFER, getID());
		printOpenGLError();
		glBufferDataARB(GL_UNIFORM_BUFFER, sizeofbytes, data, getUsage());
		// Set the Data
		printOpenGLError();
	}

	void UniformBufferObject::writeSubData( GLint offsetbytes, GLsizei sizeofbytes, const void* data)
	{
		// bind the current buffer by ID
		glBindBufferARB( GL_UNIFORM_BUFFER, getID());

		// Update part of the buffer
		glBufferSubDataARB( GL_UNIFORM_BUFFER, offsetbytes, sizeofbytes, data );
		printOpenGLError();
	}


	//void UniformBufferObject::readData( GLint offset, GLsizei size, void* pDest )
	//{
	//	// Maybe an assert if the buffer cannot be read... due to usage parameter

	//	// bind the current buffer by ID
	//	glBindBufferARB( GL_ARRAY_BUFFER_ARB, getID() );
	//	// (GLenum target, GLintptrARB offset, GLsizeiptrARB size, GLvoid* data);
	//	glGetBufferSubDataARB( GL_ARRAY_BUFFER_ARB,  getSizeofBytes(offset,_eInternalType), getSizeofBytes(size,_eInternalType), pDest );

	//	printOpenGLError();
	//}



	void UniformBufferObject::use( GLuint blockbindingpoint ) const
	{
		// bind the current buffer by ID
		glBindBufferARB( GL_UNIFORM_BUFFER, getID() );
		glBindBufferBase(GL_UNIFORM_BUFFER, blockbindingpoint, _UniformBufferObjectId);

		printOpenGLError();
	}
}
