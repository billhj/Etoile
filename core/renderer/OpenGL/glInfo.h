/**
* Copyright(C) 2009-2012                
* @author Jing HUANG
* @file glInfo.h
* @brief 
* @date 1/2/2011
*/

#pragma once
#include "glhead.h"
#include <string>
#include <vector>

// struct variable to store OpenGL info

namespace Etoile
{

	class glInfo
	{
		std::string vendor;
		std::string renderer;
		std::string version;
		std::string glewversion;
		std::vector <std::string> extensions;
		bool _isInitGlew;
		bool _isInitGlut;
		int redBits;
		int greenBits;
		int blueBits;
		int alphaBits;
		int depthBits;
		int stencilBits;
		int maxTextureSize;
		int maxLights;
		int maxAttribStacks;
		int maxModelViewStacks;
		int maxProjectionStacks;
		int maxClipPlanes;
		int maxTextureStacks;

		// ctor, init all members


	private:
		glInfo() : redBits(0), greenBits(0), blueBits(0), alphaBits(0), depthBits(0),
			stencilBits(0), maxTextureSize(0), maxLights(0), maxAttribStacks(0),
			maxModelViewStacks(0), maxClipPlanes(0), maxTextureStacks(0) { _isInitGlew = false; _isInitGlut = false;}


	public:
		static glInfo* getInstance()
		{
			static glInfo _instance;
			return &_instance;
		}

		void initAllOpenGL();
		//needs to be init after GLEW
		void initOpenGLGeneral(bool output = true);
		void initGLEW(bool output = true);
		void initGLUT(bool output = true);

		bool getOpenGLInfo();                             // extract info
		void printOpenGLInfo();                           // print itself
		bool isOpenGLExtensionSupported(const std::string& ext); // check if a extension is supported
	};
}

