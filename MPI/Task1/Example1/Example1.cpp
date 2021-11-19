#include <iostream>
#include <stdio.h>
#include <mpi.h>

int main(int* argc, char** argv)
{
    int numtasks, rank;
    // инициализация MPI-библиотеки
    MPI_Init(argc, &argv);

    // узнаем ранг процесса
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    // определяем количество процессов
    MPI_Comm_size(MPI_COMM_WORLD, &numtasks);

    printf("Hello, MPI from process = %d number of processes: %d\n", rank, numtasks);

    // закрываем MPI-библиотеку
    MPI_Finalize();
}

