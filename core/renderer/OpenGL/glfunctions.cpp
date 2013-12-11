/**
* Copyright(C) 2009-2012                
* @author Jing HUANG
* @file glfunctions.cpp
* @brief 
* @date 1/2/2011
*/

#include "glfunctions.h"
#include <assert.h>
#include <string>
#include <cmath>

namespace Etoile
{


	size_t GetInternalTypeSize( GLenum eType )
	{
		switch ( eType ) 
		{
		case GL_FLOAT:
			return sizeof( float );
			break;
		case GL_UNSIGNED_INT:
			return sizeof( unsigned int );					// Test this more thoroughly for 64
			break;
		case GL_SHORT:
			return sizeof( short );
			break;
		default:
			assert( 0 );
			return 0;
		}
	}


	int GetPixelSize(const GLenum internalFormat) {

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
			return 3;
		case GL_RGB8:
			return 3;
		case GL_RGB16F_ARB:
			return 6;
		case GL_RGB32F_ARB:
			return 12;
		case GL_RGB16I_EXT:
			return 6;
		case GL_RGB16UI_EXT:
			return 6;
		case GL_RGB32I_EXT:
			return 12;
		case GL_RGB32UI_EXT:
			return 12;

		case GL_RGBA:
			return 4;
		case GL_RGBA8:
			return 4;
		case GL_RGBA16F_ARB:
			return 8;
		case GL_RGBA32F_ARB:
			return 16;
		case GL_RGBA16I_EXT:
			return 8;
		case GL_RGBA16UI_EXT:
			return 8;
		case GL_RGBA32I_EXT:
			return 16;
		case GL_RGBA32UI_EXT:
			return 16;

		default:
			std::cout<<"Unknow internal Format"<< internalFormat<<std::endl;
			return 0;
		}
	}



	GLenum GetTextureInternalFormat(const std::string& internalFormat) {

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
		else if(internalFormat.compare("GL_R32UI")==0)
			return GL_R32UI;

		else{
			std::cout<<"Unknow internal Format "<<internalFormat<<std::endl;
			return 0;
		}

	}

	GLenum GetTexturePixelDataFormat(const std::string& format)
	{
		if(format.compare("GL_COLOR_INDEX")==0)
			return GL_COLOR_INDEX;

		else if(format.compare("GL_RED")==0)
			return GL_RED;

		else if(format.compare("GL_GREEN")==0)
			return GL_GREEN;

		else if(format.compare("GL_BLUE")==0)
			return GL_BLUE;

		else if(format.compare("GL_ALPHA")==0)
			return GL_ALPHA;

		else if(format.compare("GL_RGB")==0)
			return GL_RGB;

		else if(format.compare("GL_BGR")==0)
			return GL_BGR;

		else if(format.compare("GL_RGBA")==0)
			return GL_RGBA;

		else if(format.compare("GL_BGRA")==0)
			return GL_BGRA;

		else if(format.compare("GL_LUMINANCE")==0)
			return GL_LUMINANCE;

		else if(format.compare("GL_LUMINANCE_ALPHA")==0)
			return GL_LUMINANCE_ALPHA;
		else if(format.compare("GL_RED_INTEGER")==0)
			return GL_RED_INTEGER;
		else
		{
			assert(0 &&" no GetTexturePixelDataFormat");
			return 0;
		}
	}



	GLenum GetTexturePixelDataType(const std::string& type)
	{
		if(type.compare("GL_UNSIGNED_BYTE")==0)
			return GL_UNSIGNED_BYTE;

		else if(type.compare("GL_BYTE")==0)
			return GL_BYTE;

		else if(type.compare("GL_BITMAP")==0)
			return GL_BITMAP;

		else if(type.compare("GL_UNSIGNED_SHORT")==0)
			return GL_UNSIGNED_SHORT;

		else if(type.compare("GL_SHORT")==0)
			return GL_SHORT;

		else if(type.compare("GL_UNSIGNED_INT")==0)
			return GL_UNSIGNED_INT;

		else if(type.compare("GL_FLOAT")==0)
			return GL_FLOAT;

		else if(type.compare("GL_UNSIGNED_BYTE_3_3_2")==0)
			return GL_UNSIGNED_BYTE_3_3_2;

		else if(type.compare("GL_UNSIGNED_BYTE_2_3_3_REV")==0)
			return GL_UNSIGNED_BYTE_2_3_3_REV;

		else if(type.compare("GL_UNSIGNED_SHORT_5_6_5")==0)
			return GL_UNSIGNED_SHORT_5_6_5;

		else if(type.compare("GL_UNSIGNED_SHORT_5_6_5_REV")==0)
			return GL_UNSIGNED_SHORT_5_6_5_REV;

		else if(type.compare("GL_UNSIGNED_SHORT_4_4_4_4")==0)
			return GL_UNSIGNED_SHORT_4_4_4_4;

		else if(type.compare("GL_UNSIGNED_SHORT_4_4_4_4_REV")==0)
			return GL_UNSIGNED_SHORT_4_4_4_4_REV;

		else if(type.compare("GL_UNSIGNED_SHORT_5_5_5_1")==0)
			return GL_UNSIGNED_SHORT_5_5_5_1;

		else if(type.compare("GL_UNSIGNED_SHORT_1_5_5_5_REV")==0)
			return GL_UNSIGNED_SHORT_1_5_5_5_REV;

		else if(type.compare("GL_UNSIGNED_INT_8_8_8_8")==0)
			return GL_UNSIGNED_INT_8_8_8_8;

		else if(type.compare("GL_UNSIGNED_INT_8_8_8_8_REV")==0)
			return GL_UNSIGNED_INT_8_8_8_8_REV;

		else if(type.compare("GL_UNSIGNED_INT_10_10_10_2")==0)
			return GL_UNSIGNED_INT_10_10_10_2;

		else if(type.compare("GL_UNSIGNED_INT_2_10_10_10_REV")==0)
			return GL_UNSIGNED_INT_2_10_10_10_REV;
		else
		{
			assert(0 &&" no GetTexturePixelDataType");
			return 0;
		}
	}




	void drawCylinder(float x1, float y1, float z1, float x2,float y2, float z2, float radius,int subdivisions,GLUquadricObj *quadric)
	{
		float vx = x2-x1;
		float vy = y2-y1;
		float vz = z2-z1;

		//handle the degenerate case of z1 == z2 with an approximation
		if(vz == 0)
			vz = .0001;

		float v = sqrt( vx*vx + vy*vy + vz*vz );
		float ax = 57.2957795*acos( vz/v );
		if ( vz < 0.0 )
			ax = -ax;
		float rx = -vy*vz;
		float ry = vx*vz;
		glPushMatrix();

		//draw the cylinder body
		glTranslated( x1,y1,z1 );
		glRotated(ax, rx, ry, 0.0);
		gluQuadricOrientation(quadric,GLU_OUTSIDE);
		gluCylinder(quadric, radius, radius, v, subdivisions, 1);

		//draw the first cap
		gluQuadricOrientation(quadric,GLU_INSIDE);
		gluDisk( quadric, 0.0, radius, subdivisions, 1);
		glTranslated( 0,0,v );

		//draw the second cap
		gluQuadricOrientation(quadric,GLU_OUTSIDE);
		gluDisk( quadric, 0.0, radius, subdivisions, 1);
		glPopMatrix();
	}

	void drawCylinder_convenient(float x1, float y1, float z1, float x2,float y2, float z2, float radius,int subdivisions)
	{
		//the same quadric can be re-used for drawing many cylinders
		GLUquadricObj *quadric= gluNewQuadric();
		gluQuadricNormals(quadric, GLU_SMOOTH);
		gluQuadricTexture (quadric, GL_TRUE);
		drawCylinder(x1,y1,z1,x2,y2,z2,radius,subdivisions,quadric);
		gluDeleteQuadric(quadric);
	}

	void drawSphere(float x1, float y1, float z1, float radius, int slices, int stacks, GLUquadricObj *quadric)
	{
		glPushMatrix();
		glTranslated(x1, y1, z1);
		//drawAxis(); // show axis
		gluSphere(quadric, radius ,slices, stacks);
		glPopMatrix();
	}

	void drawSphere_convenient(float x1, float y1, float z1, float radius, int slices, int stacks)
	{
		GLUquadricObj *quadric= gluNewQuadric();
		gluQuadricNormals(quadric, GLU_SMOOTH);
		gluQuadricTexture (quadric, GL_TRUE);
		drawSphere(x1,y1,z1,radius,slices, stacks,quadric);
		gluDeleteQuadric(quadric);
	}

}
