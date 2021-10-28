#include "cuda_runtime.h"
#include "device_launch_parameters.h"
#include <stdio.h>
#include <locale.h>
#define BS 16//Размер блока
#define BC 4//Количество блоков

__global__ void kern(float* data)
{
    //Создается массив float в разделяемой памяти
    __shared__ float a[BS];
    int idx = blockIdx.x * BS + threadIdx.x;
    //Копируем из глобальной памяти в разделяемую
    a[threadIdx.x] = data[idx];
    //Перед использованием надо быть уверенным,
    //что все данные скопированы
    __syncthreads(); //Синхронизируем
    data[idx] = a[threadIdx.x] + a[(threadIdx.x + 1) % BS]; //Используем
}

int main()
{
    setlocale(LC_ALL, "Rus");
    float data[BC * BS];
    float* dev_data;
    cudaMalloc((void**)&dev_data, BC * BS * sizeof(float));
    for (int i = 0; i < BC * BS; i++)
        data[i] = i;
    cudaMemcpy(dev_data, data, BC * BS * sizeof(float), cudaMemcpyHostToDevice);
    kern <<<BC, BS>>>(dev_data);
    cudaMemcpy(data, dev_data, BC * BS * sizeof(float), cudaMemcpyDeviceToHost);
    for (int i = 0; i < BC * BS; i++)
        printf("%f\n", data[i]);
    cudaFree(dev_data);
    return 0;
}
