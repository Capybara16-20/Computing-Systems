#include <omp.h>
#include <iostream>
int main() 
{
	setlocale(LC_ALL, "Rus");
	int n;
#pragma omp parallel private(n)
	{
		n = 1;
#pragma omp master
		{
			n = 2;
		}
		printf("Первое значение n: %d\n", n);
#pragma omp barrier
#pragma omp master
		{
			n = 3;
		}
		printf("Второе значение n: %d\n", n);
	}
	return 0;
}

