/**
* Copyright(C) 2009-2012                
* @author Jing HUANG
* @file VBO.h
* @brief 
* @date 1/2/2011
*/

#pragma once
#ifndef VERTEX_BUFFER_OBJECT_H
#define VERTEX_BUFFER_OBJECT_H
#include <assert.h>
#include <vector>
#include "GLBaseBufferObject.h"

namespace Etoile
{
	template <class DataType> 
	class VBO : public GLBaseBufferObject<DataType>
	{
	public:
		VBO();
		VBO(GLsizei size, DataType* data);
		virtual~VBO();
	};

	typedef VBO<float> VBOFloat;

	struct VBOUnit
	{
		VBOFloat* _pVBO;
		std::string _attributeName;
		GLenum _primitive;
		int _numberComponents;

		VBOUnit()
		{
			_pVBO = NULL;
			_primitive = GL_TRIANGLES;
			_numberComponents = 3;
		}
		~VBOUnit()
		{
			if(_pVBO)
				delete _pVBO;
		}
	};

}
#include "VBO.cpp"

#endif //VERTEX_BUFFER_OBJECT_H
