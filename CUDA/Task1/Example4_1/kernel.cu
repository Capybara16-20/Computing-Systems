#include "cuda_runtime.h"
#include "device_launch_parameters.h"
#include <stdio.h>
#define N 10

__global__ void add(int* a, int* b, int* c)
{
	int tid = threadIdx.x; //обработать данные, находящиеся по этому индексу
	if (tid < N)
	{
		c[tid] = a[tid] + b[tid];
		printf("Thread number %d write %d in array on %d position\n", tid, c[tid], tid);
	}
}

int main()
{
	int a[N], b[N], c[N];
	int* dev_a, * dev_b, * dev_c;
	// выделение памяти на GPU
	cudaMalloc((void**)&dev_a, N * sizeof(int));
	cudaMalloc((void**)&dev_b, N * sizeof(int));
	cudaMalloc((void**)&dev_c, N * sizeof(int));
	// заполнение массивов ‘a’ и ‘b’ CPU
	for (int i = 0; i < N; i++)
	{
		a[i] = i;
		b[i] = i * i;
	}
	// копирование массивов 'a' и'b' на GPU
	cudaMemcpy(dev_a, a, N * sizeof(int), cudaMemcpyHostToDevice);
	cudaMemcpy(dev_b, b, N * sizeof(int), cudaMemcpyHostToDevice);
	add<<<1, N>>>(dev_a, dev_b, dev_c); //вызов ядра
	// копирование массива 'c' с GPU на CPU
	cudaMemcpy(c, dev_c, N * sizeof(int), cudaMemcpyDeviceToHost);
	// отображение элементов результирующего массива
	printf("\n");
	for (int i = 0; i < N; i++)
		printf("%d + %d = %d\n", a[i], b[i], c[i]);
	cudaFree(dev_a); // освобождение памяти на GPU
	cudaFree(dev_b);
	cudaFree(dev_c);
	return 0;
}


