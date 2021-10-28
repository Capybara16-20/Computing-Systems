#include "cuda_runtime.h"
#include "device_launch_parameters.h"
#include <stdio.h>
#include <stdlib.h>
#include <locale.h>
#include <windows.h>
#include <time.h>
#define SIZE (100 * 1024 * 1024)

void* big_random_block(int size)
{
	unsigned char* data = (unsigned char*)malloc(size);
	for (int i = 0; i < size; i++)
		data[i] = rand();
	return data;
}

int main(void) 
{
	setlocale(LC_ALL, "RUS");
	unsigned char* buffer = (unsigned char*)big_random_block(SIZE);
	clock_t start, stop; //объявление переменных для замера времени
	start = clock(); //начало замера
	unsigned int histo[256];
	for (int i = 0; i < 256; i++) 
		histo[i] = 0;
	for (int i = 0; i < SIZE; i++) 
		histo[buffer[i]]++;
	stop = clock(); //окончание замера
	float elapsedTime = (float)(stop - start) / (float)CLOCKS_PER_SEC * 1000.0f;
	printf("Время выполнения: %3.1f миллисекунд\n", elapsedTime);
	long histoCount = 0;
	for (int i = 0; i < 256; i++) 
		histoCount += histo[i];
	printf("Сумма гистограммы: %ld\n", histoCount);
	free(buffer); //освобождение памяти
	return 0;
}
