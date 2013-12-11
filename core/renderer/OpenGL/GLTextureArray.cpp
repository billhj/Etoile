/**
* Copyright(C) 2009-2012                
* @author Jing HUANG
* @file GLTextureArray.cpp
* @brief 
* @date 1/2/2011
*/

#include "GLTextureArray.h"

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
	GLTextureArray::GLTextureArray(const std::string& name): GLTexture(name)
	{
		printOpenGLError();
	}

	GLTextureArray::GLTextureArray(const std::string& name, GLuint id): GLTexture(name, id)
	{
	}

	bool GLTextureArray::create( int iWidth, int iHeight, int iLayer, GLenum internalFormat, GLenum pixeldataformat, GLenum type, float* data, bool bCreateMipMap)
	{
		_width = iWidth;
		_height = iHeight;
		_layer = iLayer;
		_mipmaped = bCreateMipMap;
		_internalFormat = internalFormat;
		printOpenGLError();
		glBindTexture(GL_TEXTURE_2D_ARRAY_EXT, _id);
		printOpenGLError();

		if(bCreateMipMap){

			glTexParameteri(GL_TEXTURE_2D_ARRAY_EXT,GL_TEXTURE_MIN_FILTER,GL_LINEAR_MIPMAP_LINEAR);

			glTexParameteri(GL_TEXTURE_2D_ARRAY_EXT,GL_TEXTURE_MAG_FILTER,GL_LINEAR);

			glTexParameteri(GL_TEXTURE_2D_ARRAY_EXT,GL_TEXTURE_WRAP_S,GL_CLAMP_TO_EDGE);

			glTexParameteri(GL_TEXTURE_2D_ARRAY_EXT,GL_TEXTURE_WRAP_T,GL_CLAMP_TO_EDGE);
		}else{

			glTexParameteri(GL_TEXTURE_2D_ARRAY_EXT,GL_TEXTURE_MIN_FILTER,GL_LINEAR);

			glTexParameteri(GL_TEXTURE_2D_ARRAY_EXT,GL_TEXTURE_MAG_FILTER,GL_LINEAR);

			glTexParameteri(GL_TEXTURE_2D_ARRAY_EXT,GL_TEXTURE_WRAP_S,GL_CLAMP_TO_EDGE);

			glTexParameteri(GL_TEXTURE_2D_ARRAY_EXT,GL_TEXTURE_WRAP_T,GL_CLAMP_TO_EDGE);

		}

		printOpenGLError();
		glTexImage3D(GL_TEXTURE_2D_ARRAY_EXT,0, _internalFormat, _width, _height,_layer, 0, pixeldataformat, type, data);
		printOpenGLError();

		return true;


	}

	void GLTextureArray::setGLTextureParameter(GLenum pname, GLfloat param)
	{
		GLTexture::setGLTextureParameter(GL_TEXTURE_2D_ARRAY_EXT, pname, param);
	}

	void GLTextureArray:: use() const
	{
		printOpenGLError();
		glEnable( GL_TEXTURE_2D_ARRAY );
		printOpenGLError();
		glBindTexture( GL_TEXTURE_2D_ARRAY, getId() );
		printOpenGLError();

	}

	void GLTextureArray::  unUse() const
	{
		printOpenGLError();
		glBindTexture(GL_TEXTURE_2D_ARRAY, 0);
		printOpenGLError();

	}

	void GLTextureArray::generateMipmap() const
	{
		if(_mipmaped)
		{
			glEnable(GL_TEXTURE_2D_ARRAY);
			printOpenGLError();
			glActiveTexture(GL_TEXTURE0);
			printOpenGLError();
			glBindTexture(GL_TEXTURE_2D_ARRAY, _id);
			printOpenGLError();
			glGenerateMipmap(GL_TEXTURE_2D_ARRAY);
			printOpenGLError();
		}
	}
	
	void  GLTextureArray::read(float* data)
	{
		this->use();
		glGetTexImage(GL_TEXTURE_3D, 0, GL_RGBA, GL_RGBA32F_ARB, data);
		this->unUse();
	}

	void GLTextureArray::readGLTextureArrayLayer(int layer, float* data)
	{
		read(data);
		int w = this->getWidth();
		int h = this->getHeight();
		data = data + layer * w * h * 4;
	}

	void GLTextureArray::write(int xOffset, int yOffset, int width, int height, float* data)
	{
		use();
		glTexSubImage2D( GL_TEXTURE_2D, 0, xOffset, yOffset, width, height, getPixelDataFormat(_internalFormat), GL_FLOAT , data );
		unUse();

	}

	void GLTextureArray::write(int size, float* data)
	{
		use();
		int height = (int)((float) size / (float)_width + 0.5);
		glTexSubImage2D( GL_TEXTURE_2D, 0, 0, 0, _width, height, getPixelDataFormat(_internalFormat), GL_FLOAT , data);
		unUse();
	}
}
