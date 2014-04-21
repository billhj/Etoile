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
	IndexVBO::IndexVBO() : GLBaseBufferObject<unsigned int>()
	{
		//new GLBaseBufferObject<unsigned int>();
		_target = GL_ELEMENT_ARRAY_BUFFER_ARB;
		_usage = GL_STATIC_DRAW_ARB;
	}

	IndexVBO::IndexVBO(GLsizei size, unsigned int* data) : GLBaseBufferObject<unsigned int>()
	{
		_size = size;
		_target = GL_ELEMENT_ARRAY_BUFFER_ARB;
		_usage = GL_STATIC_DRAW_ARB;
		bindData(size, data);
	}

	IndexVBO::~IndexVBO()
	{
	}

	//// No need to call this TemporaryFunction() function,
	//// it's just to avoid link error.
	//void TemporaryFunction ()
	//{
	//	IndexVBO TempObj;
	//}
}
