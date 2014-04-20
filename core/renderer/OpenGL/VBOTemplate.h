/**
* Copyright(C) 2009-2012                
* @author Jing HUANG
* @file VBOTemplate.h
* @brief 
* @date 1/2/2011
*/

#pragma once
#include "assert.h"
#include "glhead.h"
#include <vector>

namespace Etoile
{
	template <class T>
	class VBOTemplate
	{
	public:
		//float size
		VBOTemplate(GLsizei size, const T* data = 0, GLenum usage = GL_STATIC_DRAW_ARB);
		~VBOTemplate();
		void use()
		{
			glBindBufferARB(GL_ARRAY_BUFFER_ARB, getID() );
			printOpenGLError();
		}
		void unUse()
		{
			glBindBufferARB(GL_ARRAY_BUFFER_ARB, 0);
			printOpenGLError();
		}

		void writeData(GLint offset, GLsizei size, const float* data);
		void readData(GLint offset, GLsizei size, const float* pDest);
		void updateByMap(GLint offset, GLsizei size, const float* data);
	
		GLuint getID() const
		{
			return _vboId;
		}

		GLenum getUsage()
		{
			return _usage;
		}

		GLsizei getSize()
		{
			return _size;
		}

		void use(GLuint uiAttribLocation, int nbComponents) const;
		void unUse( GLuint uiAttribLocation) const;
		void drawVBOTemplate(GLenum primitive = GL_TRIANGLES, int numberComponents = 4);

		virtual void write(unsigned int offset, float value) ;
		virtual void write(unsigned int offset, std::vector<float>&value) ;
		virtual void write(unsigned int offset, unsigned int nb, float* value) ;

	protected:
		void unUseMap();
		float* useMap();
	private:
		GLuint _vboId;
		GLenum _usage;
		GLsizei _size;
		size_t _tSize;
	};

}
