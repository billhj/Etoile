/**
* Copyright(C) 2009-2012                
* @author Jing HUANG
* @file IBO.h
* @brief 
* @date 1/2/2011siz
*/

#pragma once
#ifndef INDEX_BUFFER_OBJECT_H
#define INDEX_BUFFER_OBJECT_H
#include "GLBaseBufferObject.h"

namespace Etoile
{
	class IBO : public GLBaseBufferObject<unsigned int>
	{
	public:
		IBO();
		IBO(GLsizei size, unsigned int* data);
		virtual~IBO();
	};

}

#endif //INDEX_BUFFER_OBJECT_H
