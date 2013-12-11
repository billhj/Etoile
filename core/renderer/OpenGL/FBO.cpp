/**
* Copyright(C) 2009-2012                
* @author Jing HUANG
* @file FBO.h
* @brief 
* @date 1/2/2011
*/

#include "FBO.h"
#include <string>
#include <assert.h>

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

	FBO::FBO(): _fboId(0), _depth_id(0), _stencil_id(0),_width(FBOWIDTH), _height(FBOHEIGHT)
	{
		glGetIntegerv( GL_MAX_COLOR_ATTACHMENTS_EXT, &_maxColorBuffers );
		_color_id = new GLuint[_maxColorBuffers];

#if defined(_DEBUG) || defined(DEBUG)
		GLint value;
		glGetIntegerv(GL_MAX_COMBINED_TEXTURE_IMAGE_UNITS_ARB,&value);

		std::cout<<"max input texture samples number in one shader:"<<value<<std::endl;
		std::cout<<"max COLOR_ATTACHMENTS number:"<<_maxColorBuffers<<std::endl;
#endif

		glGenFramebuffersEXT(1, &_fboId);
		for (int i=0; i<_maxColorBuffers; i++)
			_color_id[i] = 0;
	}


	FBO::~FBO()
	{
		releaseResource();
		delete[] _color_id;
	}

	void FBO::releaseResource()
	{
		for(int i=0; i<_maxColorBuffers; i++){
			glDeleteRenderbuffersEXT(1,&(_color_id[i]));
		}
		glDeleteRenderbuffersEXT(1,&_depth_id);
		glDeleteFramebuffersEXT(1,&_fboId);

	}

	bool FBO::initialize(int width , int height )
	{
		printOpenGLError ();
#if defined(_DEBUG) || defined(DEBUG)
		std::cout<<"InitFBO: "<<width<<" "<<height<<std::endl;
#endif
		_width = width;
		_height = height;
		releaseResource();

		bind();
		printOpenGLError ();

		createColorBuffer(0);
		creatDepthBuffer(0);




		// Why the hell do you do it like that if you've got CheckFramebufferStatus function???
		//    GLenum status = glCheckFramebufferStatusEXT(GL_FRAMEBUFFER_EXT);
		//    if(status != GL_FRAMEBUFFER_COMPLETE_EXT)


		glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, 0);

		printOpenGLError();

		return true;
	}

	// bug: crash in the multisampling mode
	bool FBO::creatDepthBuffer(int mode)
	{
		printOpenGLError ();
		glGenRenderbuffersEXT(1, &_depth_id);
		printOpenGLError();
		//initialize depth renderbuffer
		glBindRenderbufferEXT(GL_RENDERBUFFER_EXT, _depth_id);
		printOpenGLError();
		if(mode == 1)
		{

		}
		else
		{
			glRenderbufferStorageEXT(GL_RENDERBUFFER_EXT, GL_DEPTH_COMPONENT24, _width ,_height);
			printOpenGLError();
			// attach renderbuffer to framebuffer depth buffer
			glFramebufferRenderbufferEXT(GL_FRAMEBUFFER_EXT,
				GL_DEPTH_ATTACHMENT_EXT, GL_RENDERBUFFER_EXT,
				_depth_id);
		}

		printOpenGLError();


		//CheckFramebufferStatus(true);
		return true;
	}

	bool FBO::createColorBuffer(int mode)
	{
		printOpenGLError ();
#if defined(_DEBUG) || defined(DEBUG)
		int samples;
		//We need to find out what the maximum supported samples is
		glGetIntegerv(GL_MAX_SAMPLES_EXT, &samples);
		std::cout<<"max nb of multisamplings antialiasing:"<<samples<<std::endl;
#endif

		glGenRenderbuffersEXT( 1, &_color_id[0]);
		glBindRenderbufferEXT(GL_RENDERBUFFER_EXT, _color_id[0]);
		printOpenGLError();

		if(mode == 1)
		{

			glRenderbufferStorageMultisampleEXT( GL_RENDERBUFFER_EXT, 4, GL_RGBA32F_ARB, _width, _height);

			GLint colorV;
			glGetIntegerv(GL_MAX_COLOR_ATTACHMENTS_EXT,&colorV);
			for(int i = 0; i< colorV; ++i){

				glFramebufferRenderbufferEXT(GL_FRAMEBUFFER_EXT, GL_COLOR_ATTACHMENT0_EXT  , GL_RENDERBUFFER_EXT, _color_id[0]);
				printOpenGLError();
			}
		}
		else
		{

			glRenderbufferStorageEXT(GL_RENDERBUFFER_EXT,  GL_RGBA32F_ARB, _width ,_height);
			glFramebufferRenderbufferEXT(GL_FRAMEBUFFER_EXT, GL_COLOR_ATTACHMENT0_EXT  , GL_RENDERBUFFER_EXT, _color_id[0]);
		}
		printOpenGLError ();
		return true;
	}



	void FBO::bind()
	{
		printOpenGLError ();
		glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, _fboId);
		printOpenGLError ();
	}

	void FBO::unbind()
	{
		printOpenGLError ();
		glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, 0);
		printOpenGLError ();
	}


	bool FBO::attachColorBuffer(GLuint colorBuffer, int attachementIndex)
	{
		// If the colorBuffer value is valid, then bind the texture to the color buffer.
		if (attachementIndex < _maxColorBuffers)
		{
			this->bind();
			glBindRenderbufferEXT(GL_RENDERBUFFER_EXT, colorBuffer);
			glRenderbufferStorageEXT(GL_RENDERBUFFER_EXT, GL_RGBA,
				_width, _height);
			glFramebufferRenderbufferEXT( GL_FRAMEBUFFER_EXT, GL_COLOR_ATTACHMENT0_EXT+attachementIndex,
				GL_RENDERBUFFER_EXT, colorBuffer);
		}
		else
			return false;
		_color_id[attachementIndex] = colorBuffer;
		return true;
	}

	bool FBO::attachDepthBuffer(GLuint depthBuffer)
	{
		this->bind();
		glFramebufferRenderbufferEXT(GL_FRAMEBUFFER_EXT, GL_DEPTH_ATTACHMENT_EXT,
			GL_RENDERBUFFER_EXT, depthBuffer);
		_depth_id = depthBuffer;
		return true;
	}

	bool FBO::attachStencilBuffer(GLuint stencilBuffer)
	{
		this->bind();
		glFramebufferRenderbufferEXT(GL_FRAMEBUFFER_EXT, GL_STENCIL_ATTACHMENT_EXT,
			GL_RENDERBUFFER_EXT, stencilBuffer);
		_stencil_id = stencilBuffer;
		return true;
	}

	void FBO::bindTexture2DToBuffer(GLuint pTex, GLenum buffer)
	{
		printOpenGLError();
		this->bind();
		glBindTexture(GL_TEXTURE_2D, pTex);
		glFramebufferTexture2DEXT(GL_FRAMEBUFFER_EXT, buffer, GL_TEXTURE_2D, pTex, 0);
		glDrawBuffer(buffer);
#if defined(_DEBUG) || defined(DEBUG)	
		CheckFramebufferStatus(false);
#endif
		printOpenGLError();
	}


	void FBO::bindTexture2DToDepthBuffer(GLuint texture)
	{
		printOpenGLError();
		this->bind();
		printOpenGLError();
		// attach the texture to FBO color attachment point
		glFramebufferTexture2DEXT(GL_FRAMEBUFFER_EXT, GL_DEPTH_ATTACHMENT_EXT, GL_TEXTURE_2D, texture, 0);
		printOpenGLError();

	}

	void FBO::bindTexture2DToStencilBuffer(GLuint texture)
	{
		printOpenGLError();
		this->bind();
		printOpenGLError();
		// attach the texture to FBO color attachment point
		glFramebufferTexture2DEXT(GL_FRAMEBUFFER_EXT, GL_STENCIL_ATTACHMENT_EXT, GL_TEXTURE_2D, texture, 0);
	}


	void FBO::bindTexture2DToColorBuffer(GLuint texture, int attachementIndex)
	{
		printOpenGLError();
		this->bind();
		glBindTexture(GL_TEXTURE_2D, texture);
		glFramebufferTexture2DEXT(GL_FRAMEBUFFER_EXT, GL_COLOR_ATTACHMENT0_EXT + attachementIndex, GL_TEXTURE_2D, texture, 0);
		glDrawBuffer(GL_COLOR_ATTACHMENT0_EXT + attachementIndex);
#if defined(_DEBUG) || defined(DEBUG)	
		CheckFramebufferStatus(false);
#endif
		printOpenGLError();

	}



	void FBO::bindTexture2D(const std::vector<GLuint>& textures)
	{
		printOpenGLError();
		this->bind();
		printOpenGLError();
		// attach the texture to FBO color attachment point
		int size = textures.size();
		glActiveTexture(GL_TEXTURE0);
		for(int i = 0; i < size; ++i){
			printOpenGLError();
			glActiveTexture(GL_TEXTURE0 + i);
			glBindTexture(GL_TEXTURE_2D, textures[i]);
			printOpenGLError();

			glFramebufferTexture2DEXT(GL_FRAMEBUFFER_EXT, GL_COLOR_ATTACHMENT0_EXT + i, GL_TEXTURE_2D, textures[i], 0);

			printOpenGLError();

		}
		printOpenGLError();
		glActiveTexture(GL_TEXTURE0);
		GLenum buffers[] = { GL_COLOR_ATTACHMENT0_EXT,
			GL_COLOR_ATTACHMENT1_EXT,
			GL_COLOR_ATTACHMENT2_EXT,
			GL_COLOR_ATTACHMENT3_EXT,
			GL_COLOR_ATTACHMENT4_EXT,
			GL_COLOR_ATTACHMENT5_EXT,
			GL_COLOR_ATTACHMENT6_EXT,
			GL_COLOR_ATTACHMENT7_EXT,
			GL_COLOR_ATTACHMENT8_EXT
		};
		glDrawBuffers(size,buffers);

#if defined(_DEBUG) || defined(DEBUG)	

		if ( !CheckFramebufferStatus( false ) )
		{
			std::cout<<"FBO create failed"<<std::endl;
			return ;
		}

#endif
		printOpenGLError();

	}



	void FBO::unbindTexture2D(const std::vector<GLuint>& textures){

		for(unsigned int i = 0; i < textures.size(); ++i){
			glFramebufferTexture2DEXT( GL_FRAMEBUFFER_EXT, GL_COLOR_ATTACHMENT0_EXT + i, GL_TEXTURE_2D, 0, 0 );
		}
		glDisable(GL_TEXTURE_2D);
		this->unbind();
		printOpenGLError();
	}



	void FBO::beginTextureBinding()
	{
		printOpenGLError();
		this->bind();
		printOpenGLError();
		_incre_textureBinding = 0;
	}

	void FBO::textureBinding(GLuint texture)
	{
		printOpenGLError();
		glActiveTexture(GL_TEXTURE0 + _incre_textureBinding);
		glBindTexture(GL_TEXTURE_2D, texture);
		printOpenGLError();
		glFramebufferTexture2DEXT(GL_FRAMEBUFFER_EXT, GL_COLOR_ATTACHMENT0_EXT + _incre_textureBinding, GL_TEXTURE_2D, texture, 0);
		printOpenGLError();
		_incre_textureBinding++;
	}

	void FBO::endTextureBinding()
	{
		printOpenGLError();
		glActiveTexture(GL_TEXTURE0);
		GLenum buffers[] = { GL_COLOR_ATTACHMENT0_EXT,
			GL_COLOR_ATTACHMENT1_EXT,
			GL_COLOR_ATTACHMENT2_EXT,
			GL_COLOR_ATTACHMENT3_EXT,
			GL_COLOR_ATTACHMENT4_EXT,
			GL_COLOR_ATTACHMENT5_EXT,
			GL_COLOR_ATTACHMENT6_EXT,
			GL_COLOR_ATTACHMENT7_EXT,
			GL_COLOR_ATTACHMENT8_EXT};
		glDrawBuffers(_incre_textureBinding, buffers);

#if defined(_DEBUG) || defined(DEBUG)	

		if ( !CheckFramebufferStatus( false ) )
		{
			std::cout<<"FBO create failed"<<std::endl;
			return ;
		}

#endif
		printOpenGLError();
		_incre_textureBinding = 0;
	}


	void FBO::unbindTexture2D(unsigned int size)
	{
		for(unsigned int i = 0; i < size; ++i){
			glFramebufferTexture2DEXT( GL_FRAMEBUFFER_EXT, GL_COLOR_ATTACHMENT0_EXT + i, GL_TEXTURE_2D, 0, 0 );
		}
		glDisable(GL_TEXTURE_2D);
		this->unbind();
		printOpenGLError();
	}

	void FBO::bindTextureArraysLayer(const std::vector<GLuint>& textureArray, GLint level)
	{
		printOpenGLError();
		this->bind();
		printOpenGLError();
		GLenum buffers[] = {GL_COLOR_ATTACHMENT0_EXT,GL_COLOR_ATTACHMENT1_EXT,GL_COLOR_ATTACHMENT2_EXT, GL_COLOR_ATTACHMENT3_EXT , GL_COLOR_ATTACHMENT4_EXT,
			GL_COLOR_ATTACHMENT5_EXT, GL_COLOR_ATTACHMENT6_EXT, GL_COLOR_ATTACHMENT7_EXT};
		printOpenGLError();
		for(unsigned int i = 0; i < textureArray.size(); ++i){
			glBindTexture(GL_TEXTURE_2D_ARRAY_EXT,textureArray[i]);
			printOpenGLError();
			glFramebufferTextureLayerEXT(GL_FRAMEBUFFER_EXT, GL_COLOR_ATTACHMENT0_EXT+i, textureArray[i], 0, level );
			printOpenGLError();
		}

		printOpenGLError();
		glDrawBuffers(textureArray.size(),buffers);

#if defined(_DEBUG) || defined(DEBUG)	
		if ( !CheckFramebufferStatus( false ) )
		{
			std::cout<<"FBO create failed"<<std::endl;
			return ;
		}

#endif
		printOpenGLError();

	}

	void FBO::unbindTextureArrays()
	{
		printOpenGLError ();
		glBindTexture(GL_TEXTURE_2D_ARRAY_EXT,0);
		// glDisable(GL_TEXTURE_2D_ARRAY_EXT);
		printOpenGLError ();
		this->unbind();

		printOpenGLError();
	}



	//TODO custom mipmap
	void FBO::beginDrawToColorMipmapLevel(GLuint texture, GLuint attachementIndex, GLuint level)
	{
		printOpenGLError();
		glFramebufferTexture2DEXT(GL_FRAMEBUFFER_EXT,
			GL_COLOR_ATTACHMENT0_EXT + attachementIndex,
			GL_TEXTURE_2D, texture, level);
		printOpenGLError();
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_BASE_LEVEL, level-1);
		printOpenGLError();
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAX_LEVEL, level-1);
		printOpenGLError();
		/*glBindTexture( GL_TEXTURE_2D, texture->getID());
		glEnable(GL_TEXTURE_2D);*/
	}
	void FBO::endDrawingMipmapLevels(int maxLevel)
	{
		printOpenGLError();
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_BASE_LEVEL, 0);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAX_LEVEL, maxLevel);
		printOpenGLError();
	}

	/*
	void FBO::generateCustomMipMap(GLuint texture, int attachementIndex)
	{

	printOpenGLError();
	if(!texture->usingMipmap()) return;
	int max_level = 5;
	GLSLGpuProgram* gpuprogram = NULL; //= GpuProgramManager::getInstance()->getGLSLGpuProgramByName("MipMapGenerator");
	this->bind();
	this->bindTexture2DToColorBuffer(texture, 0);

	this->attachDepthBuffer(0);

	printOpenGLError ();
	texture->use();
	for(int level = 1; level < max_level; ++level)
	{
	beginDrawToColorMipmapLevel(texture, attachementIndex, level);
	printOpenGLError ();

	gpuprogram->setUniformVariable("level", (level - 1));
	GLint location = gpuprogram->getUniformLocation("mipmapTexture");
	printOpenGLError ();
	if(location != -1){
	glUniform1i(location,0);
	glActiveTexture(GL_TEXTURE0);
	texture->use();
	printOpenGLError ();
	}


	glPushAttrib(GL_VIEWPORT_BIT);
	int cut = pow(2.0, level);
	int size_w = texture->getWidth()/(cut);
	int size_h = texture->getHeight()/(cut);
	glViewport(0,0, size_w, size_h);
	printOpenGLError();
	glMatrixMode (GL_PROJECTION);
	glPushMatrix();
	glLoadIdentity ();
	glMatrixMode (GL_MODELVIEW);
	glPushMatrix();
	glLoadIdentity();

	printOpenGLError();
	glBegin(GL_QUADS);
	glTexCoord2f(0.0f, 0.0f); glVertex2f(-1.0f,-1.0f);
	glTexCoord2f(1.0f, 0.0f); glVertex2f(1.0f, -1.0f);
	glTexCoord2f(1.0f, 1.0f); glVertex2f(1.0f, 1.0f);
	glTexCoord2f(0.0f, 1.0f); glVertex2f(-1.0f, 1.0f);
	glEnd();

	printOpenGLError();
	glMatrixMode (GL_PROJECTION);
	glPopMatrix();
	printOpenGLError();
	glMatrixMode (GL_MODELVIEW);
	printOpenGLError();
	glPopMatrix();
	printOpenGLError();
	glPopAttrib();
	printOpenGLError();

	}
	endDrawingMipmapLevels();
	this->attachDepthBuffer(_depth_id);
	texture->unUse();
	this->unbind();

	}
	*/
	bool FBO::CheckFramebufferStatus(bool silent)
	{

		GLenum status = (GLenum)glCheckFramebufferStatusEXT(GL_FRAMEBUFFER_EXT);

		switch (status) {

		case GL_FRAMEBUFFER_COMPLETE_EXT:

			break;

		case GL_FRAMEBUFFER_UNSUPPORTED_EXT:
			if (!silent) std::cout<<"Unsupported framebuffer format"<<std::endl;
			assert( 0 );

			return false;

		case GL_FRAMEBUFFER_INCOMPLETE_MISSING_ATTACHMENT_EXT:

			if (!silent) std::cout<<"Framebuffer incomplete, missing attachment"<<std::endl;
			assert( 0 );

			return false;

		case GL_FRAMEBUFFER_INCOMPLETE_ATTACHMENT_EXT:

			if (!silent) std::cout<<"Framebuffer incomplete, duplicate attachment"<<std::endl;
			assert( 0 );

			return false;

		case GL_FRAMEBUFFER_INCOMPLETE_DIMENSIONS_EXT:

			if (!silent) std::cout<<"Framebuffer incomplete, attached images must have same dimensions"<<std::endl;
			assert( 0 );

			return false;

		case GL_FRAMEBUFFER_INCOMPLETE_FORMATS_EXT:

			if (!silent) std::cout<<"Framebuffer incomplete, attached images must have same format"<<std::endl;
			assert( 0 );

			return false;

		case GL_FRAMEBUFFER_INCOMPLETE_DRAW_BUFFER_EXT:

			if (!silent) std::cout<<"Framebuffer incomplete, missing draw buffer"<<std::endl;
			assert( 0 );

			return false;

		case GL_FRAMEBUFFER_INCOMPLETE_READ_BUFFER_EXT:

			if (!silent) std::cout<<"Framebuffer incomplete, missing read buffer"<<std::endl;
			assert( 0 );

			return false;

		case GL_FRAMEBUFFER_INCOMPLETE_MULTISAMPLE:

			if (!silent) std::cout<<"GL_FRAMEBUFFER_INCOMPLETE_MULTISAMPLE"<<std::endl;
			assert( 0 );

			return false;
		default:

			assert(0);

			return false;

		}

		return true;

	}



}
