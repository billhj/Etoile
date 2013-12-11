#pragma once
/* This sample queries the properties of the CUDA devices present in the system via CUDA Runtime API. 
CUDA setting needs to be careful,, cause different lib version, run time get error.
*/

#include "../Core/Core.h"

#ifdef _COMPILE_FOR_GPU_CUDA_
#pragma comment(lib, "cudart.lib")
#pragma comment(lib, "cuda.lib")

// CUDA-C includes
#include <cuda_runtime_api.h>
#include <string>
#include <stdio.h>
#include <iostream>
#include <cuda.h>


static bool CUDA_AVAILABLE = 0;

//void checkCUDAError(const char *msg);

bool CudaCheck(int argc, const char** argv);

// Beginning of GPU Architecture definitions
inline int ConvertSMVer2Cores(int major, int minor);

#endif