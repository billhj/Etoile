/**
* Copyright(C) 2009-2012                
* @author Jing HUANG
* @file GLTextureCube.cpp
* @brief 
* @date 1/2/2011
*/

#include "GLTextureCube.h"

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
	GLTextureCube::GLTextureCube(const std::string& name): GLTexture(name)
	{

	}

	GLTextureCube::GLTextureCube(const std::string& name, GLuint id): GLTexture(name, id)
	{

	}
	/*
	bool GLTextureCube::loadTexture(const std::string& filepath, GLenum internalFormat, bool bCreateMipMap)
	{
	QImage buf,img;
	_mipmaped = bCreateMipMap;
	if(buf.load(filepath.c_str())){
	img = QGLWidget::convertToGLFormat( buf );

	glEnable(GL_TEXTURE_CUBE_MAP_EXT);
	glBindTexture(GL_TEXTURE_CUBE_MAP, _textureid);


	if ( bCreateMipMap ) {
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_NEAREST);
	printOpenGLError();

	}else{
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	printOpenGLError();

	}



	splitLoading(img);


	glDisable(GL_TEXTURE_CUBE_MAP_EXT);
	printOpenGLError();
	_layer = 6;
	printOpenGLError();
	//  texture = this->bindTexture( dexcurveimg, GL_TEXTURE_2D, GL_RGBA );
	return true;
	}else{
	std::cout<<"texture loader failed"<<std::endl;
	return false;
	}

	}
	*/
	bool GLTextureCube::create(  int iWidth, int iHeight, int iLayer, GLenum internalFormat, GLenum pixeldataformat, GLenum type, float* data, bool bCreateMipMap)
	{
		_width = iWidth;
		_height = iHeight;
		_layer = iLayer;
		_mipmaped = bCreateMipMap;
		_internalFormat = internalFormat;
		printOpenGLError();
		glBindTexture(GL_TEXTURE_CUBE_MAP, _id);
		printOpenGLError();

		if(bCreateMipMap){

			glTexParameteri(GL_TEXTURE_CUBE_MAP,GL_TEXTURE_MIN_FILTER,GL_LINEAR_MIPMAP_LINEAR);

			glTexParameteri(GL_TEXTURE_CUBE_MAP,GL_TEXTURE_MAG_FILTER,GL_LINEAR);

			glTexParameteri(GL_TEXTURE_CUBE_MAP,GL_TEXTURE_WRAP_S,GL_CLAMP_TO_EDGE);

			glTexParameteri(GL_TEXTURE_CUBE_MAP,GL_TEXTURE_WRAP_T,GL_CLAMP_TO_EDGE);
		}else{

			glTexParameteri(GL_TEXTURE_CUBE_MAP,GL_TEXTURE_MIN_FILTER,GL_LINEAR);

			glTexParameteri(GL_TEXTURE_CUBE_MAP,GL_TEXTURE_MAG_FILTER,GL_LINEAR);

			glTexParameteri(GL_TEXTURE_CUBE_MAP,GL_TEXTURE_WRAP_S,GL_CLAMP_TO_EDGE);

			glTexParameteri(GL_TEXTURE_CUBE_MAP,GL_TEXTURE_WRAP_T,GL_CLAMP_TO_EDGE);

		}

		printOpenGLError();
		glTexImage3D(GL_TEXTURE_CUBE_MAP,0, _internalFormat, _width, _height,_layer, 0, pixeldataformat, type, data);
		printOpenGLError();

		return true;
	}


	void GLTextureCube::setGLTextureParameter(GLenum pname, GLfloat param)
	{
		GLTexture::setGLTextureParameter(GL_TEXTURE_CUBE_MAP, pname, param);
	}
	/*
	bool GLTextureCube::splitLoading(QImage& img)
	{
	img = img.mirrored(false, true);
	//img.save("img.jpg");
	_height = img.height()/4.0;
	_width = img.width()/3.0;
	QPixmap qpmp;
	qpmp = QPixmap::fromImage(img);

	QPixmap top , bottom,  left, right, back , front;
	top = qpmp.copy(_width,0,_width,_height);
	bottom =  qpmp.copy(_width, _height*2 ,_width,_height);
	left = qpmp.copy(0, _height ,_width,_height);
	right =  qpmp.copy( _width * 2, _height ,_width,_height);
	front =  qpmp.copy( _width, _height,_width,_height);
	back = qpmp.copy(_width, _height*3,_width,_height);


	QImage topI , bottomI,  leftI, rightI, backI , frontI;
	topI = top.toImage();
	bottomI = bottom.toImage();
	leftI = left.toImage();
	rightI = right.toImage();
	backI = back.toImage().mirrored(true,true);
	frontI = front.toImage();

	glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X_EXT,
	0,                  //level
	GL_RGBA32F_ARB,            //internal format
	_width,                 //width
	_height,                 //height
	0,                  //border
	GL_RGBA,             //format
	GL_UNSIGNED_BYTE,   //type
	rightI.bits()); // pixel data

	glTexImage2D(GL_TEXTURE_CUBE_MAP_NEGATIVE_X_EXT, 0, GL_RGBA32F_ARB, _width, _height,  0, GL_RGBA,  GL_UNSIGNED_BYTE, leftI.bits());

	glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_Y_EXT, 0, GL_RGBA32F_ARB, _width, _height,  0, GL_RGBA,  GL_UNSIGNED_BYTE,topI.bits() );

	glTexImage2D(GL_TEXTURE_CUBE_MAP_NEGATIVE_Y_EXT, 0, GL_RGBA32F_ARB, _width, _height,  0, GL_RGBA,  GL_UNSIGNED_BYTE,bottomI.bits() );
	glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_Z_EXT, 0, GL_RGBA32F_ARB, _width, _height,  0, GL_RGBA,  GL_UNSIGNED_BYTE, frontI.bits());
	glTexImage2D(GL_TEXTURE_CUBE_MAP_NEGATIVE_Z_EXT, 0, GL_RGBA32F_ARB, _width, _height,  0, GL_RGBA,  GL_UNSIGNED_BYTE,  backI.bits());
	printOpenGLError();
	return true;

	}
	*/
	void GLTextureCube:: use() const
	{

		printOpenGLError();
		glEnable( GL_TEXTURE_CUBE_MAP );
		printOpenGLError();
		glBindTexture( GL_TEXTURE_CUBE_MAP, getId() );
		printOpenGLError();
	}

	void GLTextureCube:: unUse() const
	{
		printOpenGLError();
		glBindTexture(GL_TEXTURE_CUBE_MAP, 0);
		printOpenGLError();
		glDisable( GL_TEXTURE_CUBE_MAP);
		printOpenGLError();
	}

	void GLTextureCube::generateMipmap() const
	{
		if(_mipmaped)
		{
			glEnable(GL_TEXTURE_CUBE_MAP);
			printOpenGLError();
			glActiveTexture(GL_TEXTURE0);
			printOpenGLError();
			glBindTexture(GL_TEXTURE_CUBE_MAP, _id);
			printOpenGLError();
			glGenerateMipmap(GL_TEXTURE_CUBE_MAP);
			printOpenGLError();
		}
	}
}
