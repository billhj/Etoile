/**
* Copyright(C) 2009-2012                
* @author Jing HUANG
* @file QtTextureLoader.h
* @brief 
* @date 1/2/2011
*/

#pragma once
#include "renderer/OpenGL/GLTexture2D.h"
#include "geometry/meshloader/TextureLoader.h"
#include "util/ImageLoader/ReadImage.h"

namespace Etoile
{
	

	enum EmptyMap{
		WRITE,
		CHECKBOARD,
	};

	class QtTextureLoader : public TextureLoader
	{
	protected:
		Texture* empty;
		GLTexture2D* whiteMap;
		GLTexture2D* checkBoard;
		
	public:
		QtTextureLoader()
		{
			float emptyMap[16] = {1,1,1,1, 1,1,1,1, 1,1,1,1, 1,1,1,1};
			float checkboard[64] = {0,0,0,0, 1,1,1,1, 0,0,0,0, 1,1,1,1, 
			1,1,1,1,0,0,0,0, 1,1,1,1,0,0,0,0,  
			0,0,0,0, 1,1,1,1, 0,0,0,0, 1,1,1,1, 
			1,1,1,1,0,0,0,0, 1,1,1,1,0,0,0,0};

			whiteMap = new GLTexture2D("whiteMap");
			whiteMap->create(2, 2, 1 , GL_RGBA32F_ARB, GL_RGBA, GL_FLOAT , &emptyMap[0], false);
			checkBoard = new GLTexture2D("checkBoardMap");
			checkBoard->create(4, 4, 1 , GL_RGBA32F_ARB, GL_RGBA, GL_FLOAT , &checkboard[0], false);
			empty = checkBoard;
		}

		~QtTextureLoader(){}
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

		Texture* useEmptyMapType(EmptyMap type)
		{
			if(type == EmptyMap::WRITE)
			{
				empty = whiteMap;
			}
			else if(type == EmptyMap::CHECKBOARD)
			{
				empty = checkBoard;
			}
			return empty;
		}
	
	};

}
