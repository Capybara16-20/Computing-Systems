#include "mpi.h"
#include <stdio.h>

int main(int* argc, char** argv) 
{
	int size, rank;
	MPI_Status status;
	MPI_Init(argc, &argv);
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);
	MPI_Comm_size(MPI_COMM_WORLD, &size);
	//int k = 8128; // k – исследуемое число
	int k = 28; // k – исследуемое число
	int p, l = 0, x1, l1, l2 = 0, itog = 0, i;
	p = k / (size - 1);
	if (rank != size - 1) // считать все процессы, кроме последнего
	{ 
		for (i = rank * p; i < (rank + 1) * p; i++) // метод перебора делителей
		{
			x1 = k / (i + 1);
			if (x1 * (i + 1) == k)
				l = l + i + 1;
		}
		MPI_Send(&l, 1, MPI_INT, size - 1, 1, MPI_COMM_WORLD);
	}
	else
	{
		for (i = 0; i < size - 1; i++) // последний процесс все собирает
		{
			MPI_Recv(&l1, 1, MPI_INT, i, 1, MPI_COMM_WORLD, &status);
			itog += l1;
		}
		for (i = (size - 1) * p; i < k - 1; i++)
		{
			x1 = k / (i + 1);
			if (x1 * (i + 1) == k)
				l2 = l2 + i + 1;
		}
		itog += l2;
		if (itog == k)
			puts("The number is perfect");
		else
			puts("The number is not perfect");
	}
	MPI_Finalize();
}
