#include <omp.h>
#include <iostream>
#include <windows.h>
omp_lock_t lock;
int main()
{
	setlocale(LC_ALL, "Rus");
	int n;
	omp_init_lock(&lock);
#pragma omp parallel private (n)
	{
		n = omp_get_thread_num();
		omp_set_lock(&lock);
		printf("Начало закрытой секции, нить %d\n", n);
		Sleep(5);
		printf("Конец закрытой секции, нить %d\n", n);
		omp_unset_lock(&lock);
	}
	omp_destroy_lock(&lock);
	return 0;
}

