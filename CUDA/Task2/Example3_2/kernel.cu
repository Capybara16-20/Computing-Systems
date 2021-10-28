#include "cuda_runtime.h"
#include "device_launch_parameters.h"
#include <stdio.h>
#include <locale.h>
#define N 16

const int threadsPerBlock = 4;
const int blocksCount = 4;
__global__ void differenceNorm(float* a, float* b, float* c)
{
    __shared__ float cache[threadsPerBlock];
    int tid = threadIdx.x + blockIdx.x * threadsPerBlock;
    int cacheIndex = threadIdx.x;
    float temp = 0;
    while (tid < N)
    {
        temp += (a[tid] - b[tid]) * (a[tid] - b[tid]);
        tid += blockDim.x * gridDim.x;
    }
    cache[cacheIndex] = temp;
    __syncthreads();
    int i = blockDim.x / 2;
    while (i != 0)
    {
        if (cacheIndex < i)
            cache[cacheIndex] += cache[cacheIndex + i];
        __syncthreads();
        i /= 2;
    }
    if (cacheIndex == 0)
        c[blockIdx.x] = cache[0];
}

int main()
{
    setlocale(LC_ALL, "Rus");
    float a[N], b[N], c[N];
    float* dev_a, * dev_b, * dev_c;
    cudaMalloc((void**)&dev_a, N * sizeof(float));
    cudaMalloc((void**)&dev_b, N * sizeof(float));
    cudaMalloc((void**)&dev_c, N * sizeof(float));
    for (int i = 0; i < N; i++)
    {
        a[i] = i;
        b[i] = i * i;
    }
    for (int i = 0; i < N; i++)
        printf("(%.0f - %.0f)^2\n", a[i], b[i]);
    cudaMemcpy(dev_a, a, N * sizeof(float), cudaMemcpyHostToDevice);
    cudaMemcpy(dev_b, b, N * sizeof(float), cudaMemcpyHostToDevice);
    differenceNorm <<<threadsPerBlock, blocksCount>>>(dev_a, dev_b, dev_c);
    cudaMemcpy(c, dev_c, N * sizeof(float), cudaMemcpyDeviceToHost);
    float mult = 0;
    for (int i = 0; i < blocksCount; i++)
        mult += c[i];
    printf("\nНорма разности векторов: %.0f\n", mult);
    cudaFree(dev_a);
    cudaFree(dev_b);
    cudaFree(dev_c);
    return 0;
}
