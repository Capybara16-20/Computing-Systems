#include "cuda_runtime.h"
#include "device_launch_parameters.h"
#include <stdio.h>
#include <stdlib.h>
#include <locale.h>
#include <windows.h>
#include <time.h>
#define SIZE (100 * 1024 * 1024)

void* big_random_block(int size)
{
	unsigned char* data = (unsigned char*)malloc(size);
	for (int i = 0; i < size; i++)
		data[i] = rand();
	return data;
}

__global__ void histo_kernel(unsigned char* buffer,
	long size, unsigned int* histo) 
{
	__shared__ unsigned int temp[256];
	temp[threadIdx.x] = 0;
	__syncthreads();
	int i = threadIdx.x + blockIdx.x * blockDim.x;
	int stride = blockDim.x * gridDim.x;
	while (i < size) 
	{
		atomicAdd(&temp[buffer[i]], 1);
		i += stride;
	}
	__syncthreads();
	atomicAdd(&(histo[threadIdx.x]), temp[threadIdx.x]);
}

/*__global__ void histo_kernel(unsigned char* buffer,
	long size, unsigned int* histo)
{
	int i = threadIdx.x + blockIdx.x * blockDim.x;
	int stride = blockDim.x * gridDim.x;
	while (i < size) 
	{
		atomicAdd(&histo[buffer[i]], 1);
		i += stride;
	}
}*/

int main(void) 
{
	setlocale(LC_ALL, "RUS");
	unsigned char* buffer = (unsigned char*)big_random_block(SIZE);
	cudaEvent_t start, stop;
	cudaEventCreate(&start);
	cudaEventCreate(&stop);
	cudaEventRecord(start, 0);

	unsigned char* dev_buffer;
	unsigned int* dev_histo;
	cudaMalloc((void**)&dev_buffer, SIZE);
	cudaMalloc((void**)&dev_histo, 256 * sizeof(int));
	cudaMemcpy(dev_buffer, buffer, SIZE, cudaMemcpyHostToDevice);
	cudaMemset(dev_histo, 0, 256 * sizeof(int));

	//наилучшая производительность достигается,
	//когда количество запускаемых блоков
	//в 2 раза больше количества мультипроцессоров
	cudaDeviceProp prop;
	cudaGetDeviceProperties(&prop, 0);
	int blocks = prop.multiProcessorCount;
	histo_kernel <<<blocks * 2, 256>>>(dev_buffer, SIZE, dev_histo);
	unsigned int histo[256];
	cudaMemcpy(histo, dev_histo, 256 * sizeof(int), cudaMemcpyDeviceToHost);
	cudaEventRecord(stop, 0);
	cudaEventSynchronize(stop);
	float elapsedTime;
	cudaEventElapsedTime(&elapsedTime, start, stop);
	printf("Время выполнения: %3.1f миллисекунд\n", elapsedTime);
	long histoCount = 0;
	for (int i = 0; i < 256; i++)
		histoCount += histo[i];
	printf("Сумма гистограммы на GPU: %ld\n", histoCount);

	for (int i = 0; i < SIZE; i++)
			histo[buffer[i]]--;
	for (int i = 0; i < 256; i++)
		if (histo[i] != 0)
			printf("Ошибка в столбце %d\n", i);

	cudaEventDestroy(start);
	cudaEventDestroy(stop);
	cudaFree(dev_histo);
	cudaFree(dev_buffer);
	free(buffer);
	return 0;
}


