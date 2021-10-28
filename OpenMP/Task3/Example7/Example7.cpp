#include <windows.h>
#include <omp.h>
#include <iostream>

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

double InputDouble(const char text[])
{
	double n;
	ShowText(text, false);
	scanf("%lf", &n);

	return n;
}

struct Vector
{
	double x;
	double y;
	double z;
};

struct Vector InputVector() 
{
	struct Vector vector;
	vector.x = InputDouble("Введите координату x: ");
	vector.y = InputDouble("Введите координату y: ");
	vector.z = InputDouble("Введите координату z: ");

	return vector;
}

double CalculateMultiplication(struct Vector a, struct Vector b)
{
	omp_set_num_threads(3);

	double result = 0;
#pragma omp parallel reduction(+: result)
	{
#pragma omp sections
		{
#pragma omp section
			{
				result += a.x * b.x;
			}
#pragma omp section
			{
				result += a.y * b.y;
			}
#pragma omp section
			{
				result += a.z * b.z;
			}
		}
	}

	return result;
}

int main()
{
	setlocale(LC_ALL, "Rus");
	ShowText("Вектор A:", true);
	struct Vector vectorA = InputVector();

	ShowText("Вектор B:", true);
	struct Vector vectorB = InputVector();

	double multiplication = CalculateMultiplication(vectorA, vectorB);
	ShowText("Полученное значение: ", false);
	ShowDouble(multiplication, true);

	return 0;
}

