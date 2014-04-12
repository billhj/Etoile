/**
* Copyright(C) 2009-2012                
* @author Jing HUANG
* @file glInfo.cpp
* @brief 
* @date 1/2/2011
*/

#include "glInfo.h"
#include <iostream>
#include <sstream>
#include <iomanip>
#include <algorithm>
#include <cstring>
#include <assert.h>

namespace Etoile
{

	void glInfo::initAllOpenGL()
	{
		if(!_isInitGlew)
		{
			initGLEW();
		}
		if(!_isInitGlut)
		{
			initGLUT();
		}
		// initOpenGLGeneral(true);
	}

	void glInfo::initOpenGLGeneral(bool output)
	{
		getOpenGLInfo();
		if(output)
			printOpenGLInfo();
	}

	void glInfo::initGLUT(bool output)
	{
		_isInitGlut = false;
		int argc = 0;
		char* argv = 0;


#if defined( _MSC_VER )

#else
		glutInit(&argc,&argv);
#endif // 
		if(output)
			std::cout<<"glut init"<<std::endl;
	}


	void glInfo::initGLEW(bool output)
	{
		_isInitGlew = true;
		GLenum errorEnum = glewInit();

		if(!output) return;

		std::cout<<"glew init"<<std::endl;
		const char* str = (char*)glewGetString(GLEW_VERSION);
		if(str) this->glewversion = str;                 // check NULL return value

		if ( GLEW_OK != errorEnum ) {
			std::cout<< "Error Initializing Glew: " << glewGetErrorString( errorEnum ) << std::endl;
			assert( 0 );
			exit( 1 );
			return;
		}
		printOpenGLError();
		if (glewIsSupported("GL_VERSION_2_0")){
			std::cout<<"Ready for OpenGL 2.0"<<std::endl;
			std::cout<<"OpenGLEW version :"<<str<<std::endl;
		}else {
			std::cout<<"OpenGL 2.0 not supported\n"<<std::endl;

		}
		printOpenGLError();
		//	const GLubyte* str = glGetString( GL_EXTENSIONS );

		if (glewGetExtension ("GL_ARB_vertex_shader")        != GL_TRUE ||
			glewGetExtension ("GL_ARB_shader_objects")       != GL_TRUE /*||
																		glewGetExtension ("GL_ARB_shading_language_100") != GL_TRUE*/) {
																			std::cout << "Driver does not support OpenGL Shading Language" << std::endl;
																			exit (EXIT_FAILURE);
		}else{
			std::cout << "Driver supports OpenGL Shading Language" << std::endl;
			std::cout<<"Shading program version :"<<glGetString(GL_SHADING_LANGUAGE_VERSION)<<std::endl;
		}
		if (glewGetExtension ("GL_ARB_vertex_buffer_object") != GL_TRUE) {
			std::cout << "Driver does not support Vertex Buffer Objects" <<std::endl;
			exit (EXIT_FAILURE);
		}else{
			std::cout << "Driver supports Vertex Buffer Objects" <<std::endl;
		}


		if (glewGetExtension("GL_NVX_gpu_memory_info")){
			std::cout<<"Ready for GL_NVX_gpu_memory_info"<<std::endl;
		}else {
			std::cout<<"GL_NVX_gpu_memory_info not supported\n"<<std::endl;

		}
	}

