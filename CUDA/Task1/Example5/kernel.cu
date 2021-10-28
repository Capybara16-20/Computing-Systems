#include "cuda_runtime.h"
#include "device_launch_parameters.h"
#include <stdio.h>
#define N 10

__global__ void kernel() 
{
	//обработать данные, находящиеся по этому индексу
	int tid = threadIdx.x;
	printf("Thread number %d\n", tid);
}

int main(void)
{
	int* dev_a;
	//выделение памятина GPU
	cudaMalloc((void**)&dev_a, sizeof(int));
	kernel<<<1, N>>>(); //вызов ядра
	// освобождение памяти на устройстве GPU
	cudaFree(dev_a);
	return 0;
}