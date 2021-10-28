#include <windows.h>
#include <omp.h>
#include <iostream>

int* InputArray(int n)
{
	int *arr = (int*)malloc(sizeof(int) * n);
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

int CalculateMultiplication(int *a, int *b, int n)
{
	int result = 0;
	int sum = 0;
#pragma omp parallel for firstprivate (sum)
		for (int i = 0; i < n; i++)
		{
			sum += a[i] * b[i];

#pragma omp atomic
			result += sum;
		}

	return result;
}

int main()
{
	setlocale(LC_ALL, "Rus");

	int n;
	printf("Введите количество элементов массива: ");
	scanf("%d", &n);
	printf("\n");

	printf("Массив A:\n");
	int* arrayA = InputArray(n);
	printf("\n");
	printf("Массив B:\n");
	int* arrayB = InputArray(n);
	printf("\n");

	printf("Массив A: ");
	ShowArray(arrayA, n);
	printf("Массив B: ");
	ShowArray(arrayB, n);
	printf("\n");

	int result = CalculateMultiplication(arrayA, arrayB, n);
	printf("Результат умножения: %d\n", result);

	return 0;
}

