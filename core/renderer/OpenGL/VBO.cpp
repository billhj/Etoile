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
	template <class DataType>
	VBO<DataType>::VBO()
	{
		_target = GL_ARRAY_BUFFER_ARB;
		_usage = GL_STATIC_DRAW_ARB;
	}

	template <class DataType>
	VBO<DataType>::VBO(GLsizei size, DataType* data)
	{
		_size = size;
		printOpenGLError();
		_target = GL_ARRAY_BUFFER_ARB;
		_usage = GL_STATIC_DRAW_ARB;
		bindData(size, data);
	}

	template <class DataType>
	VBO<DataType>::~VBO()
	{
	}
	
}
