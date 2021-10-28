#include <omp.h>
#include <iostream>
int main()
{
	setlocale(LC_ALL, "Rus");
	//omp_set_num_threads(1606);
	omp_set_num_threads(120000);
#pragma omp parallel
	{
		printf("Поток n: %d\n", omp_get_thread_num());
	}
	printf("%d\n", omp_get_max_threads());
	return 0;
}