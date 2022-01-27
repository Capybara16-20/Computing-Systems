#include "Kernel.cuh"
#include "cuda.h"
#include "cuda_runtime.h"
#include "device_launch_parameters.h"
#include <ctime>
const int maxThreads = 512;

__device__ double GetEuclideanDistance(unsigned color1, unsigned color2)
{
	unsigned red1 = color1 & 0x00FF0000 >> 0x10;
	unsigned green1 = color1 & 0x0000FF00 >> 0x8;
	unsigned blue1 = color1 & 0x000000FF;
	unsigned red2 = color2 & 0x00FF0000 >> 0x10;
	unsigned green2 = color2 & 0x0000FF00 >> 0x8;
	unsigned blue2 = color2 & 0x000000FF;

	return (red1 - red2) * (red1 - red2) + (green1 - green2) * (green1 - green2) + (blue1 - blue2) * (blue1 - blue2);
}

__global__ void calculateClusters_kernel(unsigned* src, int width, int height, 
	int clustersCount, int* centers_x, int* centers_y, int* pointsClusters, int* changesCount)
{
	//Для каждой точки
	int i = blockIdx.x * blockDim.x + threadIdx.x;
	if (i < width * height)
	{
		unsigned current_color = src[i];
		int nearestCluster_id = 0;
		unsigned cluster_color = src[centers_y[0] * width + centers_x[0]];
		double minDistance = GetEuclideanDistance(current_color, cluster_color);

		//Ищем ближайший кластер
		for (int k = 1; k < clustersCount; k++)
		{
			cluster_color = src[centers_y[k] * width + centers_x[k]];
			double cl_distance = GetEuclideanDistance(current_color, cluster_color);
			if (cl_distance < minDistance)
			{
				minDistance = cl_distance;
				nearestCluster_id = k;
			}
		}

		//Помещаем точку в ближайший кластер
		int currentCluster = pointsClusters[i];
		if (nearestCluster_id != currentCluster)
		{
			pointsClusters[i] = nearestCluster_id;
			*changesCount++;
		}
	}
}

__global__ void initializePointsClusters_kernel(int* pointsClusters, int width, int height)
{
	int x = blockIdx.x * blockDim.x + threadIdx.x;
	int y = blockIdx.y * blockDim.y + threadIdx.y;
	if (x < width && y < height)
	{
		pointsClusters[y * width + x] = -1;
	}
}

__global__ void initializeColors_kernel(unsigned* colors, int clustersCount)
{
	int i = blockIdx.x * blockDim.x + threadIdx.x;
	if (i < clustersCount)
	{
		colors[i] = 0;
	}
}

__global__ void calculateColors_kernel(unsigned* src, int width, int height, 
	int* pointsClusters, unsigned* colors)
{
	int i = blockIdx.x * blockDim.x + threadIdx.x;
	if (i < width * height)
	{
		unsigned pixel = src[i];
		int cluster = pointsClusters[i];

		//colors[cluster] |= (pixel & 0x00FF0000 >> 0x10 + pixel & 0x0000FF00 >> 0x8 + pixel & 0x000000FF);
		colors[cluster] |= pixel;
	}
}

__global__ void fillColors_kernel(unsigned* colors, int clustersCount)
{
	int i = blockIdx.x * blockDim.x + threadIdx.x;
	if (i < clustersCount)
	{
		//colors[i] = 0xFF000000 | (colors[i] << 0x10) | (colors[i] << 0x8) | colors[i];
		colors[i] = 0xFF000000 | colors[i];
	}
}

__global__ void paint_kernel(unsigned* src, int width, int height, unsigned* colors, int* pointsClusters)
{
	int i = blockIdx.x * blockDim.x + threadIdx.x;
	if (i < width * height)
	{
		src[i] = colors[pointsClusters[i]];
	}
}

