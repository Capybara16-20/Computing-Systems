#include "cuda_runtime.h"
#include "device_launch_parameters.h"
#include <stdio.h>
#define N 10

__global__ void kernel()
{
	//обработать данные, находящиеся по этому индексу
	int tid = blockIdx.x;
	printf("Block number %d\n", tid);
}

int main(void)
{
	int* dev_a;
	// выделение памяти на GPU
	cudaMalloc((void**)&dev_a, sizeof(int));
	kernel<<<N, 1>>>(); //вызов ядра
	cudaFree(dev_a); // освобождение памяти на устройстве GPU
}
