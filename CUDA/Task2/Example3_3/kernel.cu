#include <device_functions.h>
#include "cuda_runtime.h"
#include "device_launch_parameters.h"
#include <stdio.h>
#include<locale.h>
#define N 16

__global__ void getAverage(float* matrix, float* average)
{
	__shared__ float cache[N];
	int tid_block = blockIdx.x;
	int tid_thread = threadIdx.x;
	cache[tid_thread] = matrix[tid_block * N + tid_thread] / N;
	__syncthreads();
	int i = blockDim.x / 2;
	while (i != 0)
	{
		if (tid_thread < i)
			cache[tid_thread] += cache[tid_thread + i];
		__syncthreads();
		i /= 2;
	}
	if (tid_thread == 0)
		average[blockIdx.x] = cache[0];
}

int main(void)
{
	setlocale(LC_ALL, "Rus");
	float matrix[N * N];
	float* dev_matrix;
	float average[N];
	float* dev_average;
	cudaMalloc((void**)&dev_matrix, N * N * sizeof(float));
	cudaMalloc((void**)&dev_average, N * sizeof(float));
	for (int i = 0; i < N; i++)
		for (int j = 0; j < N; j++)
			matrix[i * N + j] = i + j;
	cudaMemcpy(dev_matrix, matrix, N * N * sizeof(float), cudaMemcpyHostToDevice);
	printf("Матрица:\n");
	for (int i = 0; i < N; i++)
	{
		for (int j = 0; j < N; j++)
			printf("%.0f\t", matrix[i * N + j]);
		printf("\n");
	}
	getAverage <<<N, N>>>(dev_matrix, dev_average);
	cudaMemcpy(average, dev_average, N * sizeof(float), cudaMemcpyDeviceToHost);
	printf("\n");
	for (int i = 0; i < N; i++)
		printf("Среднее арефмитическое %d-й строки: %.1f\n", i + 1, average[i]);
	cudaFree(dev_matrix);
	cudaFree(dev_average);
	return 0;
}

