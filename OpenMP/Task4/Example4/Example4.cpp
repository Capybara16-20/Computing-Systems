#include <windows.h>
#include <omp.h>
#include <iostream>
#include <cmath>
omp_lock_t lock;

void ShowText(const char text[], bool isBreak)
{
	printf("%s", text);
	if (isBreak)
		printf("\n");
}

void ShowDouble(double number, bool isBreak)
{
	printf("%lf", number);
	if (isBreak)
		printf("\n");
}

int InputNumber(const char text[])
{
	int n;
	ShowText(text, false);
	scanf("%d", &n);

	return n;
}

double CalculatePi(int n)
{
	double result = 0;
	omp_init_lock(&lock);
#pragma omp parallel for
	for (int i = 0; i < n; i++)
	{
		int numerator = pow(-1.0, i);
		int denominator = (2 * i + 1);

		omp_set_lock(&lock);
		result += (double)numerator / denominator;
		omp_unset_lock(&lock);
	}
	omp_destroy_lock(&lock);

	return result * 4;
}

int main()
{
	setlocale(LC_ALL, "Rus");

	int n = InputNumber("Введите количество элементов ряда: ");

	double pi = 0;
	pi = CalculatePi(n);

	ShowText("Полученное значение числа Pi: ", false);
	ShowDouble(pi, true);

	return 0;
}

