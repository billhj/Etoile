/**
* Copyright(C) 2009-2012                
* @author Jing HUANG
* @file IndexVBO.cpp
* @brief 
* @date 1/2/2011
*/

#include "IndexVBO.h"

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

	IndexVBO::IndexVBO(GLsizei size, const int* data, GLenum usage): _usage( usage ), _size(size)
	{
		_unsignedIntSize = sizeof(GL_UNSIGNED_INT);
		glGenBuffersARB(1, &_vboId);
		glBindBufferARB(GL_ELEMENT_ARRAY_BUFFER_ARB, _vboId);
		glBufferDataARB(GL_ELEMENT_ARRAY_BUFFER_ARB, size * _unsignedIntSize, data, usage );
		printOpenGLError();
	}


	IndexVBO::~IndexVBO()
	{
		printOpenGLError();
		glDeleteBuffersARB(1, &_vboId);
		printOpenGLError();
	}


	void IndexVBO::writeData( GLint offset, GLsizei size, const int* data )
	{
		// bind the current buffer by ID
		use();
		if ( size == _size && offset == 0 ) 
		{
			// Update the complete Buffer
			// (GLenum target, GLsizeiptrARB size, const GLvoid* data, GLenum usage);
			//GL_STATIC_DRAW_ARB GL_STATIC_READ_ARB GL_STATIC_COPY_ARB GL_DYNAMIC_DRAW_ARB GL_DYNAMIC_READ_ARB GL_DYNAMIC_COPY_ARB
			//GL_STREAM_DRAW_ARB GL_STREAM_READ_ARB GL_STREAM_COPY_ARB

			glBufferDataARB(GL_ELEMENT_ARRAY_BUFFER_ARB, size * _unsignedIntSize, data, getUsage());
		}
		else 
		{
			// Update part of the buffer
			glBufferSubDataARB( GL_ELEMENT_ARRAY_BUFFER_ARB, offset * _unsignedIntSize, size * _unsignedIntSize, data);
		}
		unUse();
	}

	void IndexVBO::readData( GLint offset, GLsizei size, int* pDest )
	{
		// bind the current buffer by ID
		use();
		// (GLenum target, GLintptrARB offset, GLsizeiptrARB size, GLvoid* data);
		glGetBufferSubDataARB( GL_ELEMENT_ARRAY_BUFFER_ARB,  offset * _unsignedIntSize, size * _unsignedIntSize, pDest );
		unUse();
	}

	void IndexVBO::use( GLuint uiAttribLocation ) const
	{
		glVertexAttribPointer( uiAttribLocation, 3, GL_UNSIGNED_INT, GL_FALSE, 0, 0 );
	}

}
