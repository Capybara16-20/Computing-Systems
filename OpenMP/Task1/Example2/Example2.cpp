#include <stdio.h>
#include<locale.h>
#include <omp.h>
int main()
{
	setlocale(LC_ALL, "RUS");
	printf("Последовательная область 1\n");
	//omp_set_num_threads(3);
#pragma omp parallel
	{
		printf("Параллельная область\n");
	}
	printf("Последовательная область 2\n");
	return 0;
}

//Сколько потоков порождается в этом примере? Можно ли изменить
//количество потоков ?