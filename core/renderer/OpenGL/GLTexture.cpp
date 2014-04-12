/**
* Copyright(C) 2009-2012                
* @author Jing HUANG
* @file GLTexture.cpp
* @brief 
* @date 1/2/2011
*/

#include "GLTexture.h"
#include <fstream>
#include <sstream>

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
	GLTexture::GLTexture(const std::string& name) : Texture(name)
	{
		printOpenGLError();
		glGenTextures(1, &_id);
		printOpenGLError();
		_selfGenerated = true;
	}

	GLTexture::GLTexture(const std::string& name, GLuint id) : Texture(name), _id(id)
	{
		_selfGenerated = false;
	}

	GLTexture::~GLTexture()
	{
		release();
	}

	void GLTexture::draw(int w, int h)
	{
		use();
		glPushAttrib(GL_VIEWPORT_BIT);
		glViewport(0,0, w, h);
		glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
		glMatrixMode (GL_PROJECTION);
		glPushMatrix();
		glLoadIdentity ();
		glMatrixMode (GL_MODELVIEW);
		glPushMatrix();
		glLoadIdentity();

		glBegin(GL_QUADS);
		glTexCoord2f(0.0f, 0.0f); glVertex2f(-1.0f, -1.0f);
		glTexCoord2f(1.0f, 0.0f); glVertex2f( 1.0f, -1.0f);
		glTexCoord2f(1.0f, 1.0f); glVertex2f( 1.0f,  1.0f);
		glTexCoord2f(0.0f, 1.0f); glVertex2f(-1.0f,  1.0f);
		glEnd();

		glMatrixMode (GL_PROJECTION);
		glPopMatrix();
		glMatrixMode (GL_MODELVIEW);
		glPopMatrix();
		glPopAttrib();
		unUse();
		printOpenGLError();
	}

	void GLTexture::draw()
	{
		use();
		glPushAttrib(GL_VIEWPORT_BIT);
		glViewport(0,0, _width, _height);
		glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
		glMatrixMode (GL_PROJECTION);
		glPushMatrix();
		glLoadIdentity ();
		glMatrixMode (GL_MODELVIEW);
		glPushMatrix();
		glLoadIdentity();

		glBegin(GL_QUADS);
		glTexCoord2f(0.0f, 0.0f); glVertex2f(-1.0f, -1.0f);
		glTexCoord2f(1.0f, 0.0f); glVertex2f( 1.0f, -1.0f);
		glTexCoord2f(1.0f, 1.0f); glVertex2f( 1.0f,  1.0f);
		glTexCoord2f(0.0f, 1.0f); glVertex2f(-1.0f,  1.0f);
		glEnd();

		glMatrixMode (GL_PROJECTION);
		glPopMatrix();
		glMatrixMode (GL_MODELVIEW);
		glPopMatrix();
		glPopAttrib();
		unUse();
		printOpenGLError();
	}

	void GLTexture::release()
	{
		printOpenGLError();
		if(_selfGenerated)
			glDeleteTextures(1, &_id );
		printOpenGLError();
	}

	void GLTexture::setGLTextureParameter(GLenum target, GLenum pname, GLfloat param)
	{
		use();
		glTexParameterf(target, pname, param);
		unUse();
	}

	int GLTexture::getPixelDataFormat(const GLenum internalFormat) {

		switch(internalFormat) {

		case GL_ALPHA:
			return 1;
		case GL_ALPHA8:
			return 1;
		case GL_ALPHA16:
			return 2;
		case GL_ALPHA16F_ARB:
			return 2;
		case GL_ALPHA32F_ARB:
			return 4;
		case GL_ALPHA16I_EXT:
			return 2;
		case GL_ALPHA16UI_EXT:
			return 4;
		case GL_ALPHA32I_EXT:
			return 4;
		case GL_ALPHA32UI_EXT:
			return 4;

		case GL_LUMINANCE:
			return 1;
		case GL_LUMINANCE8:
			return 1;
		case GL_LUMINANCE16:
			return 2;
		case GL_LUMINANCE16F_ARB:
			return 2;
		case GL_LUMINANCE32F_ARB:
			return 4;
		case GL_LUMINANCE16I_EXT:
			return 2;
		case GL_LUMINANCE16UI_EXT:
			return 2;
		case GL_LUMINANCE32I_EXT:
			return 4;
		case GL_LUMINANCE32UI_EXT:
			return 4;

		case GL_DEPTH_COMPONENT16 :
			return 2;
		case GL_DEPTH_COMPONENT24 :
			return 3;
		case GL_DEPTH_COMPONENT32 :
			return 4;

		case GL_LUMINANCE_ALPHA:
			return 2;
		case GL_LUMINANCE8_ALPHA8:
			return 2;
		case GL_LUMINANCE16_ALPHA16:
			return 4;
		case GL_LUMINANCE_ALPHA16F_ARB:
			return 4;
		case GL_LUMINANCE_ALPHA32F_ARB:
			return 8;
		case GL_LUMINANCE_ALPHA16I_EXT:
			return 4;
		case GL_LUMINANCE_ALPHA16UI_EXT:
			return 4;
		case GL_LUMINANCE_ALPHA32I_EXT:
			return 8;
		case GL_LUMINANCE_ALPHA32UI_EXT:
			return 8;

		case GL_RGB:
			return GL_RGB;
		case GL_RGB8:
			return GL_RGB;
		case GL_RGB16F_ARB:
			return GL_RGB;
		case GL_RGB32F_ARB:
			return GL_RGB;
		case GL_RGB16I_EXT:
			return GL_RGB;
		case GL_RGB16UI_EXT:
			return GL_RGB;
		case GL_RGB32I_EXT:
			return GL_RGB;
		case GL_RGB32UI_EXT:
			return GL_RGB;

		case GL_RGBA:
			return GL_RGBA;
		case GL_RGBA8:
			return GL_RGBA;
		case GL_RGBA16F_ARB:
			return GL_RGBA;
		case GL_RGBA32F_ARB:
			return GL_RGBA;
		case GL_RGBA16I_EXT:
			return GL_RGBA;
		case GL_RGBA16UI_EXT:
			return GL_RGBA;
		case GL_RGBA32I_EXT:
			return GL_RGBA;
		case GL_RGBA32UI_EXT:
			return GL_RGBA;
			break;
		default:
			std::cout<<"internal Format Unknow Format"<< internalFormat<<std::endl;
			return 0;
		}
	}





	GLenum GLTexture::getInternalFormat(const std::string& internalFormat) {

		if(internalFormat.compare("GL_ALPHA")==0)
			return GL_ALPHA;

		else if(internalFormat.compare("GL_ALPHA8")==0)
			return GL_ALPHA8;

		else if(internalFormat.compare("GL_ALPHA16")==0)
			return GL_ALPHA16;

		else if(internalFormat.compare("GL_ALPHA16F_ARB")==0)
			return GL_ALPHA16F_ARB;

		else if(internalFormat.compare("GL_ALPHA32F_ARB")==0)
			return GL_ALPHA32F_ARB;

		else if(internalFormat.compare("GL_ALPHA16I_EXT")==0)
			return GL_ALPHA16I_EXT;

		else if(internalFormat.compare("GL_ALPHA16UI_EXT")==0)
			return GL_ALPHA16UI_EXT;

		else if(internalFormat.compare("GL_ALPHA32I_EXT")==0)
			return GL_ALPHA32I_EXT;

		else if(internalFormat.compare("GL_ALPHA32UI_EXT")==0)
			return GL_ALPHA32UI_EXT;

		else if(internalFormat.compare("GL_LUMINANCE")==0)
			return GL_LUMINANCE;

		else if(internalFormat.compare("GL_LUMINANCE8")==0)
			return GL_LUMINANCE8;

		else if(internalFormat.compare("GL_LUMINANCE16")==0)
			return GL_LUMINANCE16;

		else if(internalFormat.compare("GL_LUMINANCE16F_ARB")==0)
			return GL_LUMINANCE16F_ARB;

		else if(internalFormat.compare("GL_LUMINANCE32F_ARB")==0)
			return GL_LUMINANCE32F_ARB;

		else if(internalFormat.compare("GL_LUMINANCE16I_EXT")==0)
			return GL_LUMINANCE16I_EXT;

		else if(internalFormat.compare("GL_LUMINANCE16UI_EXT")==0)
			return GL_LUMINANCE16UI_EXT;


		else if(internalFormat.compare("GL_LUMINANCE32I_EXT")==0)
			return GL_LUMINANCE32I_EXT;

		else if(internalFormat.compare("GL_LUMINANCE32UI_EXT")==0)
			return GL_LUMINANCE32UI_EXT;

		else if(internalFormat.compare("GL_DEPTH_COMPONENT16")==0)
			return GL_DEPTH_COMPONENT16;

		else if(internalFormat.compare("GL_DEPTH_COMPONENT24")==0)
			return GL_DEPTH_COMPONENT24;

		else if(internalFormat.compare("GL_DEPTH_COMPONENT32")==0)
			return GL_DEPTH_COMPONENT32;

		else if(internalFormat.compare("GL_LUMINANCE_ALPHA")==0)
			return GL_LUMINANCE_ALPHA;


		else if(internalFormat.compare("GL_LUMINANCE8_ALPHA8")==0)
			return GL_LUMINANCE8_ALPHA8;

		else if(internalFormat.compare("GL_LUMINANCE16_ALPHA16")==0)
			return GL_LUMINANCE16_ALPHA16;

		else if(internalFormat.compare("GL_LUMINANCE_ALPHA16F_ARB")==0)
			return GL_LUMINANCE_ALPHA16F_ARB;

		else if(internalFormat.compare("GL_LUMINANCE_ALPHA32F_ARB")==0)
			return GL_LUMINANCE_ALPHA32F_ARB;

		else if(internalFormat.compare("GL_LUMINANCE_ALPHA16I_EXT")==0)
			return GL_LUMINANCE_ALPHA16I_EXT;

		else if(internalFormat.compare("GL_LUMINANCE_ALPHA16UI_EXT")==0)
			return GL_LUMINANCE_ALPHA16UI_EXT;

		else if(internalFormat.compare("GL_LUMINANCE_ALPHA32I_EXT")==0)
			return GL_LUMINANCE_ALPHA32I_EXT;

		else if(internalFormat.compare("GL_LUMINANCE_ALPHA32UI_EXT")==0)
			return GL_LUMINANCE_ALPHA32UI_EXT;

		else if(internalFormat.compare("GL_RGB")==0)
			return GL_RGB;

		else if(internalFormat.compare("GL_RGB8")==0)
			return GL_RGB8;

		else if(internalFormat.compare("GL_RGB16F_ARB")==0)
			return GL_RGB16F_ARB;

		else if(internalFormat.compare("GL_RGB32F_ARB")==0)
			return GL_RGB32F_ARB;

		else if(internalFormat.compare("GL_RGB16I_EXT")==0)
			return GL_RGB16I_EXT;

		else if(internalFormat.compare("GL_RGB16UI_EXT")==0)
			return GL_RGB16UI_EXT;

		else if(internalFormat.compare("GL_RGB32I_EXT")==0)
			return GL_RGB32I_EXT;

		else if(internalFormat.compare("GL_RGB32UI_EXT")==0)
			return GL_RGB32UI_EXT;

		else if(internalFormat.compare("GL_RGBA")==0)
			return GL_RGBA;

		else if(internalFormat.compare("GL_RGBA8")==0)
			return GL_RGBA8;

		else if(internalFormat.compare("GL_RGBA16F_ARB")==0)
			return GL_RGBA16F_ARB;

		else if(internalFormat.compare("GL_RGBA32F_ARB")==0)
			return GL_RGBA32F_ARB;

		else if(internalFormat.compare("GL_RGBA16I_EXT")==0)
			return GL_RGBA16I_EXT;

		else if(internalFormat.compare("GL_RGBA16UI_EXT")==0)
			return GL_RGBA16UI_EXT;

		else if(internalFormat.compare("GL_RGBA32I_EXT")==0)
			return GL_RGBA32I_EXT;

		else if(internalFormat.compare("GL_RGBA32UI_EXT")==0)
			return GL_RGBA32UI_EXT;


		else{
			std::cout<<"Unknow internal Format "<< internalFormat<<std::endl;
			return 0;
		}

	}

}
