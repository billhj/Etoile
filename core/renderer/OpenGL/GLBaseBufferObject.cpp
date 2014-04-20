/**
* Copyright(C) 2009-2012                
* @author Jing HUANG
* @file GLBaseBufferObject.cpp
* @brief 
* @date 1/2/2011
*/
#include "GLBaseBufferObject.h"

namespace Etoile
{
	template <class DataType>
	GLBaseBufferObject<DataType>::GLBaseBufferObject()
	{
		printOpenGLError();
		glGenBuffersARB(1, &_id);
		printOpenGLError();
	}

	template <class DataType>
	GLBaseBufferObject<DataType>::~GLBaseBufferObject()
	{
		glDeleteBuffersARB(1, &_id);
		printOpenGLError();
	}

	template <class DataType>
	void GLBaseBufferObject<DataType>::use()
	{
		glBindBufferARB(_target, _id);
		printOpenGLError();
	}

	template <class DataType>
	void GLBaseBufferObject<DataType>::unUse()
	{
		glBindBufferARB(_target, 0);
		printOpenGLError();
	}

	template <class DataType>
	void GLBaseBufferObject<DataType>::setTarget(GLenum target)
	{
		_target = target;
	}

	template <class DataType>
	void GLBaseBufferObject<DataType>::setUsage(GLenum usage)
	{
		_usage = usage;
	}

	template <class DataType>
	GLuint GLBaseBufferObject<DataType>::id()
	{
		return _id;
	}

	template <class DataType>
	GLenum GLBaseBufferObject<DataType>::usage()
	{
		return _usage;
	}

	template <class DataType>
	GLenum GLBaseBufferObject<DataType>::target()
	{
		return _target;
	}

	template <class DataType>
	GLsizei GLBaseBufferObject<DataType>::dataSize()
	{
		return _size;
	}

	template <class DataType>
	void GLBaseBufferObject<DataType>::bindData(GLsizei datasize, DataType* data)
	{
		use();
		_size = datasize;
		//Buffer respeciﬁcation, detach the original memory,
		glBufferDataARB(_target,  datasize * sizeof(DataType), NULL, _usage);
		glBufferDataARB(_target,  datasize * sizeof(DataType), data, _usage);
		unUse();
	}

	template <class DataType>
	void GLBaseBufferObject<DataType>::bindSubData(GLint offset, GLsizei datasize, DataType* data)
	{
		use();
		int sizet = sizeof(DataType);
		//not sure for subdata. Buffer respeciﬁcation, detach the original memory,
		//glBufferDataARB(_target,  datasize * sizeof(DataType), NULL, _usage);
		glBufferSubDataARB(_target, offset * sizet,  datasize * sizet, data);
		unUse();
	}

	template <class DataType>
	DataType* GLBaseBufferObject<DataType>::useMap(GLenum accessType)
	{
		use();
		DataType* ptr = (float*)glMapBufferARB(_target, accessType);
		return ptr;
	}

	template <class DataType>
	void GLBaseBufferObject<DataType>::unUseMap()
	{
		glUnmapBufferARB(_target); // unmap it after use
		unUse();
	}

	template <class DataType>
	DataType* GLBaseBufferObject<DataType>::useMapForRead()
	{
		return useMap(GL_READ_ONLY);
	}

	template <class DataType>
	DataType* GLBaseBufferObject<DataType>::useMapForWrite()
	{
		return useMap(GL_WRITE_ONLY);
	}


	template <class DataType>
	DataType* GLBaseBufferObject<DataType>::useMapForReadAndWrite()
	{
		return useMap(GL_READ_WRITE);
	}

	template <class DataType>
	DataType* GLBaseBufferObject<DataType>::useMapRange(GLint offset, GLsizei datasize)
	{
		use();
		int sizet = sizeof(DataType);
		glMapBufferRange(_target, offset * sizet,  datasize * sizet, GL_MAP_WRITE_BIT | GL_MAP_INVALIDATE_BUFFER_BIT);
	}

	// No need to call this TemporaryFunction() function,
	// it's just to avoid link error.
	/*void TemporaryFunction ()
	{
		GLBaseBufferObjectUnsignedInt TempObj;
		TempObj.unUse();
		TempObj.use();
		GLBaseBufferObjectFloat TempObj2;
	}*/
}

