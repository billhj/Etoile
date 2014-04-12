/**
* Copyright(C) 2009-2012                
* @author Jing HUANG
* @file QTTextureLoader.h
* @brief 
* @date 1/2/2011
*/

#pragma once
#include "renderer/OpenGL/GLTexture2D.h"
#include "geometry/meshloader/TextureLoader.h"
#include "util/ImageLoader/ReadImage.h"

namespace Etoile
{
	class QTTextureLoader : public TextureLoader
	{
	protected:
		Texture* empty;
	public:
		QTTextureLoader()
		{
			float emptyMap[16] = {1,1,1,1, 1,1,1,1, 1,1,1,1, 1,1,1,1};
			float checkboard[64] = {0,0,0,0, 1,1,1,1, 0,0,0,0, 1,1,1,1, 
			1,1,1,1,0,0,0,0, 1,1,1,1,0,0,0,0,  
			0,0,0,0, 1,1,1,1, 0,0,0,0, 1,1,1,1, 
			1,1,1,1,0,0,0,0, 1,1,1,1,0,0,0,0};
			GLTexture2D* t = new GLTexture2D("emptyMap");
			t->create(2, 2, 1 , GL_RGBA32F_ARB, GL_RGBA, GL_FLOAT , &emptyMap[0], false);
			GLTexture2D* t2 = new GLTexture2D("checkBoardMap");
			t2->create(4, 4, 1 , GL_RGBA32F_ARB, GL_RGBA, GL_FLOAT , &checkboard[0], false);
			empty = t2;
		}

		~QTTextureLoader(){}
		virtual Texture* loadFromFile(const std::string& fileName) override
		{
			Image image;
			bool b_image = ReadImage::loadImageFromFile(fileName, image);
			if(b_image != true)
			{
				std::cout<<"can not load texture : "<<fileName<<std::endl;
				//assert(!qimage.isNull());
				return empty;
			}
			else
			{
				GLTexture2D* t = new GLTexture2D(fileName);
				t->create(image.getWidth(), image.getHeight(),1 , GL_RGBA32F_ARB, GL_RGBA, GL_FLOAT ,(float*)&image.getData()[0], false);
				return t;
			}
		}
	
	};

}
