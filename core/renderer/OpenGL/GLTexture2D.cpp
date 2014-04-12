/**
* Copyright(C) 2009-2012                
* @author Jing HUANG
* @file GLTexture2D.cpp
* @brief 
* @date 1/2/2011
*/

#include "GLTexture2D.h"

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
	GLTexture2D::GLTexture2D(const std::string& name) : GLTexture(name)
	{

	}

	GLTexture2D::GLTexture2D(const std::string& name, GLuint id) : GLTexture(name, id)
	{

	}

	bool GLTexture2D::create( int iWidth, int iHeight, int iLayer, GLenum internalFormat, GLenum pixeldataformat, GLenum type, float* data, bool bCreateMipMap)
	{
		// Create a single texture
		_width = iWidth;
		_height = iHeight;
		_layer = iLayer;
		_layer = 1;
		_mipmaped = bCreateMipMap;
		_internalFormat = internalFormat;
		glEnable( GL_TEXTURE_2D );
		use();
		if(bCreateMipMap){
			glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
			glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
			//glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
			//glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
			glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_BASE_LEVEL, 0 );
			glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAX_LEVEL, 1000);
			glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT );
			glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT );
			glTexParameteri(GL_TEXTURE_2D, GL_GENERATE_MIPMAP, GL_TRUE); // automatic mipmap
			glGenerateMipmap(GL_TEXTURE_2D);
			printOpenGLError();
		}else{
			glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
			glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
			//glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
			//glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
			glTexParameteri(GL_TEXTURE_2D, GL_GENERATE_MIPMAP, GL_FALSE); // automatic mipmap
			glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT );
			glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT );
			printOpenGLError();

		}

		glTexImage2D(GL_TEXTURE_2D, 0, _internalFormat, iWidth, iHeight, 0, pixeldataformat, type, data);


		unUse();
		glDisable( GL_TEXTURE_2D );
		printOpenGLError();
		return true;
	}

	void GLTexture2D::setGLTextureParameter(GLenum pname, GLfloat param)
	{
		GLTexture::setGLTextureParameter(GL_TEXTURE_2D, pname, param);
	}

	void GLTexture2D::use() const
	{
		printOpenGLError();
		glEnable( GL_TEXTURE_2D );
		printOpenGLError();
		glBindTexture( GL_TEXTURE_2D, getId() );
		printOpenGLError();

	}

	void GLTexture2D::unUse() const
	{
		printOpenGLError();
		glBindTexture(GL_TEXTURE_2D, 0);
		printOpenGLError();
		glDisable( GL_TEXTURE_2D );
		printOpenGLError();

	}

	void GLTexture2D::generateMipmap() const
	{
		if(_mipmaped)
		{
			glEnable(GL_TEXTURE_2D);
			printOpenGLError();
			glActiveTexture(GL_TEXTURE0);
			printOpenGLError();
			glBindTexture(GL_TEXTURE_2D, _id);
			printOpenGLError();
			glGenerateMipmap(GL_TEXTURE_2D);
			printOpenGLError();
		}

	}
	void GLTexture2D::read(float* data)
	{
		this->use();
		glGetTexImage(GL_TEXTURE_2D, 0, GL_RGBA, GL_RGBA32F_ARB, data);
		this->unUse();
	}

	void GLTexture2D::write(int xOffset, int yOffset, int width, int height, float* data)
	{
		use();
		glTexSubImage2D( GL_TEXTURE_2D, 0, xOffset, yOffset, width, height, getPixelDataFormat(_internalFormat), GL_FLOAT , data );
		unUse();

	}

	void GLTexture2D::write(int size, float* data)
	{
		use();
		int height = (int)((float) size / (float)_width + 0.5);
		glTexSubImage2D( GL_TEXTURE_2D, 0, 0, 0, _width, height, getPixelDataFormat(_internalFormat), GL_FLOAT , data);
		unUse();
	}
}
