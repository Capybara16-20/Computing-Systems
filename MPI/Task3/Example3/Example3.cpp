#include <stdio.h>
#include <mpi.h>
double Fact(int n)// функция вычисления факториала
{
	if (n == 0)
		return 1;
	else
		return n * Fact(n - 1);
}
int main(int argc, char* argv[])
{
	int n = 100; //изменяемое пользователем значение
	int myid; // номер процесса
	int numprocs; // общее число процессов
	int i;
	int rc;
	long double drob, drobSum = 0, Result, sum;
	double startwtime = 0.0;
	double endwtime;
	/* выполняется инициализация MPI, если функция возвращает значение
	ошибки, выводится соответствующее сообщение и уничтожается среда
	выполнения MPI */
	if (rc = MPI_Init(&argc, &argv))
	{
		printf("Ошибка запуска, выполнение остановлено\n");
		MPI_Abort(MPI_COMM_WORLD, rc);
	}
	MPI_Comm_size(MPI_COMM_WORLD, &numprocs);
	MPI_Comm_rank(MPI_COMM_WORLD, &myid);
	// запуск таймера, если был инициализирован главный процесс
	if (myid == 0)
		startwtime = MPI_Wtime();
	/* широковещательная рассылка значения n всем процессам, входящим в
	коммуникатор MPI_COMM_WORLD */
	MPI_Bcast(&n, 1, MPI_INT, 0, MPI_COMM_WORLD);
	/* цикл выполняется каждым процессом, сумма слагаемых записывается
	в переменную drobSum */
	for (i = myid; i <= n; i += numprocs)
	{
		drob = 1 / Fact(i);
		drobSum += drob;
	}
	// объединение частичных сумм в переменной Result
	MPI_Reduce(&drobSum, &Result, 1, MPI_LONG_DOUBLE, MPI_SUM, 0, MPI_COMM_WORLD);
	if (myid == 0)
	{
		printf("%1.20f\n", Result); //форматированный вывод результата
		endwtime = MPI_Wtime();
		printf("%1.20f\n", (endwtime - startwtime) * 1000);
	}
	MPI_Finalize();
}
