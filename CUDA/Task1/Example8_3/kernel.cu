#include <device_functions.h>
#include "cuda_runtime.h"
#include "device_launch_parameters.h"
#include <stdio.h>
#include<locale.h>
#define N 10
#define K 3

__global__ void mult(int* matrix, int* result)
{
	int tid_block = blockIdx.x;
	int tid_thread = threadIdx.x;
	if (tid_block < N && tid_thread < N)
		result[tid_block * N + tid_thread] = matrix[tid_block * N + tid_thread] * 3;
}

int main(void)
{
	setlocale(LC_ALL, "Rus");
	int matrix[N * N];
	int* dev_matrix;
	int result[N * N];
	int* dev_result;
	cudaMalloc((void**)&dev_matrix, N * N * sizeof(int));
	cudaMalloc((void**)&dev_result, N * N * sizeof(int));
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
	mult<<<N, N>>>(dev_matrix, dev_result);
	cudaMemcpy(result, dev_result, N * N * sizeof(int), cudaMemcpyDeviceToHost);
	printf("\nРезультат:\n");
	for (int i = 0; i < N; i++)
	{
		for (int j = 0; j < N; j++)
			printf("%d\t", result[i * N + j]);
		printf("\n");
	}
	cudaFree(dev_matrix);
	cudaFree(dev_result);
	return 0;
}

