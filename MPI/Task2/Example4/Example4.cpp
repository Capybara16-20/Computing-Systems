#include "mpi.h"
#include <stdio.h>

int main(int argc, char** argv)
{
	int rank, size;
	float a, b;
	MPI_Status status;
	MPI_Init(&argc, &argv);
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);
	MPI_Comm_size(MPI_COMM_WORLD, &size);
	a = 0.0;
	b = 0.0;

	if (rank % 2 == 0)
	{
		b = (float)rank;
		MPI_Send(&b, 1, MPI_FLOAT, rank + 1, 5, MPI_COMM_WORLD);
		MPI_Recv(&a, 1, MPI_FLOAT, rank + 1, 5, MPI_COMM_WORLD, &status);
		printf("process %d get a = %3.1f from process %d\n", rank, a, rank + 1);
	}
	else
	{
		a = (float)rank;
		MPI_Recv(&b, 1, MPI_FLOAT, rank - 1, 5, MPI_COMM_WORLD, &status);
		MPI_Send(&a, 1, MPI_FLOAT, rank - 1, 5, MPI_COMM_WORLD);
		printf("process %d get b = %3.1f from process %d\n", rank, b, rank - 1);
	}
	MPI_Finalize();
}

