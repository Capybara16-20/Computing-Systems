#include <device_functions.h>
#include "cuda_runtime.h"
#include "device_launch_parameters.h"
#include <stdio.h>
#include<locale.h>
#define N 16

__global__ void squaresSum(float* matrix, float* row_sum, float* col_sum)
{
	__shared__ float row_cache[N], col_cache[N];
	int tid_row = threadIdx.x + blockIdx.x * N;
	int tid_col = blockIdx.x + threadIdx.x * N;
	row_cache[threadIdx.x] = matrix[tid_row] * matrix[tid_row];
	col_cache[threadIdx.x] = matrix[tid_col] * matrix[tid_col];
	__syncthreads();
	int i = blockDim.x / 2;
	while (i != 0)
	{
		if (threadIdx.x < i)
		{
			row_cache[threadIdx.x] += row_cache[threadIdx.x + i];
			col_cache[threadIdx.x] += col_cache[threadIdx.x + i];
		}
		__syncthreads();
		i /= 2;
	}
	if (threadIdx.x == 0)
	{
		row_sum[blockIdx.x] = row_cache[0];
		col_sum[blockIdx.x] = col_cache[0];
	}
}

int main(void)
{
	setlocale(LC_ALL, "Rus");
	float matrix[N * N];
	float* dev_matrix;
	float row_sum[N];
	float* dev_row_sum;
	float col_sum[N];
	float* dev_col_sum;
	cudaMalloc((void**)&dev_matrix, N * N * sizeof(float));
	cudaMalloc((void**)&dev_row_sum, N * sizeof(float));
	cudaMalloc((void**)&dev_col_sum, N * sizeof(float));
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
	squaresSum << <N, N >> > (dev_matrix, dev_row_sum, dev_col_sum);
	cudaMemcpy(row_sum, dev_row_sum, N * sizeof(float), cudaMemcpyDeviceToHost);
	cudaMemcpy(col_sum, dev_col_sum, N * sizeof(float), cudaMemcpyDeviceToHost);
	printf("\n");
	for (int i = 0; i < N; i++)
		printf("Сумма квадратов %d-й строки: %.0f  \tСумма квадратов %d-го столбца: %.0f\n",
			i + 1, row_sum[i], i + 1, col_sum[i]);
	cudaFree(dev_matrix);
	cudaFree(dev_row_sum);
	return 0;
}
