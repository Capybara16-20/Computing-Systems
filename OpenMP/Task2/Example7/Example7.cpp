#include <omp.h>
#include <iostream>
int main() 
{
	setlocale(LC_ALL, "Rus");
		int n;
#pragma omp parallel private(n)
	{
		n = omp_get_thread_num();
		printf("Значение n (начало): %d\n", n);
#pragma omp single copyprivate(n)
		{
			n = 100;
		}
		printf("Значение n (конец): %d\n", n);
	}
	return 0;
}

