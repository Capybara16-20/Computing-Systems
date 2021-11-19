#include "mpi.h"
#include <stdio.h>
#include <malloc.h>

int main(int argc, char** argv)
{
	int rank;
	float a, b;
	MPI_Status status;
	MPI_Init(&argc, &argv);
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);
	a = 0.0;
	b = 0.0;
	if (rank == 0)
	{
		b = 3.0;
		int BUFSIZE = sizeof(float) + MPI_BSEND_OVERHEAD;
		float* buffer = (float*)malloc(BUFSIZE);
		MPI_Buffer_attach(buffer, BUFSIZE);
		MPI_Bsend(&b, 1, MPI_FLOAT, 1, 5, MPI_COMM_WORLD);
		MPI_Buffer_detach(buffer, &BUFSIZE);
		free(buffer);
		MPI_Recv(&a, 1, MPI_FLOAT, 1, 5, MPI_COMM_WORLD, &status);
	}
	if (rank == 1)
	{
		MPI_Recv(&b, 1, MPI_FLOAT, 0, 5, MPI_COMM_WORLD, &status);
		a = 5.0;
		MPI_Send(&a, 1, MPI_FLOAT, 0, 5, MPI_COMM_WORLD);
	}
	printf("process %d a = %3.1f, b = %3.1f\n", rank, a, b);
	MPI_Finalize();
}
