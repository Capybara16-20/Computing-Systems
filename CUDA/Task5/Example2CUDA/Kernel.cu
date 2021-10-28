#include "Kernel.cuh"
#include "cuda.h"
#include "cuda_runtime.h"
#include "device_launch_parameters.h"
//#include "device_functions.h"

__global__ void kernel(unsigned* src, unsigned* red, unsigned* green,
	unsigned* blue, int width, int height)
{
	int x = blockIdx.x * blockDim.x + threadIdx.x;
	int y = blockIdx.y * blockDim.y + threadIdx.y;
	if ((x < width) && (y < height))
	{
		unsigned pixel = src[y * width + x];
		unsigned gray = ((pixel & 0x00FF0000 >> 0x10) +
			(pixel & 0x0000FF00 >> 0x8) + (pixel & 0x000000FF)) / 3;
		unsigned graypixel = 0xFF000000 | (gray << 0x10) | (gray << 0x8) | gray;
		red[y * width + x] = graypixel | 0x00FF0000;
		green[y * width + x] = graypixel | 0x0000FF00;
		blue[y * width + x] = graypixel | 0x000000FF;
	}
}

int calc(unsigned* srcImage, unsigned* redImage, unsigned* greenImage,
	unsigned* blueImage, int width, int height)
{
	unsigned* dev_srcImage;
	unsigned* dev_redImage;
	unsigned* dev_greenImage;
	unsigned* dev_blueImage;
	size_t size = sizeof(unsigned) * width * height;
	if ((cudaMalloc((void**)&dev_srcImage, size) != cudaError::cudaSuccess) ||
		(cudaMalloc((void**)&dev_redImage, size) != cudaError::cudaSuccess) ||
		(cudaMalloc((void**)&dev_greenImage, size) != cudaError::cudaSuccess) ||
		(cudaMalloc((void**)&dev_blueImage, size) != cudaError::cudaSuccess))
		return -1;
	if ((cudaMemcpy(dev_srcImage, srcImage, size,
		cudaMemcpyHostToDevice) != cudaError::cudaSuccess) ||
		(cudaMemcpy(dev_redImage, redImage, size,
			cudaMemcpyHostToDevice) != cudaError::cudaSuccess) ||
		(cudaMemcpy(dev_greenImage, greenImage, size,
			cudaMemcpyHostToDevice) != cudaError::cudaSuccess) ||
		(cudaMemcpy(dev_blueImage, blueImage, size,
			cudaMemcpyHostToDevice) != cudaError::cudaSuccess))
		return -1;
	// Максимальное количество нитей на блок может отличаться
	dim3 threads(128, 128);
	dim3 blocks((width + threads.x - 1) / threads.x,
		(height + threads.y - 1) / threads.y);
	kernel << <threads, blocks >> > (dev_srcImage, dev_redImage,
		dev_greenImage, dev_blueImage, width, height);
	cudaMemcpy(srcImage, dev_srcImage, size, cudaMemcpyDeviceToHost);
	cudaMemcpy(redImage, dev_redImage, size, cudaMemcpyDeviceToHost);
	cudaMemcpy(greenImage, dev_greenImage, size, cudaMemcpyDeviceToHost);
	cudaMemcpy(blueImage, dev_blueImage, size, cudaMemcpyDeviceToHost);
	cudaFree(dev_srcImage);
	cudaFree(dev_redImage);
	cudaFree(dev_greenImage);
	cudaFree(dev_blueImage);
	return 0;
}

