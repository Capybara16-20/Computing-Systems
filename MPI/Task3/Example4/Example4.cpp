#include <stdio.h>
#include <mpi.h>
int main(int argc, char* argv[])
{
	int n = 1000000;
	int rank;
	int size;
	int i;
	int rc;
	long double term, termSum = 0, result;
	double startwtime = 0.0;
	double endwtime;
	if (rc = MPI_Init(&argc, &argv))
	{
		printf("Ошибка запуска, выполнение остановлено\n");
		MPI_Abort(MPI_COMM_WORLD, rc);
	}
	MPI_Comm_size(MPI_COMM_WORLD, &size);
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);
	if (rank == 0)
		startwtime = MPI_Wtime();
	MPI_Bcast(&n, 1, MPI_INT, 0, MPI_COMM_WORLD);
	for (i = rank; i <= n; i += size)
	{
		term = (long double)(1.0 / (2.0 * i + 1));
		if (i % 2 != 0)
			term = -term;
		termSum += term;
	}
	MPI_Reduce(&termSum, &result, 1, MPI_LONG_DOUBLE, MPI_SUM, 0, MPI_COMM_WORLD);
	if (rank == 0)
	{
		printf("%1.20f\n", result * 4);
		endwtime = MPI_Wtime();
		printf("%1.20f\n", (endwtime - startwtime) * 1000);
	}
	MPI_Finalize();
}

