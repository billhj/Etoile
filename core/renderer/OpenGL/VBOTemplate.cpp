/**
* Copyright(C) 2009-2012                
* @author Jing HUANG
* @file VBOTemplate.cpp
* @brief 
* @date 1/2/2011
*/

#include "VBOTemplate.h"
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
	template<class T>
	VBOTemplate<T>::VBOTemplate(GLsizei size, const T* data, GLenum usage): _usage(usage), _size(size)
	{
		_tSize = sizeof(T);
		printOpenGLError();
		glGenBuffersARB(1, &_vboId);
		printOpenGLError();
		glBindBufferARB(GL_ARRAY_BUFFER_ARB, _vboId);
		glBufferDataARB( GL_ARRAY_BUFFER_ARB, _size * _tSize, data, usage );
		printOpenGLError();
	}

	template<class T>
	VBOTemplate<T>::~VBOTemplate()
	{
		printOpenGLError();
		glDeleteBuffersARB(1, &_vboId);
		printOpenGLError();

	}

	template<class T>
	void VBOTemplate<T>::writeData(GLint offset, GLsizei size, const float* data)
	{
		// bind the current buffer by ID
		use();
		if ( size == _size && offset == 0 ) 
		{
			// Update the complete Buffer
			// (GLenum target, GLsizeiptrARB size, const GLvoid* data, GLenum usage);
			//GL_STATIC_DRAW_ARB GL_STATIC_READ_ARB GL_STATIC_COPY_ARB GL_DYNAMIC_DRAW_ARB GL_DYNAMIC_READ_ARB GL_DYNAMIC_COPY_ARB
			//GL_STREAM_DRAW_ARB GL_STREAM_READ_ARB GL_STREAM_COPY_ARB
			glBufferDataARB(GL_ARRAY_BUFFER_ARB,  size * _tSize, data, getUsage());
		}
		else 
		{
			// Update part of the buffer
			glBufferSubDataARB( GL_ARRAY_BUFFER_ARB, offset * _tSize, size * _tSize, data);
		}
		unUse();
	}

	template<class T>
	void VBOTemplate<T>::readData(GLint offset, GLsizei size, const float* pDest)
	{
		// Maybe an assert if the buffer cannot be read... due to usage parameter
		// bind the current buffer by ID
		use();
		// (GLenum target, GLintptrARB offset, GLsizeiptrARB size, GLvoid* data);
		glGetBufferSubDataARB( GL_ARRAY_BUFFER_ARB, offset * _tSize, size * _tSize, (GLvoid*)pDest);
		printOpenGLError();
		unUse();
	}

	template<class T>
	float* VBOTemplate<T>::useMap()
	{
		use();
		float* ptr = (float*)glMapBufferARB(GL_ARRAY_BUFFER_ARB, GL_WRITE_ONLY_ARB);
		return ptr;
	}

	template<class T>
	void VBOTemplate<T>::unUseMap()
	{
		glUnmapBufferARB(GL_ARRAY_BUFFER_ARB); // unmap it after use
		unUse();
	}

	template<class T>
	void VBOTemplate<T>::updateByMap(GLint offset, GLsizei size, const float* data)
	{
		float* pDest = useMap();
		memcpy(&pDest[offset * _tSize], data, size * _tSize);
		unUseMap();
	}

	template<class T>
	void VBOTemplate<T>::use( GLuint uiAttribLocation, int nbComponents) const
	{
		// bind the current buffer by ID
		glBindBufferARB( GL_ARRAY_BUFFER_ARB, getID() );
		glVertexAttribPointer( uiAttribLocation, nbComponents, GL_FLOAT, GL_FALSE, 0, 0);
		printOpenGLError();
		glEnableVertexAttribArray(uiAttribLocation);
	}

	template<class T>
	void VBOTemplate<T>::unUse( GLuint uiAttribLocation) const
	{
		glDisableVertexAttribArray(uiAttribLocation);
		glBindBufferARB(GL_ARRAY_BUFFER_ARB, 0);
	}

	template<class T>
	void VBOTemplate<T>::drawVBOTemplate(GLenum primitive, int numberComponents)
	{
		use();
		glVertexPointer(numberComponents, GL_FLOAT, 0, 0);
		glEnableClientState(GL_VERTEX_ARRAY);
		glDrawArrays(primitive, 0, _size);
		glDisableClientState(GL_VERTEX_ARRAY);
		printOpenGLError();
		unUse();
	}

	template<class T>
	void VBOTemplate<T>::write(unsigned int offset, float value)
	{
		updateByMap(offset, 1, &value);
	}

	template<class T>
	void VBOTemplate<T>::write(unsigned int offset, std::vector<float>& value)
	{
		if(value.size() > 0)
		{
			updateByMap(offset, value.size(), &value[0]);
		}
	}

	template<class T>
	void VBOTemplate<T>::write(unsigned int offset, unsigned int nb, float* value)
	{
		updateByMap(offset, nb, value);
	}
	
}
