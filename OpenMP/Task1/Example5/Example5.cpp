#include <omp.h>
#include <iostream>
int main() 
{
	setlocale(LC_ALL, "RUS");
	int n = 1;
	printf("Значение n в начале: %d\n", n);
#pragma omp parallel firstprivate(n)
	{
		printf("Значение n на нити (на входе): %d\n", n);
		n = omp_get_thread_num();
		printf("Значение n на нити (на выходе): %d\n", n);
	}
		printf("Значение n в конце: %d\n", n);
	return 0;
}

