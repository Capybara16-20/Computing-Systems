#include "cuda_runtime.h"
#include "device_launch_parameters.h"
#include <stdio.h>
#include <locale.h>
#include <math.h>
#define N 32

__global__ void pow(int k, unsigned long long* c)
{
	int tid = threadIdx.x;
	c[tid] = pow(k, tid);
}

int main()
{
	setlocale(LC_ALL, "Rus");

	int k;
	printf("Введите номер варианта: ");
	scanf("%d", &k);
	k += 3;

	printf("k = %d\n\n", k);

	unsigned long long res[N];
	unsigned long long* dev_res;
	cudaMalloc((void**)&dev_res, N * sizeof(unsigned long long));
	cudaMemcpy(dev_res, res, N * sizeof(unsigned long long), cudaMemcpyHostToDevice);

	pow<<<1, N>>>(k, dev_res);

	cudaMemcpy(res, dev_res, N * sizeof(unsigned long long), cudaMemcpyDeviceToHost);

	printf("Степени числа %d\n", k);
	for (int i = 0; i < N; i++)
	{
		printf("%d ^ %d ", k, i);
		if (res[i] == ULLONG_MAX)
			printf(" > %zu\n", res[i]);
		else
			printf(" = %zu\n", res[i]);
	}

	cudaFree(dev_res);
	return 0;
}


