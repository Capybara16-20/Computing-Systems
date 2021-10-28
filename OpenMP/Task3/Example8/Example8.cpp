#include <windows.h>
#include <omp.h>
#include <iostream>

int* InputArray(int n)
{
	int* arr = (int*)malloc(sizeof(int) * n);
	for (int i = 0; i < n; i++)
	{
		int value;
		printf("элемент %d: ", i);
		scanf("%d", &value);
		arr[i] = value;
	}

	return arr;
}

void ShowArray(int *arr, int n)
{
	for (int i = 0; i < n; i++)
	{
		printf("%d ", arr[i]);
	}
	printf("\n");
}

int GetMaxElement(int *arr, int n) 
{
	int max = arr[0];
#pragma omp parallel for
	for (int i = 1; i < n; i++)
	{
		if (arr[i] > max)
#pragma omp critical
			if (arr[i] > max)
				max = arr[i];
	}

	return max;
}

int main()
{
	setlocale(LC_ALL, "Rus");

	int n;
	printf("Введите количество элементов массива: ");
	scanf("%d", &n);
	printf("\n");

	printf("Введите значения массива:\n");
	int* arr = InputArray(n);
	printf("\n");

	printf("Массив: ");
	ShowArray(arr, n);
	printf("\n");

	int max = GetMaxElement(arr, n);
	printf("Максимальный элемент массива: %d\n", max);

	return 0;
}

