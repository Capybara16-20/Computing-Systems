#include <omp.h>
#include <iostream>
int main() 
{
	system("chcp 1251>nul"); // Изменение кодировки консоли
	int n = 1;
	printf("n в последовательной области (начало): %d\n", n);
#pragma omp parallel private(n)
	{
		n = 1;
		printf("Значение n на нити (на входе): %d\n", n);
		n = omp_get_thread_num();
		printf("Значение n на нити (на выходе): %d\n", n);
	}
	printf("n в последовательной области (конец): %d\n", n);
	system("pause>nul"); // Задержка консольного окна
	return 0;
}