	///////////////////////////////////////////////////////////////////////////////
	// extract openGL info
	// This function must be called after GL rendering context opened.
	///////////////////////////////////////////////////////////////////////////////
	bool glInfo::getOpenGLInfo()
	{
		char* str = 0;
		std::string tok;

		// get vendor string
		str = (char*)glGetString(GL_VENDOR);
		if(str) this->vendor = str;                  // check NULL return value
		else return false;

		// get renderer string
		str = (char*)glGetString(GL_RENDERER);
		if(str) this->renderer = str;                // check NULL return value
		else return false;

		// get version string
		str = (char*)glGetString(GL_VERSION);
		if(str) this->version = str;                 // check NULL return value
		else return false;



		//  so wired glGetString(GL_EXTENSIONS); antialising pb
		// get all extensions as a string
		str = (char*)glGetString(GL_EXTENSIONS);

		// split extensions
		if(str)
		{
			std::stringstream stream(str);
			while(stream.eof())
			{
				stream >> tok;
				this->extensions.push_back(tok);    // put a extension into struct
			}
		}
		else
		{
			return false;
		}

		// sort extension by alphabetical order
		std::sort(this->extensions.begin(), this->extensions.end());

		// get number of color bits
		glGetIntegerv(GL_RED_BITS, &this->redBits);
		glGetIntegerv(GL_GREEN_BITS, &this->greenBits);
		glGetIntegerv(GL_BLUE_BITS, &this->blueBits);
		glGetIntegerv(GL_ALPHA_BITS, &this->alphaBits);

		// get depth bits
		glGetIntegerv(GL_DEPTH_BITS, &this->depthBits);

		// get stecil bits
		glGetIntegerv(GL_STENCIL_BITS, &this->stencilBits);

		// get max number of lights allowed
		glGetIntegerv(GL_MAX_LIGHTS, &this->maxLights);

		// get max texture resolution
		glGetIntegerv(GL_MAX_TEXTURE_SIZE, &this->maxTextureSize);

		// get max number of clipping planes
		glGetIntegerv(GL_MAX_CLIP_PLANES, &this->maxClipPlanes);

		// get max modelview and projection matrix stacks
		glGetIntegerv(GL_MAX_MODELVIEW_STACK_DEPTH, &this->maxModelViewStacks);
		glGetIntegerv(GL_MAX_PROJECTION_STACK_DEPTH, &this->maxProjectionStacks);
		glGetIntegerv(GL_MAX_ATTRIB_STACK_DEPTH, &this->maxAttribStacks);

		// get max texture stacks
		glGetIntegerv(GL_MAX_TEXTURE_STACK_DEPTH, &this->maxTextureStacks);

		return true;
	}



	///////////////////////////////////////////////////////////////////////////////
	// check if the video card support a certain extension
	///////////////////////////////////////////////////////////////////////////////
	bool glInfo::isOpenGLExtensionSupported(const std::string& ext)
	{
		// search corresponding extension
		std::vector<std::string>::const_iterator iter = this->extensions.begin();
		std::vector<std::string>::const_iterator endIter = this->extensions.end();

		while(iter != endIter)
		{
			if(ext == *iter)
				return true;
			else
				++iter;
		}
		return false;
	}



	///////////////////////////////////////////////////////////////////////////////
	// print OpenGL info to screen and save to a file
	///////////////////////////////////////////////////////////////////////////////
	void glInfo::printOpenGLInfo()
	{
		std::stringstream ss;

		ss << std::endl; // blank line
		ss << "OpenGL Driver Info" << std::endl;
		ss << "==================" << std::endl;
		ss << "Vendor: " << this->vendor << std::endl;
		ss << "Version: " << this->version << std::endl;
		ss << "Renderer: " << this->renderer << std::endl;

		ss << std::endl;
		ss << "Color Bits(R,G,B,A): (" << this->redBits << ", " << this->greenBits
			<< ", " << this->blueBits << ", " << this->alphaBits << ")\n";
		ss << "Depth Bits: " << this->depthBits << std::endl;
		ss << "Stencil Bits: " << this->stencilBits << std::endl;

		ss << std::endl;
		ss << "Max Texture Size: " << this->maxTextureSize << "x" << this->maxTextureSize << std::endl;
		ss << "Max Lights: " << this->maxLights << std::endl;
		ss << "Max Clip Planes: " << this->maxClipPlanes << std::endl;
		ss << "Max Modelview Matrix Stacks: " << this->maxModelViewStacks << std::endl;
		ss << "Max Projection Matrix Stacks: " << this->maxProjectionStacks << std::endl;
		ss << "Max Attribute Stacks: " << this->maxAttribStacks << std::endl;
		ss << "Max Texture Stacks: " << this->maxTextureStacks << std::endl;

		ss << std::endl;
		ss << "Total Number of Extensions: " << this->extensions.size() << std::endl;
		ss << "==============================" << std::endl;
		for(unsigned int i = 0; i < this->extensions.size(); ++i)
			ss << this->extensions.at(i) << std::endl;

		ss << "======================================================================" << std::endl;

		std::cout << ss.str() << std::endl;
	}
}