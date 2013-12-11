/**
* Copyright(C) 2009-2012                
* @author Jing HUANG
* @file TessCtrlShader.cpp
* @brief 
* @date 1/2/2011
*/

#include "TessCtrlShader.h"
#include <vector>

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

	TessCtrlShader::TessCtrlShader(): ShaderObject(SHADERTYPE_TESSELLATION_CTRL)
	{
	}

	TessCtrlShader::TessCtrlShader(const std::string& shaderFilename):ShaderObject(SHADERTYPE_TESSELLATION_CTRL,shaderFilename){}

}
