/**
* Copyright(C) 2009-2012                
* @author Jing HUANG
* @file glfunctions.h
* @brief 
* @date 1/2/2011
*/

#pragma once
#include "glhead.h"

namespace Etoile
{


	size_t GetInternalTypeSize( GLenum eType );

	int GetPixelSize(const GLenum internalFormat) ;

	GLenum GetTextureInternalFormat(const std::string& internalFormat);

	GLenum GetTexturePixelDataFormat(const std::string& format);

	GLenum GetTexturePixelDataType(const std::string& type);




	void drawCylinder(float x1, float y1, float z1, float x2, float y2, float z2, float radius,int subdivisions,GLUquadricObj *quadric);
	void drawCylinder_convenient(float x1, float y1, float z1, float x2,float y2, float z2, float radius,int subdivisions);

	void drawSphere(float x1, float y1, float z1, float radius, int slices, int stacks, GLUquadricObj *quadric);
	void drawSphere_convenient(float x1, float y1, float z1, float radius, int slices, int stacks);

}


