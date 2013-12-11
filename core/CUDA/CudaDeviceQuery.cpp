#include "CudaDeviceQuery.h"

#ifdef _COMPILE_FOR_GPU_CUDA_
/**
 * @brief For tracking memory leaks under windows using the crtdbg
 */
#if ( defined( _DEBUG ) || defined( DEBUG ) ) && defined( _MSC_VER )
	#define _CRTDBG_MAP_ALLOC
	#include <crtdbg.h>

	#define DEBUG_NEW new( _NORMAL_BLOCK, __FILE__, __LINE__ )
	#define new DEBUG_NEW
#endif


void checkCUDAError(const char *msg)
{
    cudaError_t err = cudaGetLastError();
    if( cudaSuccess != err) 
    {
        fprintf(stderr, "Cuda error: %s: %s.\n", msg, 
                                  cudaGetErrorString( err) );
      //  exit(EXIT_FAILURE);
    }                         
}



// Beginning of GPU Architecture definitions
inline int ConvertSMVer2Cores(int major, int minor)
{
	// Defines for GPU Architecture types (using the SM version to determine the # of cores per SM
	typedef struct {
		int SM; // 0xMm (hexidecimal notation), M = SM Major version, and m = SM minor version
		int Cores;
	} sSMtoCores;

	sSMtoCores nGpuArchCoresPerSM[] = 
	{ { 0x10,  8 },
	  { 0x11,  8 },
	  { 0x12,  8 },
	  { 0x13,  8 },
	  { 0x20, 32 },
	  { 0x21, 48 },
	  {   -1, -1 }
	};

	int index = 0;
	while (nGpuArchCoresPerSM[index].SM != -1) {
		if (nGpuArchCoresPerSM[index].SM == ((major << 4) + minor) ) {
			return nGpuArchCoresPerSM[index].Cores;
		}
		index++;
	}
	printf("MapSMtoCores undefined SMversion %d.%d!\n", major, minor);
	return -1;
}
// end of GPU Architecture definitions


