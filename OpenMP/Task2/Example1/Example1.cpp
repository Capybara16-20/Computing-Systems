#include <omp.h>
#include <iostream>
int main() 
{
	setlocale(LC_ALL, "Rus");
	int count = 1;
#pragma omp parallel reduction (*: count)
	{
		count++;
		printf("Текущее значение count = %d\n", count);
	}
	printf("Итоговое значение count = %d\n", count);
	return 0;
}



