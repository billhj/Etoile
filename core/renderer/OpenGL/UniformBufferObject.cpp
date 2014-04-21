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
	template <class DataType>
	void UniformBufferObject<DataType>::check()
	{
#if defined(_DEBUG) || defined(DEBUG)
		GLint UniformBufferMax = 0;
		glGetIntegerv(GL_MAX_UNIFORM_BUFFER_BINDINGS, &UniformBufferMax);
		std::cout<<"max UniformBuffer:"<<UniformBufferMax<<std::endl;
#endif
	}

	template <class DataType>
	UniformBufferObject<DataType>::UniformBufferObject()
	{
		_target = GL_UNIFORM_BUFFER;
		_usage = GL_READ_WRITE;
	}

	template <class DataType>
	UniformBufferObject<DataType>::UniformBufferObject(GLsizei size, DataType* data)
	{
		_size = size;
		_target = GL_UNIFORM_BUFFER;
		_usage = GL_READ_WRITE;
		bindData(size, data);
	}

	template <class DataType>
	UniformBufferObject<DataType>::~UniformBufferObject()
	{
	
	}

	template <class DataType>
	void UniformBufferObject<DataType>::bindToBindingPoint(GLuint blockbindingpoint) const
	{
		// bind the current buffer by ID
		use();
		glBindBufferBase(_target, blockbindingpoint, _id);
		printOpenGLError();
	}
}
