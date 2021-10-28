#include <windows.h>
#include <omp.h>
#include <iostream>
int main() 
{
	setlocale(LC_ALL, "Rus");
#pragma omp parallel
	{
#pragma omp for schedule (guided)
		for (int i = 0; i < 20; i++)
		{
			printf("Нить %d выполнила итерацию %d\n",
				omp_get_thread_num(), i);
			Sleep(1);
		}
	}
	return 0;
}

