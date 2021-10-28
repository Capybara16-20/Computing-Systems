#include "device_launch_parameters.h"
#include <stdio.h>
#include <locale.h>
#include <stdlib.h>
//функция определения наименьшего значения
#define imin(a, b) (a < b ? a : b)

const int N = 33 * 1024; //количество элементов в векторе
const int threadsPerBlock = 256; //количество нитей в блоке
//количество блоков в гриде
const int blocksPerGrid = imin(32, (N + threadsPerBlock - 1) / threadsPerBlock);
__global__ void kernel(float* a, float* b, float* c)
{
	__shared__ float cache[threadsPerBlock];
	int tid = threadIdx.x + blockIdx.x * threadsPerBlock;
	int cacheIndex = threadIdx.x;
	float temp = 0;
	while (tid < N)
	{
		temp += a[tid] * b[tid];
		tid += blockDim.x * gridDim.x;
	}
	cache[cacheIndex] = temp;
	__syncthreads();
	int i = blockDim.x / 2;
	while (i != 0)
	{
		if (cacheIndex < i)
			cache[cacheIndex] += cache[cacheIndex + i];
		__syncthreads();
		i /= 2;
	}
	if (cacheIndex == 0)
		c[blockIdx.x] = cache[0];
}

int main(void)
{
	setlocale(LC_ALL, "RUS");
	float* a, * b, c, * partial_c;
	float* dev_a, * dev_b, * dev_partial_c;
	//выделение памяти на CPU
	a = (float*)malloc(N * sizeof(float));
	b = (float*)malloc(N * sizeof(float));
	partial_c = (float*)malloc(blocksPerGrid * sizeof(float));
	//выделение памяти на GPU
	cudaMalloc((void**)&dev_a, N * sizeof(float));
	cudaMalloc((void**)&dev_b, N * sizeof(float));
	cudaMalloc((void**)&dev_partial_c, blocksPerGrid * sizeof(float));
	//заполнение массивов данными на хосте
	for (int i = 0; i < N; i++)
	{
		a[i] = i;
		b[i] = i * 2;
	}
	//копирование массивов 'a' и 'b' на GPU
	cudaMemcpy(dev_a, a, N * sizeof(float), cudaMemcpyHostToDevice);
	cudaMemcpy(dev_b, b, N * sizeof(float), cudaMemcpyHostToDevice);
	kernel <<<blocksPerGrid, threadsPerBlock>>>(dev_a, dev_b, dev_partial_c);
	//копирование массива 'c' с GPU назад на CPU
	cudaMemcpy(partial_c, dev_partial_c, blocksPerGrid * sizeof(float), cudaMemcpyDeviceToHost);
	//окончание вычислений на CPU
	c = 0;
	for (int i = 0; i < blocksPerGrid; i++)
		c += partial_c[i];
	printf("Скалярное произведение векторов, вычисленное на GPU, равно % .6f\n", c);
	//освобождение памяти на GPU
	cudaFree(dev_a);
	cudaFree(dev_b);
	cudaFree(dev_partial_c);
	//освобождение памяти на CPU
	free(a);
	free(b);
	free(partial_c);
	return 0;
}

