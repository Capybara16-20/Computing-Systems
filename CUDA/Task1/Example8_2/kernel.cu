#include <device_functions.h>
#include "cuda_runtime.h"
#include "device_launch_parameters.h"
#include <stdio.h>
#include<locale.h>
#define N 15

__global__ void row_sum(int* matrix, int* sum)
{
	__shared__ float cache[cudaDevAttrMaxThreadsPerBlock];
	int tid_block = blockIdx.x;
	int tid_thread = threadIdx.x;
	cache[tid_thread] = matrix[tid_block * N + tid_thread];
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
		sum[blockIdx.x] = cache[0];
}

int main(void)
{
	setlocale(LC_ALL, "Rus");
	int matrix[N * N];
	int* dev_matrix;
	int sum[N];
	int* dev_sum;
	cudaMalloc((void**)&dev_matrix, N * N * sizeof(int));
	cudaMalloc((void**)&dev_sum, N * sizeof(int));
	for (int i = 0; i < N; i++)
		for (int j = 0; j < N; j++)
			matrix[i * N + j] = i + j;
	cudaMemcpy(dev_matrix, matrix, N * N * sizeof(int), cudaMemcpyHostToDevice);
	printf("Матрица:\n");
	for (int i = 0; i < N; i++)
	{
		for (int j = 0; j < N; j++)
			printf("%d\t", matrix[i * N + j]);
		printf("\n");
	}
	row_sum <<<N, N>>>(dev_matrix, dev_sum);
	cudaMemcpy(sum, dev_sum, N * sizeof(int), cudaMemcpyDeviceToHost);
	printf("\n");
	for (int i = 0; i < N; i++)
		printf("Сумма %d-й строки: %d\n", i + 1, sum[i]);
	cudaFree(dev_matrix);
	cudaFree(dev_sum);
	return 0;
}

