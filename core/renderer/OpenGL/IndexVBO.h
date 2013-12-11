/**
* Copyright(C) 2009-2012                
* @author Jing HUANG
* @file IndexVBO.h
* @brief 
* @date 1/2/2011
*/

#pragma once
#include "assert.h"
#include "glhead.h"
#include "glfunctions.h"

namespace Etoile
{
	class IndexVBO
	{
	public:
		IndexVBO(GLsizei size, const int* data, GLenum usage);
		~IndexVBO();
		void use()
		{
			glBindBufferARB( GL_ELEMENT_ARRAY_BUFFER_ARB, getID() );
			printOpenGLError();
		}
		void unUse()
		{
			glBindBufferARB(GL_ELEMENT_ARRAY_BUFFER_ARB, 0);
			printOpenGLError();
		}
		void writeData( GLint offset, GLsizei size, const int* data );
		void readData( GLint offset, GLsizei size, int* pDest );
		
		GLuint getID()
		{
			return _vboId;
		}

		GLenum getUsage()
		{
			return _usage;
		}

		GLsizei getSize(){
			return _size;
		}
		void use( GLuint uiAttribLocation ) const;
	private:
	
		GLuint _vboId;
		GLenum _usage;
		GLsizei _size;
		size_t _unsignedIntSize;
	};

}
