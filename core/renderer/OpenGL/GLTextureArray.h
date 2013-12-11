/**
* Copyright(C) 2009-2012                
* @author Jing HUANG
* @file GLTextureArray.h
* @brief 
* @date 1/2/2011
*/

#pragma once

#include "GLTexture.h"

namespace Etoile
{
	class GLTextureArray : public GLTexture
	{
	public:
		GLTextureArray(const std::string& name);
		GLTextureArray(const std::string& name, GLuint id);

		virtual bool create(  int iWidth, int iHeight,int _iLayer, GLenum internalFormat = GL_RGBA16F_ARB, GLenum pixeldataformat = GL_RGBA, GLenum type = GL_FLOAT, float* data = 0, bool bCreateMipMap = false) override;
		virtual void setGLTextureParameter(GLenum pname, GLfloat param) override;
		virtual void use() const override;
		virtual void unUse() const override;
		virtual void generateMipmap() const override;
		virtual void read(float* data) override;
		virtual void write(int x, int y, int width, int height, float* data) override;
		virtual void write(int size, float* data) override;
		void readGLTextureArrayLayer(int layer, float* data);

	};

}
