/**
* Copyright(C) 2009-2012                
* @author Jing HUANG
* @file ComputeShader.cpp
* @brief 
* @date 1/2/2011
*/

#include "ComputeShader.h"
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

	ComputeShader::ComputeShader(): ShaderObject(SHADERTYPE_VERTEX)
	{
		_x_Compute = _y_Compute = _z_Compute = 0;
	}

	ComputeShader::ComputeShader(const std::string& shaderFilename):ShaderObject(SHADERTYPE_VERTEX,shaderFilename){}
	
	void ComputeShader::setSize(unsigned int x, unsigned int y, unsigned int z)
	{
		_x_Compute = x;
		_y_Compute = y;
		_z_Compute = z;
	}

	void ComputeShader::checkShaderObject()
	{
		ShaderObject::checkShaderObject();
		GLint maxGroupCount;
		glGetIntegeri_v(GL_MAX_COMPUTE_WORK_GROUP_COUNT,0, &maxGroupCount);
		std::cout<<"GL_MAX_COMPUTE_WORK_GROUP_COUNT: "<<maxGroupCount<<std::endl;
		GLint x, y, z;
		glGetIntegeri_v(GL_MAX_COMPUTE_WORK_GROUP_SIZE, 0, &x);
		glGetIntegeri_v(GL_MAX_COMPUTE_WORK_GROUP_SIZE, 1, &y);
		glGetIntegeri_v(GL_MAX_COMPUTE_WORK_GROUP_SIZE, 2, &z);
		std::cout<<"GL_MAX_COMPUTE_WORK_GROUP_SIZE: x["<< x<<"] y["<<y <<"] z["<<z<<"]"<<std::endl;
		GLint maximum_number_of_invocations;
		glGetIntegerv(GL_MAX_COMPUTE_WORK_GROUP_INVOCATIONS, &maximum_number_of_invocations);
		std::cout<<"GL_MAX_COMPUTE_WORK_GROUP_INVOCATIONS: "<<maximum_number_of_invocations<<std::endl;
	}
}
