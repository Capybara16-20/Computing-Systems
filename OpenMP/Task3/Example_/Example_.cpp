#include <windows.h>
#include <omp.h>
#include <iostream>

int main()
{
	setlocale(LC_ALL, "Rus");
	omp_set_nested(1);

#pragma omp parallel
	{
		printf("Внешняя область. Нить: %d, кол-во потоков: %d\n", omp_get_thread_num(), omp_get_num_threads());

#pragma omp parallel num_threads(6)
		{
			printf("Внутренняя область. Нить: %d, кол-во потоков: %d\n", omp_get_thread_num(), omp_get_num_threads());
		}

		printf("Внешняя область. Нить: %d, кол-во потоков: %d\n", omp_get_thread_num(), omp_get_num_threads());
	}

	return 0;
}

