/**
* Copyright(C) 2009-2012                
* @author Jing HUANG
* @file FBOManager.cpp
* @brief 
* @date 1/2/2011
*/

#include "FBOManager.h"
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

	void checkFBOInit()
	{
		// check for multisample support, and add it as an option
		if (! glewIsSupported( "GL_EXT_framebuffer_blit GL_EXT_framebuffer_multisample")) {
			printf( "Warning: No support for multisampled Framebuffer objects\n");
		}
		else {
			//glGetIntegerv( GL_MAX_SAMPLES_EXT, &maxSamples);


			//check for CSAA support
			if (! glewIsSupported( "GL_NV_framebuffer_multisample_coverage")) {
				printf( "Warning: No support for coverage multisampling\n");
			}
			else {
				/*   // get the valid configs for Coverage multisampling
				glGetIntegerv( GL_MAX_MULTISAMPLE_COVERAGE_MODES_NV, &coverageSampleConfigs);

				coverageConfigs = new int[coverageSampleConfigs * 2 + 4];
				glGetIntegerv( GL_MULTISAMPLE_COVERAGE_MODES_NV, coverageConfigs);*/
			}
		}

		// Check for sRGB support, and add it as an option
		if (! glewIsSupported( "GL_EXT_texture_sRGB GL_EXT_framebuffer_sRGB")) {
			printf( "Warning: No support for sRGB framebuffer objects\n");
		}


		// check for float support, and add it as an option
		if (! glewIsSupported( "GL_ARB_texture_float")) {
			printf( "Warning: No support for floating point framebuffer objects\n");
		}


		// check for packed float support, and add it as an option
		if (! glewIsSupported( "GL_EXT_packed_float")) {
			printf( "Warning: No support for packed floating point framebuffer objects\n");
		}


		// check for depth float support, and add it as an option
		if (! glewIsSupported( "GL_NV_depth_buffer_float")) {
			printf( "Warning: No support for floating point depth buffers\n");
		}


	}



	FBOManager::FBOManager()
	{
		checkFBOInit();
	}

	FBOManager::~FBOManager()
	{
		clear();
	}

	void FBOManager::clear()
	{
		std::vector<FBO*>::iterator itor;
		for(itor=_fbolist.begin(); itor!=_fbolist.end();++itor){
			assert(*itor);
			delete (*itor);
		}
		_fbolist.clear();

	}

	FBO* FBOManager::getFBO(int width, int height)
	{
		std::vector<FBO*>::iterator itor;
		for(itor=_fbolist.begin(); itor!=_fbolist.end();++itor){
			assert(*itor);
			int w =((FBO*)(*itor))->getWidth();
			int h =((FBO*)(*itor))->getHeight();
			if( w == width && h == height ){
				return *itor;
			}
		}
		FBO* pfbo = new FBO();
		pfbo->initialize(width,height);
		_fbolist.push_back(pfbo);
		return pfbo;
	}

}
