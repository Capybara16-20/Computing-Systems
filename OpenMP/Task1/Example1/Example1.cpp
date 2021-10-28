#include <stdio.h>
#include<locale.h>
int main()
{
	setlocale(LC_ALL, "RUS");
#ifdef _OPENMP
	printf("OpenMP поддерживается!\n");
#endif
	char c = getchar();
	return 0;
}


