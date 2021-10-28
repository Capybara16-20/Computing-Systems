#include <omp.h>
#include <iostream>
int main() 
{
	setlocale(LC_ALL, "Rus");
	int n;
#pragma omp parallel private(n)
	{
			n = omp_get_thread_num();
#pragma omp sections
		{
#pragma omp section
			{
				printf("Первая секция, процесс %d\n", n);
			}
#pragma omp section
			{
				printf("Вторая секция, процесс %d\n", n);
			}
#pragma omp section
			{
				printf("Третья секция, процесс %d\n", n);
			}
		}
		printf("Параллельная область, процесс %d\n", n);
	}
	return 0;
}


