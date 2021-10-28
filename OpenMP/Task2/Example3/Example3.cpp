#include <omp.h>
#include <iostream>
int main()
{
	setlocale(LC_ALL, "Rus");
	printf("Значение OMP_DYNAMIC: %d\n", omp_get_dynamic());
		omp_set_dynamic(1);
	printf("Значение OMP_DYNAMIC: %d\n", omp_get_dynamic());
#pragma omp parallel num_threads(128)
	{
#pragma omp master
		{
			printf("Параллельная область, %d потоков\n",
				omp_get_num_threads());
		}
	}
	return 0;
}

