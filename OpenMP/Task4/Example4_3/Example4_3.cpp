#include <omp.h>
#include <iostream>
int main() 
{
	setlocale(LC_ALL, "Rus");
	int i, n;
#pragma omp parallel private (i, n)
	{
		n = omp_get_thread_num();
#pragma omp for ordered
		for (i = 0; i < 5; i++) 
		{
			printf("Нить %d, итерация %d\n", n, i);
#pragma omp ordered
			{
				printf("ordered: Нить %d, итерация %d\n", n, i);
			}
		}
	}
	return 0;
}

