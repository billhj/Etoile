/**
* Copyright(C) 2009-2012                
* @author Jing HUANG
* @file GLTextureCube.h
* @brief 
* @date 1/2/2011
*/

#pragma once
#include "GLTexture.h"

namespace Etoile
{
	class GLTextureCube : public GLTexture
	{
	public:
		GLTextureCube(const std::string& name);
		GLTextureCube(const std::string& name, GLuint id);

		virtual bool create(int iWidth, int iHeight,int _iLayer, GLenum internalFormat = GL_RGBA16F_ARB, GLenum pixeldataformat = GL_RGBA, GLenum type = GL_FLOAT, float* data = 0, bool bCreateMipMap = false) override;
		virtual void setGLTextureParameter(GLenum pname, GLfloat param) override;
		virtual void use() const override;
		virtual void unUse() const override;
		virtual void generateMipmap() const override;
		virtual void read(float* data) override{data = data;}
		void write(int xOffset, int yOffset, int width, int height, float* data)
		{
			use();
			glTexSubImage2D( GL_TEXTURE_2D, 0, xOffset, yOffset, width, height, getPixelDataFormat(_internalFormat), GL_FLOAT , data );
			unUse();
		}

		void write(int size, float* data)
		{
			use();
			int height = (int)((float) size / (float)_width + 0.5);
			glTexSubImage2D( GL_TEXTURE_2D, 0, 0, 0, _width, height, getPixelDataFormat(_internalFormat), GL_FLOAT , data);
			unUse();
		}
	private:

	};
}
