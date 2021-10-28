﻿#include "cuda_runtime.h"
#include "device_launch_parameters.h"
#include <stdio.h>
#include <stdlib.h>
#include <locale.h>
const int N = 1024 * 33;
const int BLOCK_SIZE = 256;
const int blocksPerGrid = N / BLOCK_SIZE;

__global__ void mult5(int* a, int* b, int* c)
{
	__shared__ float cache[BLOCK_SIZE];
	int tid = threadIdx.x;
	int i = 2 * blockIdx.x * blockDim.x + threadIdx.x;
	//Записать сумму первых двух элементов в разделяемую память
	float temp = 0;
	while (i < N)
	{
		temp += a[i] * b[i];
		temp += a[i + blockDim.x] * b[i + blockDim.x];
		i += blockDim.x * gridDim.x;
	}
	cache[tid] = temp;
	__syncthreads();
	for (int s = blockDim.x / 2; s > 32; s >>= 1)
	{
		if (tid < s)
			cache[tid] += cache[tid + s];
		__syncthreads();
	}
	if (tid < 32)
	{
		cache[tid] += cache[tid + 32];
		cache[tid] += cache[tid + 16];
		cache[tid] += cache[tid + 8];
		cache[tid] += cache[tid + 4];
		cache[tid] += cache[tid + 2];
		cache[tid] += cache[tid + 1];
	}
	if (tid == 0)
		c[blockIdx.x] = cache[0];
}

/*__global__ void reduce5(int* inData, int* outData)
{
	__shared__ int data[BLOCK_SIZE];
	int tid = threadIdx.x;
	int i = 2 * blockIdx.x * blockDim.x + threadIdx.x;
	//Записать сумму первых двух элементов в разделяемую память
	data[tid] = inData[i] + inData[i + blockDim.x];
	__syncthreads();
	for (int s = blockDim.x / 2; s > 32; s >>= 1)
	{
		if (tid < s)
			data[tid] += data[tid + s];
		__syncthreads();
	}
	if (tid < 32)
	{
		data[tid] += data[tid + 32];
		data[tid] += data[tid + 16];
		data[tid] += data[tid + 8];
		data[tid] += data[tid + 4];
		data[tid] += data[tid + 2];
		data[tid] += data[tid + 1];
	}
	if (tid == 0)
		outData[blockIdx.x] = data[0];
}*/

int main(void)
{
	setlocale(LC_ALL, "RUS");
	cudaEvent_t start, stop;
	cudaEventCreate(&start);
	cudaEventCreate(&stop);
	cudaEventRecord(start, 0);
	int* a, * b, * partial_c, c;
	int* dev_a, * dev_b, * dev_partial_c;
	a = (int*)malloc(N * sizeof(int));
	b = (int*)malloc(N * sizeof(int));
	partial_c = (int*)malloc(blocksPerGrid * sizeof(int));
	cudaMalloc((void**)&dev_a, N * sizeof(int));
	cudaMalloc((void**)&dev_b, N * sizeof(int));
	cudaMalloc((void**)&dev_partial_c, blocksPerGrid * sizeof(int));
	for (int i = 0; i < N; i++)
	{
		a[i] = 1;
		b[i] = 2;
	}
	cudaMemcpy(dev_a, a, N * sizeof(int), cudaMemcpyHostToDevice);
	cudaMemcpy(dev_b, b, N * sizeof(int), cudaMemcpyHostToDevice);
	mult5 <<<blocksPerGrid, BLOCK_SIZE>>>(dev_a, dev_b, dev_partial_c);
	cudaMemcpy(partial_c, dev_partial_c, blocksPerGrid * sizeof(int), cudaMemcpyDeviceToHost);
	cudaEventRecord(stop, 0);
	cudaEventSynchronize(stop);
	float elapsedTime;
	cudaEventElapsedTime(&elapsedTime, start, stop);
	c = 0;
	for (int i = 0; i < blocksPerGrid; i++)
		c += partial_c[i];
	printf("Скалярное произведение, вычисленное на GPU, равно %d\n", c);
	printf("Время выполнения: %.6f миллисекунд\n", elapsedTime);
	cudaEventDestroy(start);
	cudaEventDestroy(stop);
	cudaFree(dev_a);
	cudaFree(dev_b);
	cudaFree(dev_partial_c);
	free(a);
	free(b);
	free(partial_c);
	return 0;
}
