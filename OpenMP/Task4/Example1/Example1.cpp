#include <omp.h>
#include <iostream>
int main() 
{
	setlocale(LC_ALL, "Rus");

	int n;
	int current = 0;
#pragma omp parallel private(n) shared(current)
	{
		n = omp_get_thread_num();
		int numthreads = omp_get_num_threads();
		while (current != numthreads)
		{
			//printf("Нить проверяется: %d\n", n);
			if (current == n)
			{
				printf("Нить: %d\n", n);
				//printf("current: %d\n", current);
				current++;
				//printf("current увеличено до %d\n", current);
			}
#pragma omp barrier
		}
	}
	return 0;
}

