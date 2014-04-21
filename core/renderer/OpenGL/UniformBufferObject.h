/**
* Copyright(C) 2009-2012                
* @author Jing HUANG
* @file UniformBufferObject.h
* @brief 
* @date 1/2/2011
*/

#pragma once
#ifndef UNIFORM_BUFFER_OBJECT_H
#define UNIFORM_BUFFER_OBJECT_H
#include "assert.h"
#include "GLBaseBufferObject.h"

namespace Etoile
{
	template <class DataType>
	class UniformBufferObject : public GLBaseBufferObject<DataType>
	{
	public:
		UniformBufferObject();
		UniformBufferObject(GLsizei size, DataType* data);
		virtual~UniformBufferObject();
		void bindToBindingPoint(GLuint blockbindingpoint) const;
		void check();
	};
	//typedef UniformBufferObject<float> UniformBufferObjectFloat;
}

#include "UniformBufferObject.cpp"
#endif //UNIFORM_BUFFER_OBJECT_H