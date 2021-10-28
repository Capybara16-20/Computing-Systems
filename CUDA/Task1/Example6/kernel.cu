#include "cuda_runtime.h"
#include "device_launch_parameters.h"
#include <stdio.h>
#define N 10
#define M 3

__global__ void kernel()
{
	// обработать данные по этому индексу
	int tid_block = blockIdx.x;
	int tid_thread = threadIdx.x;
	printf("Block number %d. Thread number %d\n", tid_block, tid_thread);
}

int main(void)
{
	int* dev_a;
	// выделение памяти на GPU
	cudaMalloc((void**)&dev_a, sizeof(int));
	kernel <<<M, N>>>(); //вызов ядра
	// освобождение памяти на устройстве GPU
	cudaFree(dev_a);
	return 0;
}