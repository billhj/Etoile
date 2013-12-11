
#include "cuda_runtime.h"
#include "device_launch_parameters.h"

#include <stdio.h>
#include <iostream>



void checkCUDAError(const char *msg)
{
    cudaError_t err = cudaGetLastError();
    if( cudaSuccess != err) 
    {
        fprintf(stderr, "Cuda error: %s: %s.\n", msg,  cudaGetErrorString( err) );
    }                         
}

cudaError_t FFTWithCuda(float *input, float *output, size_t size);

__global__ void addKernel(float *input, float *output)
{
	int i = ( blockIdx.x * blockDim.x ) + threadIdx.x;
	output[i] = input[i] + 0.5;
	if(output[i] > 1)
		output[i] = 1;
}

int applyFFT(float* input, float* output, int size)
{

	// Add vectors in parallel.
	cudaError_t cudaStatus = FFTWithCuda(input, output, size);
	if (cudaStatus != cudaSuccess) {
		fprintf(stderr, "FFTWithCuda failed!");
		return 1;
	}

	// cudaDeviceReset must be called before exiting in order for profiling and
	// tracing tools such as Nsight and Visual Profiler to show complete traces.
	cudaStatus = cudaDeviceReset();
	if (cudaStatus != cudaSuccess) {
		fprintf(stderr, "cudaDeviceReset failed!");
		return 1;
	}

	return 0;
}

// Helper function for using CUDA to add vectors in parallel.
cudaError_t FFTWithCuda(float *input, float *output, size_t size)
{
	int dimElement = size;
	int numThreadsPerBlock = 100; 
	int numBlocks = dimElement / numThreadsPerBlock;
	dim3 dimGrid(numBlocks);
	dim3 dimBlock(numThreadsPerBlock);
	size_t memSize = dimElement * sizeof(*input);
	printf("nb of elements: %d \n", dimElement);
	printf("memory size: %d \n", memSize);

	float *dev_i = 0;
	float *dev_o = 0;
	cudaError_t cudaStatus;

	// Choose which GPU to run on, change this on a multi-GPU system.
	cudaStatus = cudaSetDevice(0);
	if (cudaStatus != cudaSuccess) {
		checkCUDAError("cudaSetDevice failed!  Do you have a CUDA-capable GPU installed?");
		return cudaStatus;
	}

	
	// Allocate GPU buffers for three vectors (two input, one output)    .
	cudaStatus = cudaMalloc((void**)&dev_i, size * sizeof(float));
	if (cudaStatus != cudaSuccess) {
		checkCUDAError("cudaMalloc failed!");
		return cudaStatus;
	}

	cudaStatus = cudaMalloc((void**)&dev_o, size * sizeof(float));
	if (cudaStatus != cudaSuccess) {
		checkCUDAError("cudaMalloc failed!");
		return cudaStatus;
	}

	// Copy input vectors from host memory to GPU buffers.
	cudaStatus = cudaMemcpy(dev_i, input, size * sizeof(float), cudaMemcpyHostToDevice);
	if (cudaStatus != cudaSuccess) {
		checkCUDAError("cudaMemcpy failed!");
		return cudaStatus;
	}

	// Launch a kernel on the GPU with one thread for each element.
	addKernel<<<dimGrid, dimBlock>>>(dev_i, dev_o);

	// cudaDeviceSynchronize waits for the kernel to finish, and returns
	// any errors encountered during the launch.
	cudaStatus = cudaDeviceSynchronize();
	if (cudaStatus != cudaSuccess) {
		checkCUDAError("cudaDeviceSynchronize failed!");
		printf("cudaDeviceSynchronize returned error code %d after launching addKernel!\n", cudaStatus);
		return cudaStatus;
	}

	// Copy output vector from GPU buffer to host memory.
	cudaStatus = cudaMemcpy(output, dev_o, size * sizeof(float), cudaMemcpyDeviceToHost);
	if (cudaStatus != cudaSuccess) {
		checkCUDAError("cudaMemcpy failed!");
		return cudaStatus;
	}

	return cudaStatus;
}
