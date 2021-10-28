#include <omp.h>
#include <iostream>
void mode(void) 
{
	if (omp_in_parallel()) printf("Параллельная область\n");
	else printf("Последовательная область\n");
}
int main() 
{
	setlocale(LC_ALL, "Rus");
	mode();
#pragma omp parallel
	{
		mode();
#pragma omp master
		{
			mode();
		}
	}
	return 0;
}

