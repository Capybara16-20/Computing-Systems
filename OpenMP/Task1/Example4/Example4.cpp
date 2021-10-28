#include <omp.h>
#include <iostream>
int main() 
{
	setlocale(LC_ALL, "RUS");
	int i, m[10];
	printf("Массив m в начале:\n");
	for (i = 0; i < 10; i++) {
		m[i] = 0;
		printf("%d\n", m[i]);
	}
#pragma omp parallel shared(m)
	{
		m[omp_get_thread_num()] = 1;
	}
	printf("Массив m в конце:\n");
	for (i = 0; i < 10; i++) printf("%d\n", m[i]);
	return 0;
}