int calc(unsigned* srcImage, int width, int height, int clustersCount)
{
	srand(time(NULL));

	//Инициализируем кластеры точек, цвета и количество точек по кластерам
#pragma region
	int* pointsClusters = new int[width * height];
	int* dev_pointsClusters;
	cudaMalloc((void**)&dev_pointsClusters, sizeof(int) * width * height);
	cudaMemcpy(dev_pointsClusters, pointsClusters, sizeof(int) * width * height, cudaMemcpyHostToDevice);

	dim3 threads;
	dim3 blocks;
	if (width * height < maxThreads)
	{
		threads = dim3(width, height);
		blocks = dim3();
	}
	else
	{
		threads = dim3(maxThreads, maxThreads);
		blocks = dim3((width + maxThreads - 1) / maxThreads, (height + maxThreads - 1) / maxThreads);
	}

	initializePointsClusters_kernel <<<blocks, threads>>> (dev_pointsClusters, width, height);
	cudaMemcpy(pointsClusters, dev_pointsClusters, sizeof(int) * width * height, cudaMemcpyDeviceToHost);

	cudaFree(dev_pointsClusters);
#pragma endregion

	//Получаем случайные кластеры
#pragma region
	int* centers_x = new int[clustersCount];
	int* centers_y = new int[clustersCount];
	for (int i = 0; i < clustersCount; i++)
	{
		centers_x[i] = rand() % width;
		centers_y[i] = rand() % height;
	}

	int blockSize;
	int gridSize;
	if (width * height < maxThreads)
	{
		blockSize = width * height;
		gridSize = 1;
	}
	else
	{
		blockSize = maxThreads;
		gridSize = (width * height) / maxThreads + 1;
	}
	threads = dim3(blockSize);
	blocks = dim3(gridSize);

	unsigned* dev_srcImage;
	cudaMalloc((void**)&dev_srcImage, sizeof(unsigned) * width * height);
	cudaMemcpy(dev_srcImage, srcImage, sizeof(unsigned) * width * height, cudaMemcpyHostToDevice);
	bool complited = false;
	while (!complited) //Пока не прекратятся изменения
	{
		int changesCount = 0;
		int* dev_changesCount;
		cudaMalloc((void**)&dev_changesCount, sizeof(int));
		cudaMemcpy(dev_changesCount, &changesCount, sizeof(int), cudaMemcpyHostToDevice);
		
		cudaMalloc((void**)&dev_pointsClusters, sizeof(int) * width * height);
		cudaMemcpy(dev_pointsClusters, pointsClusters, sizeof(int) * width * height, cudaMemcpyHostToDevice);

		int* dev_centers_x;
		cudaMalloc((void**)&dev_centers_x, sizeof(int) * clustersCount);
		cudaMemcpy(dev_centers_x, centers_x, sizeof(int) * clustersCount, cudaMemcpyHostToDevice);
		int* dev_centers_y;
		cudaMalloc((void**)&dev_centers_y, sizeof(int) * clustersCount);
		cudaMemcpy(dev_centers_y, centers_y, sizeof(int) * clustersCount, cudaMemcpyHostToDevice);

		calculateClusters_kernel <<<blocks, threads>>> (dev_srcImage, width, height, clustersCount, 
			dev_centers_x, dev_centers_y, dev_pointsClusters, dev_changesCount);
		cudaMemcpy(centers_x, dev_centers_x, sizeof(int) * clustersCount, cudaMemcpyDeviceToHost);
		cudaMemcpy(centers_y, dev_centers_y, sizeof(int) * clustersCount, cudaMemcpyDeviceToHost);
		cudaMemcpy(pointsClusters, dev_pointsClusters, sizeof(int) * width * height, cudaMemcpyDeviceToHost);
		cudaMemcpy(&changesCount, dev_changesCount, sizeof(int), cudaMemcpyDeviceToHost);

		cudaFree(dev_centers_x);
		cudaFree(dev_centers_y);
		cudaFree(dev_pointsClusters);
		cudaFree(dev_changesCount);

		if (changesCount == 0) //Если изменений не было
			complited = true; //то точки распределены по кластерам
	}
	cudaFree(dev_srcImage);
#pragma endregion

	//Вычисляем цвета каждого кластера
#pragma region
	//Инициализация цветов
	if (clustersCount < maxThreads)
	{
		blockSize = clustersCount;
		gridSize = 1;
	}
	else
	{
		blockSize = maxThreads;
		gridSize = clustersCount / maxThreads + 1;
	}
	threads = dim3(blockSize);
	blocks = dim3(gridSize);

	unsigned* colors = new unsigned[clustersCount];
	unsigned* dev_colors;
	cudaMalloc((void**)&dev_colors, sizeof(unsigned) * clustersCount);

	initializeColors_kernel <<<blocks, threads>>> (dev_colors, clustersCount);
	cudaMemcpy(colors, dev_colors, sizeof(unsigned) * clustersCount, cudaMemcpyDeviceToHost);
	
	//Вычисление цветов
	if (width * height < maxThreads)
	{
		blockSize = width * height;
		gridSize = 1;
	}
	else
	{
		blockSize = maxThreads;
		gridSize = (width * height) / maxThreads + 1;
	}
	threads = dim3(blockSize);
	blocks = dim3(gridSize);

	cudaMalloc((void**)&dev_colors, sizeof(unsigned) * clustersCount);

	cudaMalloc((void**)&dev_pointsClusters, sizeof(int) * width * height);
	cudaMemcpy(dev_pointsClusters, pointsClusters, sizeof(int) * width * height, cudaMemcpyHostToDevice);

	cudaMalloc((void**)&dev_srcImage, sizeof(unsigned) * width * height);
	cudaMemcpy(dev_srcImage, srcImage, sizeof(unsigned) * width * height, cudaMemcpyHostToDevice);

	calculateColors_kernel <<<blocks, threads>>> (dev_srcImage, width, height,
		dev_pointsClusters, dev_colors);
	cudaMemcpy(colors, dev_colors, sizeof(unsigned) * clustersCount, cudaMemcpyDeviceToHost);

	if (clustersCount < maxThreads)
	{
		blockSize = clustersCount;
		gridSize = 1;
	}
	else
	{
		blockSize = maxThreads;
		gridSize = clustersCount / maxThreads + 1;
	}
	threads = dim3(blockSize);
	blocks = dim3(gridSize);

	cudaMalloc((void**)&dev_colors, sizeof(unsigned) * clustersCount);
	cudaMemcpy(dev_colors, colors, sizeof(unsigned)* clustersCount, cudaMemcpyHostToDevice);

	fillColors_kernel <<<blocks, threads>>> (dev_colors, clustersCount);
	cudaMemcpy(colors, dev_colors, sizeof(unsigned)* clustersCount, cudaMemcpyDeviceToHost);

	cudaFree(dev_srcImage);
	cudaFree(dev_pointsClusters);
	cudaFree(dev_colors);
#pragma endregion

	//Перекраска исходного изображения
#pragma region
	if (width * height < maxThreads)
	{
		blockSize = width * height;
		gridSize = 1;
	}
	else
	{
		blockSize = maxThreads;
		gridSize = (width * height) / maxThreads + 1;
	}
	threads = dim3(blockSize);
	blocks = dim3(gridSize);

	cudaMalloc((void**)&dev_srcImage, sizeof(unsigned)* width* height);
	cudaMemcpy(dev_srcImage, srcImage, sizeof(unsigned)* width* height, cudaMemcpyHostToDevice);

	cudaMalloc((void**)&dev_colors, sizeof(unsigned)* clustersCount);
	cudaMemcpy(dev_colors, colors, sizeof(unsigned)* clustersCount, cudaMemcpyHostToDevice);

	cudaMalloc((void**)&dev_pointsClusters, sizeof(int)* width* height);
	cudaMemcpy(dev_pointsClusters, pointsClusters, sizeof(int)* width* height, cudaMemcpyHostToDevice);

	paint_kernel <<<blocks, threads>>> (dev_srcImage, width, height, dev_colors, dev_pointsClusters);
	cudaMemcpy(srcImage, dev_srcImage, sizeof(unsigned)* width* height, cudaMemcpyDeviceToHost);

	cudaFree(dev_srcImage);
	cudaFree(dev_colors);
	cudaFree(dev_pointsClusters);
#pragma endregion

	return 0;
}


