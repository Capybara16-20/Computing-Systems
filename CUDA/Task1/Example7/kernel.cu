#include "cuda_runtime.h"
#include "device_launch_parameters.h"
#include <math.h>
#include <stdio.h>
#define CUDA_FLOAT float
#define GRID_SIZE 256
#define BLOCK_SIZE 256

__global__ void pi_kern(CUDA_FLOAT* res)
{
	int n = threadIdx.x + blockIdx.x * BLOCK_SIZE;
	// Начало отрезка интегрирования
	CUDA_FLOAT x0 = n * 1.f / (BLOCK_SIZE * GRID_SIZE);
	CUDA_FLOAT y0 = sqrtf(1 - x0 * x0);
	CUDA_FLOAT dx = 1.f / (1.f * BLOCK_SIZE * GRID_SIZE); // Шаги интегрирования
	// Значение интеграла по отрезку, данному текущему треду
	CUDA_FLOAT s = 0;
	CUDA_FLOAT x1, y1;
	x1 = x0 + dx;
	y1 = sqrtf(1 - x1 * x1);
	s = (y0 + y1) * dx / 2.f; // Площадь трапеции
	res[n] = s; // Запись результата в глобальную память
}

int main() 
{
	CUDA_FLOAT pi = 0;
	CUDA_FLOAT* res_d; // Результаты на устройстве
	CUDA_FLOAT res[GRID_SIZE * BLOCK_SIZE]; // Результаты в host памяти
	// Выделение памяти на CPU
	cudaMalloc((void**)&res_d, sizeof(CUDA_FLOAT) * GRID_SIZE * BLOCK_SIZE);
	// Размеры грида и блока на GPU
	dim3 grid(GRID_SIZE);
	dim3 block(BLOCK_SIZE);
	pi_kern<<<grid, block>>>(res_d); // Запуск ядра
	cudaThreadSynchronize(); // Ожидаем завершения работы ядра
	cudaMemcpy(&res, res_d, sizeof(CUDA_FLOAT) * GRID_SIZE * BLOCK_SIZE, 
		cudaMemcpyDeviceToHost); // Копируем результаты на хост
	cudaFree(res_d); // Освобождаем память на GPU
	for (int i = 0; i < GRID_SIZE * BLOCK_SIZE; i++)
		pi += res[i];
	pi *= 4;
	printf("PI = %f\n", pi);
	return 0;
}


