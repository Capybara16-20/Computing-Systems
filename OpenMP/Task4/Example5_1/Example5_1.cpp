#include <omp.h>
#include <iostream>
#include <windows.h>
omp_lock_t lock;
int main()
{
	setlocale(LC_ALL, "Rus");

	int n;
	int sum = 0;
	printf("Начальная сумма: %d\n", sum);

	omp_init_lock(&lock);
#pragma omp parallel private(n) shared(sum)
	{
		n = omp_get_thread_num();

		omp_set_lock(&lock);
		printf("Нить %d прибавила: %d + %d = ", n, sum, n);
		sum += n;
		printf("%d\n", sum);
		omp_unset_lock(&lock);

	}
	omp_destroy_lock(&lock);

	printf("Конечная сумма: %d\n", sum);
	return 0;
}

