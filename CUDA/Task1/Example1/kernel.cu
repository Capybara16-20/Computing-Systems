#include "cuda_runtime.h"
#include "device_launch_parameters.h"
#include <locale.h>
#include <stdio.h>
int main()
{
	cudaDeviceProp prop;
	setlocale(LC_ALL, "RUS");
	int count;
	cudaGetDeviceCount(&count);
	printf("Количество устройств: %d\n\n", count);
	for (int i = 0; i < count; i++)
	{
		cudaGetDeviceProperties(&prop, i);
		printf("- Общая информация об устройстве %d -\n", i);
		printf("	Имя: %s\n", prop.name);
		printf("	Вычислительные возможности: %d.%d\n", prop.major, prop.minor);
		printf("	Тактовая частота: %d\n", prop.clockRate);
		printf("	Перекрытие копирования: ");
		if (prop.deviceOverlap)
			printf("разрешено\n");
		else printf("запрещено\n");
		printf("	Тайм-аут выполнения ядра: ");
		if (prop.kernelExecTimeoutEnabled)
			printf("включен\n\n");
		else
			printf("выключен\n\n");
		printf("- Информация о памяти для устройства %d -\n", i);
		printf("	Всего глобальной памяти: %zu\n", prop.totalGlobalMem);
		printf("	Всего константной памяти: %zu\n", prop.totalConstMem);
		printf("	Максимальный шаг: %zu\n", prop.memPitch);
		printf("	Выравнивание текстур: %zu\n\n", prop.textureAlignment);
		printf("- Инфо о мультипроцессорах для устройства %d -\n", i);
		printf("	Кол-во мультипроцессоров: %d\n", prop.multiProcessorCount);
		printf("	Разделяемая память на один МП: %zu\n", prop.sharedMemPerBlock);
		printf("	Количество регистров на один МП: %d\n", prop.regsPerBlock);
		printf("	Количество нитей в варпе: %d\n", prop.warpSize);
		printf("	Макс.кол-во нитей в блоке: %d\n", prop.maxThreadsPerBlock);
		printf("	Макс.кол-во нитей по измерениям: (%d, %d,% d)\n", prop.maxThreadsDim[0],
			prop.maxThreadsDim[1], prop.maxThreadsDim[2]);
		printf("	Максимальные размеры сетки: (%d, %d, %d)\n", prop.maxGridSize[0],
			prop.maxGridSize[1], prop.maxGridSize[2]);
	}
}