////////////////////////////////////////////////////////////////////////////////
// Program main
////////////////////////////////////////////////////////////////////////////////
bool CudaCheck(int argc, const char** argv)
{
	// shrSetLogFileName ("deviceQuery.txt");
	if(argv == 0) std::cout<<" Starting..."<<std::endl; 
	else std::cout<< argv[0]<<" Starting..."<<std::endl; 
    std::cout<<" CUDA Device Query (Runtime API) version (CUDART static linking)\n\n"<<std::endl;
        
    int deviceCount = 0;
	if (cudaGetDeviceCount(&deviceCount) != cudaSuccess) {
		std::cout<<"cudaGetDeviceCount FAILED CUDA Driver and Runtime version may be mismatched.\n"<<std::endl;
		std::cout<<"FAILED"<<std::endl;
		CUDA_AVAILABLE = 0;
		return 0;
	}

    // This function call returns 0 if there are no CUDA capable devices.
    if (deviceCount == 0)
       std::cout<<"There is no device supporting CUDA"<<std::endl;

    int dev;
	int driverVersion = 0, runtimeVersion = 0;     
    for (dev = 0; dev < deviceCount; ++dev) {
        cudaDeviceProp deviceProp;
        cudaGetDeviceProperties(&deviceProp, dev);

        if (dev == 0) {
			// This function call returns 9999 for both major & minor fields, if no CUDA capable devices are present
            if (deviceProp.major == 9999 && deviceProp.minor == 9999)
                std::cout<<"There is no device supporting CUDA."<<std::endl;
            else if (deviceCount == 1)
                std::cout<<"There is 1 device supporting CUDA"<<std::endl;
            else
                std::cout<<"There are "<< deviceCount<<" devices supporting CUDA"<<std::endl;
        }
		std::cout<<"Device "<<  dev<<" : "<< deviceProp.name<<std::endl;

    #if CUDART_VERSION >= 2020
        // Console log
		cudaDriverGetVersion(&driverVersion);
		std::cout<<"  CUDA Driver Version:"<< driverVersion/1000<<"." <<driverVersion%100<<std::endl;
		cudaRuntimeGetVersion(&runtimeVersion);
		std::cout<<"  CUDA Runtime Version:"<< runtimeVersion/1000<<"."<< runtimeVersion%100<<std::endl;
    #endif
        std::cout<<"  CUDA Capability Major/Minor version number:" <<deviceProp.major<<"."<< deviceProp.minor<<std::endl;

		char msg[256];
		sprintf(msg, "  Total amount of global memory:                 %llu bytes\n", (unsigned long long) deviceProp.totalGlobalMem);
		std::cout<<msg<<std::endl;
    #if CUDART_VERSION >= 2000
		std::cout<<"  Multiprocessors x Cores/MP = Cores: "<< deviceProp.multiProcessorCount <<" (MP) x "<<ConvertSMVer2Cores(deviceProp.major, deviceProp.minor) <<" (Cores/MP) = "<<ConvertSMVer2Cores(deviceProp.major, deviceProp.minor) * deviceProp.multiProcessorCount<<" (Cores)"<<std::endl; 
			
    #endif
		std::cout<<"  Total amount of constant memory:              "<< deviceProp.totalConstMem<<" bytes"<<std::endl; 
        std::cout<<"  Total amount of shared memory per block:      "<< deviceProp.sharedMemPerBlock<<" bytes"<<std::endl; 
		std::cout<<"  Total number of registers available per block: "<<deviceProp.regsPerBlock<<std::endl;
		std::cout<<"  Warp size:                                    "<<deviceProp.warpSize<<std::endl;
		std::cout<<"  Maximum number of threads per block:           "<<deviceProp.maxThreadsPerBlock<<std::endl;
		std::cout<<"  Maximum sizes of each dimension of a block:   "<<deviceProp.maxThreadsDim[0] <<" x "<<deviceProp.maxThreadsDim[1]<<" x "<<deviceProp.maxThreadsDim[2]<<std::endl;
           
		std::cout<<"  Maximum sizes of each dimension of a grid:    "<<deviceProp.maxGridSize[0]<<" x "<< deviceProp.maxGridSize[1]<<" x "<< deviceProp.maxGridSize[2]<<std::endl;
             
		std::cout<<"  Maximum memory pitch:                         "<<deviceProp.memPitch<<" bytes"<<std::endl;
		std::cout<<"  Texture alignment:                             "<<deviceProp.textureAlignment<<" bytes"<<std::endl;
		std::cout<<"  Clock rate:                                    "<<deviceProp.clockRate * 1e-6f<<" GHz"<<std::endl;
    #if CUDART_VERSION >= 2000
		std::cout<<"  Concurrent copy and execution:                 "<<(deviceProp.deviceOverlap ? "Yes" : "No")<<std::endl;
    #endif
    #if CUDART_VERSION >= 2020
		std::cout<<"  Run time limit on kernels:                     "<<(deviceProp.kernelExecTimeoutEnabled ? "Yes" : "No")<<std::endl;
		std::cout<<"  Integrated:                                    "<< (deviceProp.integrated ? "Yes" : "No")<<std::endl;
		std::cout<<"  Support host page-locked memory mapping:       "<< (deviceProp.canMapHostMemory ? "Yes" : "No")<<std::endl;
        std::cout<<"  Compute mode:                                  "<< (deviceProp.computeMode == cudaComputeModeDefault ?
			                                                            "Default (multiple host threads can use this device simultaneously)" :
		                                                                deviceProp.computeMode == cudaComputeModeExclusive ?
																		"Exclusive (only one host thread at a time can use this device)" :
		                                                                deviceProp.computeMode == cudaComputeModeProhibited ?
																		"Prohibited (no host thread can use this device)" :
																		"Unknown")<<std::endl;
    #endif
    #if CUDART_VERSION >= 3000
																		std::cout<<"  Concurrent kernel execution:                   "<<( deviceProp.concurrentKernels ? "Yes" : "No")<<std::endl;
    #endif
    #if CUDART_VERSION >= 3010
																		std::cout<<"  Device has ECC support enabled:                "<< (deviceProp.ECCEnabled ? "Yes" : "No")<<std::endl;
    #endif
    #if CUDART_VERSION >= 3020
																		std::cout<<"  Device is using TCC driver mode:               "<<( deviceProp.tccDriver ? "Yes" : "No")<<std::endl;
    #endif
	}

    // csv masterlog info
    // *****************************
    // exe and CUDA driver name 
	std::cout<<std::endl;    
    std::string sProfileString = "deviceQuery, CUDA Driver = CUDART";        
    char cTemp[10];
    
    // driver version
    sProfileString += ", CUDA Driver Version = ";
    #ifdef WIN32
	    sprintf_s(cTemp, 10, "%d.%d", driverVersion/1000, driverVersion%100);    
    #else
	    sprintf(cTemp, "%d.%d", driverVersion/1000, driverVersion%100);	
    #endif
    sProfileString +=  cTemp;
    
    // Runtime version
    sProfileString += ", CUDA Runtime Version = ";
    #ifdef WIN32
	    sprintf_s(cTemp, 10, "%d.%d", runtimeVersion/1000, runtimeVersion%100);
    #else
	    sprintf(cTemp, "%d.%d", runtimeVersion/1000, runtimeVersion%100);
    #endif
    sProfileString +=  cTemp;  
    
    // Device count      
    sProfileString += ", NumDevs = ";
    #ifdef WIN32
        sprintf_s(cTemp, 10, "%d", deviceCount);
    #else
        sprintf(cTemp, "%d", deviceCount);
    #endif
    sProfileString += cTemp;
    
    // First 2 device names, if any
    for (dev = 0; dev < ((deviceCount > 2) ? 2 : deviceCount); ++dev) 
    {
        cudaDeviceProp deviceProp;
        cudaGetDeviceProperties(&deviceProp, dev);
        sProfileString += ", Device = ";
        sProfileString += deviceProp.name;
    }
    sProfileString += "\n";
    //shrLogEx(LOGBOTH | MASTER, 0, sProfileString.c_str());
	std::cout<<sProfileString.c_str()<<std::endl;
    // finish
	std::cout<<"PASSED"<<std::endl;
	CUDA_AVAILABLE = 1;
	return 1;
}
/*
int main( int argc, const char** argv) 
{
   bool check = CudaCheck(argc, argv);
   exit(0);
}
*/

#endif