/**
* Copyright(C) 2009-2012                
* @author Jing HUANG
* @file UniformBufferObject.h
* @brief 
* @date 1/2/2011
*/

#pragma once
#include "assert.h"
#include "glhead.h"

namespace Etoile
{
	//template<class T>
	class UniformBufferObject
	{
	public:
		UniformBufferObject(GLsizei sizeofbytes, GLenum usage, const void* data = 0);
		~UniformBufferObject();

		void writeData(GLsizei sizeofbytes, const void* data);
		void writeSubData( GLint offsetbytes, GLsizei sizeofbytes, const void* data);

		void use()
		{
			glBindBufferARB( GL_UNIFORM_BUFFER, getID() );
		}
		void unUse()
		{
			glBindBufferARB(GL_UNIFORM_BUFFER, 0);

		}
		GLuint getID() const
		{
			return _UniformBufferObjectId;
		}

		GLenum getUsage()
		{
			return _usage;
		}

		GLsizei getSize(){
			return _size;
		}
		void use( GLuint blockbindingpoint ) const;
	private:

		GLuint _UniformBufferObjectId;
		GLenum _usage;
		GLsizei _size;

	};


	//typedef UniformBufferObject<float> UniformBufferObjectFloat;
}
