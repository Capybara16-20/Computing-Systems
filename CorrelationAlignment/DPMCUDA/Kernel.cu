#include "Kernel.cuh"
#include "cuda.h"
#include "cuda_runtime.h"
#include "device_launch_parameters.h"

__global__ void kernel(unsigned* src, int width, int height)
{
	int x = blockIdx.x * blockDim.x + threadIdx.x;
	int y = blockIdx.y * blockDim.y + threadIdx.y;
	if ((x < width) && (y < height))
	{
		unsigned pixel = src[y * width + x];
		unsigned gray = ((pixel & 0x00FF0000 >> 0x10) +
			(pixel & 0x0000FF00 >> 0x8) + (pixel & 0x000000FF)) / 3;
		src[y * width + x] = 0xFF000000 | (gray << 0x10) | (gray << 0x8) | gray;
	}
}

int calc(unsigned* srcImage, int width, int height)
{
	unsigned* dev_srcImage;
	size_t size = sizeof(unsigned) * width * height;
	if (cudaMalloc((void**)&dev_srcImage, size) != cudaError::cudaSuccess)
		return -1;
	if (cudaMemcpy(dev_srcImage, srcImage, size,
		cudaMemcpyHostToDevice) != cudaError::cudaSuccess)
		return -1;
	// Максимальное количество нитей на блок может отличаться
	dim3 threads(128, 128);
	dim3 blocks((width + threads.x - 1) / threads.x,
		(height + threads.y - 1) / threads.y);
	kernel <<<threads, blocks>>> (dev_srcImage, width, height);
	cudaError error = cudaMemcpy(srcImage, dev_srcImage, size,
		cudaMemcpyDeviceToHost);
	if (error != cudaError::cudaSuccess)
		return -1;
	cudaFree(dev_srcImage);
	return 0;
}

