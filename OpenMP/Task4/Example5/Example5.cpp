#include <omp.h>
#include <iostream>
#include <windows.h>
omp_lock_t lock;
int main()
{
	setlocale(LC_ALL, "Rus");

	int n;
	bool areAllStopped = false;
	int stoppedCount = 1;
	omp_init_lock(&lock);
#pragma omp parallel private(n) shared(areAllStopped, stoppedCount)
	{
		int threadsCount = omp_get_num_threads();
		n = omp_get_thread_num();
		if (omp_test_lock(&lock))
		{
			printf("Нить %d ждёт остальные потоки\n", n);
			while (stoppedCount != threadsCount) {}

			printf("Потоки синхронизированы\n");
			omp_unset_lock(&lock);
			areAllStopped = true;
		}
		else 
		{
			printf("Нить %d остановлена\n", n);
			stoppedCount++;
			while (!areAllStopped) {}
		}
	}
	omp_destroy_lock(&lock);
	return 0;
}

