#pragma once
/* This sample queries the properties of the CUDA devices present in the system. */
#include "../Core/Core.h" 
// Standard utilities and common systems includes
//#include <oclUtils.h>

#ifdef COMPILE_FOR_GPU_OPENCL
#include <string>
#include <stdio.h>
#include <iostream>
#include <CL/opencl.h>
#include <CL/cl_platform.h>

#pragma comment(lib, "OpenCL.lib")

#ifdef UNIX
#include <sstream>
#include <fstream>
#endif



static bool OPENCL_AVAILABLE = 0;

bool OpenCLCheck(int argc, const char** argv);

inline void checkOpenCLError(cl_int err, const char * name);

#endif
