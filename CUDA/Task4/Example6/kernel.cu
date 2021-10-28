#include "cuda_runtime.h"
#include "device_launch_parameters.h"
#include <stdio.h>
#include <stdlib.h>
#include <locale.h>
const int N = 1024 * 33;
const int BLOCK_SIZE = 256;
const int blocksPerGrid = N / BLOCK_SIZE;

__global__ void kernel(int* a, int* b, int* c)
{
	__shared__ float cache[BLOCK_SIZE];
	int tid = threadIdx.x;
	int i = blockIdx.x * blockDim.x + threadIdx.x;
	float temp = 0;
	while (i < N)
	{
		temp += a[i] * b[i];
		i += blockDim.x * gridDim.x;
	}
	cache[tid] = temp;
	__syncthreads();
	for (int s = 1; s < blockDim.x; s *= 2)
	{
		//проверuть, участвует лu нить на данном шаге
		if (tid % (2 * s) == 0)
			cache[tid] += cache[tid + s];
		__syncthreads();
	}
	if (tid == 0)
		c[blockIdx.x] = cache[0];
}

int reduce(int* data, int N)
{
	int* sums = NULL;
	int numBlocks = N / BLOCK_SIZE;
	int res = 0;
	//Выделumь памяmь под массив сумм блоков.
	cudaMalloc((void**)&sums, numBlocks * sizeof(int));
	//Провесmu поблочную редукцuю, запuсав суммы
	//для каждого блока в массив sums
	kernel <<<dim3(numBlocks), dim3(BLOCK_SIZE)>>>(data, sums);
	//Теперь редуцируем массив сумм для блоков
	if (numBlocks > BLOCK_SIZE)
	{
		res = reduce(sums, numBlocks);
	}
	else 
	{
		//Еслu значенuе мало, то просуммируем явно
		int* sumsHost = new int[numBlocks];
		cudaMemcpy(sumsHost, sums, numBlocks * sizeof(int), cudaMemcpyDeviceToHost);
		for (int i = 0; i < numBlocks; i++)
			res += sumsHost[i];
		delete[] sumsHost;
	}
	cudaFree(sums);
	return res;
}

int main(void) 
{
	setlocale(LC_ALL, "RUS");
	
	return 0;
}