#include <omp.h>
#include <iostream>
int main() 
{
	setlocale(LC_ALL, "Rus");
#pragma omp parallel
	{
		printf("Сообщение 1\n");
#pragma omp single nowait
		{
			printf("Одна нить\n");
		}
		printf("Сообщение 2\n");
	}
	return 0;
}

