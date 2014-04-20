/**
* Copyright(C) 2009-2012                
* @author Jing HUANG
* @file IndexVBO.h
* @brief 
* @date 1/2/2011
*/

#pragma once
#ifndef INDEX_BUFFER_OBJECT_H
#define INDEX_BUFFER_OBJECT_H
#include "GLBaseBufferObject.h"

namespace Etoile
{
	class IndexVBO : public GLBaseBufferObject<unsigned int>
	{
	public:
		IndexVBO();
		IndexVBO(GLsizei size, unsigned int* data);
		virtual~IndexVBO();
	};

}

#endif //INDEX_BUFFER_OBJECT_H
