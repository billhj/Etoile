#pragma once
/* This sample queries the properties of the CUDA devices present in the system via CUDA Runtime API. */
#include "../Core/Core.h" 
// CUDA-C includes
//#define _COMPILE_FOR_GPU_CUDA_
#ifdef _COMPILE_FOR_GPU_CUDA_
#       include <cuda.h>
#		include "CudaDeviceQuery.h"
#       ifndef GPU_DEVICE
#               define GPU_DEVICE __forceinline__ __device__
#       endif
#       ifndef GPU_HOST
#               define GPU_HOST __host__
#       endif
#       ifndef GPU_HOST_AND_DEVICE
#               define GPU_HOST_AND_DEVICE __device__ __host__
#       endif
#else
#       ifndef GPU_DEVICE
#               define GPU_DEVICE
#       endif
#       ifndef GPU_HOST
#               define GPU_HOST
#       endif
#       ifndef GPU_HOST_AND_DEVICE
#               define GPU_HOST_AND_DEVICE
#       endif
#endif




