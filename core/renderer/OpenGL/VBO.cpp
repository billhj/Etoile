/**
* Copyright(C) 2009-2012                
* @author Jing HUANG
* @file VBO.cpp
* @brief 
* @date 1/2/2011
*/

#include "VBO.h"
#include <string>

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

	VBO::VBO(GLsizei size, const float* data, GLenum usage): _usage(usage), _size(size)
	{
		_floatSize = sizeof(float);
		printOpenGLError();
		glGenBuffersARB(1, &_vboId);
		printOpenGLError();
		glBindBufferARB(GL_ARRAY_BUFFER_ARB, _vboId);
		glBufferDataARB( GL_ARRAY_BUFFER_ARB, _size * _floatSize, data, usage );
		printOpenGLError();
	}


	VBO::~VBO()
	{
		printOpenGLError();
		glDeleteBuffersARB(1, &_vboId);
		printOpenGLError();

	}


	void VBO::writeData(GLint offset, GLsizei size, const float* data)
	{
		// bind the current buffer by ID
		use();
		if ( size == _size && offset == 0 ) 
		{
			// Update the complete Buffer
			// (GLenum target, GLsizeiptrARB size, const GLvoid* data, GLenum usage);
			//GL_STATIC_DRAW_ARB GL_STATIC_READ_ARB GL_STATIC_COPY_ARB GL_DYNAMIC_DRAW_ARB GL_DYNAMIC_READ_ARB GL_DYNAMIC_COPY_ARB
			//GL_STREAM_DRAW_ARB GL_STREAM_READ_ARB GL_STREAM_COPY_ARB
			glBufferDataARB(GL_ARRAY_BUFFER_ARB,  size * _floatSize, data, getUsage());
		}
		else 
		{
			// Update part of the buffer
			glBufferSubDataARB( GL_ARRAY_BUFFER_ARB, offset * _floatSize, size * _floatSize, data);
		}
		unUse();
	}

	void VBO::readData(GLint offset, GLsizei size, const float* pDest)
	{
		// Maybe an assert if the buffer cannot be read... due to usage parameter
		// bind the current buffer by ID
		use();
		// (GLenum target, GLintptrARB offset, GLsizeiptrARB size, GLvoid* data);
		glGetBufferSubDataARB( GL_ARRAY_BUFFER_ARB, offset * _floatSize, size * _floatSize, (GLvoid*)pDest);
		printOpenGLError();
		unUse();
	}

	float* VBO::useMap()
	{
		use();
		float* ptr = (float*)glMapBufferARB(GL_ARRAY_BUFFER_ARB, GL_WRITE_ONLY_ARB);
		return ptr;
	}

	void VBO::unUseMap()
	{
		glUnmapBufferARB(GL_ARRAY_BUFFER_ARB); // unmap it after use
		unUse();
	}

	void VBO::updateByMap(GLint offset, GLsizei size, const float* data)
	{
		float* pDest = useMap();
		memcpy(&pDest[offset * _floatSize], data, size * _floatSize);
		unUseMap();
	}

	void VBO::use( GLuint uiAttribLocation, int nbComponents) const
	{
		// bind the current buffer by ID
		glBindBufferARB( GL_ARRAY_BUFFER_ARB, getID() );
		glVertexAttribPointer( uiAttribLocation, nbComponents, GL_FLOAT, GL_FALSE, 0, 0);
		printOpenGLError();
		glEnableVertexAttribArray(uiAttribLocation);
	}

	void VBO::unUse( GLuint uiAttribLocation) const
	{
		glDisableVertexAttribArray(uiAttribLocation);
		glBindBufferARB(GL_ARRAY_BUFFER_ARB, 0);
	}

	void VBO::drawVBO(GLenum primitive, int numberComponents)
	{
		use();
		glVertexPointer(numberComponents, GL_FLOAT, 0, 0);
		glEnableClientState(GL_VERTEX_ARRAY);
		glDrawArrays(primitive, 0, _size);
		glDisableClientState(GL_VERTEX_ARRAY);
		printOpenGLError();
		unUse();
	}

	void VBO::write(unsigned int offset, float value)
	{
		updateByMap(offset, 1, &value);
	}

	void VBO::write(unsigned int offset, std::vector<float>& value)
	{
		if(value.size() > 0)
		{
			updateByMap(offset, value.size(), &value[0]);
		}
	}

	void VBO::write(unsigned int offset, unsigned int nb, float* value)
	{
		updateByMap(offset, nb, value);
	}
	
}
