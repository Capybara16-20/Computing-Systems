#include <stdio.h>
#include "mpi.h"
int main(int argc, char** argv)
{
	int rank, size, prev, next;
	int buf[2];
	MPI_Request reqs[4];
	MPI_Status stats[4];
	//инициализация MPI
	MPI_Init(&argc, &argv);
	//определение количества процессов
	MPI_Comm_size(MPI_COMM_WORLD, &size);
	//получение номера текущего процесса
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);
	// вычисление номеров предыдущего и следующего процессов
	prev = rank - 1;
	next = rank + 1;
	if (rank == 0) prev = size - 1;
	if (rank == size - 1) next = 0;
	//прием-передача сообщений
	MPI_Irecv(&buf[0], 1, MPI_INT, prev, 5, MPI_COMM_WORLD, &reqs[0]);
	MPI_Irecv(&buf[1], 1, MPI_INT, next, 6, MPI_COMM_WORLD, &reqs[1]);
	MPI_Isend(&rank, 1, MPI_INT, prev, 6, MPI_COMM_WORLD, &reqs[2]);
	MPI_Isend(&rank, 1, MPI_INT, next, 5, MPI_COMM_WORLD, &reqs[3]);
	//ожидание завершения всех запросов
	MPI_Waitall(4, reqs, stats);
	printf("process %d prev = %d next=%d\n", rank, buf[0], buf[1]);
	//уничтожение среды MPI
	MPI_Finalize();
}

