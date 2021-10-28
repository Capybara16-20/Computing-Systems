#include <omp.h>
#include <iostream>
int n;
#pragma omp threadprivate(n)
int main() 
{
	setlocale(LC_ALL, "Rus");
	n = 1;
#pragma omp parallel copyin(n)
	{
		printf("Значение n: %d\n", n);
	}
	return 0;
}


