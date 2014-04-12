/**
* Copyright(C) 2009-2012                
* @author Jing HUANG
* @file FBO.h
* @brief 
* @date 1/2/2011
*/

#pragma once
#include "glhead.h"
#include <vector>

namespace Etoile
{

	static int FBOWIDTH = 800,  FBOHEIGHT = 600;

	class FBO
	{
	public:
		FBO();
		~FBO();
		void releaseResource();
		bool initialize(int width = FBOWIDTH, int height = FBOHEIGHT);


		bool attachColorBuffer(GLuint colorBuffer, int attachementIndex);
		bool attachDepthBuffer(GLuint depthBuffer);
		bool attachStencilBuffer(GLuint stencilBuffer);

		void bindTexture2DToBuffer(GLuint pTex, GLenum buffer);
		void bindTexture2DToDepthBuffer(GLuint texture);


		void bindTexture2DToStencilBuffer(GLuint texture);


		void bindTexture2DToColorBuffer(GLuint texture, int attachementIndex);

		void bindTextureArraysLayer(const std::vector<GLuint>& textureArray, GLint level);
		void unbindTextureArrays();

		void bindTexture2D(const std::vector<GLuint>& textures);
		void unbindTexture2D(const std::vector<GLuint>& textures);

		void beginTextureBinding();
		void textureBinding(GLuint texture);
		void endTextureBinding();
		void unbindTexture2D(unsigned int size);


		void beginDrawToColorMipmapLevel(GLuint texture, GLuint attachementIndex, GLuint level); // Use only if you know what you're doing!
		void endDrawingMipmapLevels(int maxLevel = 10);                            // Use only if you know what you're doing!

		//void generateCustomMipMap(GLuint texture, int attachementIndex = 0);

		int getWidth() const{ return _width;}
		int getHeight() const{return _height;}
		bool CheckFramebufferStatus(bool silent);
		GLuint getID() const{
			return _fboId;
		}
	private:
		bool createColorBuffer(int mode);
		bool creatDepthBuffer(int mode);

		void bind();
		void unbind();

		GLuint _fboId;
		GLuint _depth_id, _stencil_id;
		GLuint *_color_id;
		int _maxColorBuffers;
		int _width, _height;

		int _incre_textureBinding;
	};

}
