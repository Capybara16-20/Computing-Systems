#include "Kernel.cuh"
#include "cuda.h"
#include "cuda_runtime.h"
#include "device_launch_parameters.h"
#include <ctime>
#include <curand.h>
#include <curand_kernel.h>

const int maxThreads = 128;
const int maxCount = 1024;

__global__ void getGrayImage_kernel(unsigned* src, int width, int height)
{
	int x = blockIdx.x * blockDim.x + threadIdx.x;
	int y = blockIdx.y * blockDim.y + threadIdx.y;
	if ((x < width) && (y < height))
	{
		unsigned pixel = src[y * width + x];
		unsigned gray = ((pixel & 0x00FF0000 >> 0x10) +
			(pixel & 0x0000FF00 >> 0x8) + (pixel & 0x000000FF)) / 3;
		src[y * width + x] = 0xFF000000 | (gray << 0x10) | (gray << 0x8) | gray;
	}
}

__global__ void getRandomNumbers_kernel(unsigned int seed, curandState_t* states, 
	int iterations, int pointsCount, int coordsCount, int maxX, int maxY, int* coords)
{
	int i = blockIdx.x * blockDim.x + threadIdx.x;
	if (i < iterations * pointsCount * coordsCount)
	{
		curand_init(seed, i, 0, &states[i]);
		if (i % 2 == 0)
			coords[i] = curand(&states[i]) % maxX;
		else
			coords[i] = curand(&states[i]) % maxY;
	}
}

__global__ void getCoords(int iterations, int pointsCount, int coordsCount, int* coords, int* xCoords, int* yCoords)
{
	int tid = blockIdx.x * blockDim.x + threadIdx.x;
	int i = tid * coordsCount;
	xCoords[tid] = coords[i];
	yCoords[tid] = coords[i + 1];
}

__global__ void getFunctionParameters(int x, int y, unsigned* main, int mainWidth, 
	int mainHeight, unsigned* sub, int subWidth, int subHeight, 
	long double* numerators, long double* subMultipliers, long double* mainMultipliers)
{
	/*int subX = blockIdx.x * blockDim.x + threadIdx.x;
	int subY = blockIdx.y * blockDim.y + threadIdx.y;

	unsigned subPixel = sub[subY * subWidth + subX];
	float subBrightness = 0.299 * ((subPixel & 0x00ff0000) >> 16)
		+ 0.587 * ((subPixel & 0x0000ff00) >> 8) + 0.114 * (subPixel & 0x000000ff);

	unsigned mainPixel = main[(y + subY) * mainWidth + x + subX];
	float mainBrightness = 0.299 * ((mainPixel & 0x00ff0000) >> 16)
		+ 0.587 * ((mainPixel & 0x0000ff00) >> 8) + 0.114 * (mainPixel & 0x000000ff);

	numerators[subY * subWidth + subX] = subBrightness * mainBrightness;
	subMultipliers[subY * subWidth + subX] = subBrightness * subBrightness;
	mainMultipliers[subY * subWidth + subX] = mainBrightness * mainBrightness;*/

	__shared__ long double temp_numerators[maxThreads];
	__shared__ long double temp_subMultipliers[maxThreads];
	__shared__ long double temp_mainMultipliers[maxThreads];
	int tid = threadIdx.x;
	int i = blockIdx.x * blockDim.x + threadIdx.x;
	long double temp_numerator = 0;
	long double temp_subMultiplier = 0;
	long double temp_mainMultiplier = 0;
	while (i < subWidth * subHeight)
	{
		unsigned subPixel = sub[blockIdx.x * blockDim.x + threadIdx.x];
		long double subBrightness = 0.299 * ((subPixel & 0x00ff0000) >> 16)
			+ 0.587 * ((subPixel & 0x0000ff00) >> 8) + 0.114 * (subPixel & 0x000000ff);

		unsigned mainPixel = main[(blockIdx.x + y) * blockDim.x + threadIdx.x + x];
		long double mainBrightness = 0.299 * ((mainPixel & 0x00ff0000) >> 16)
			+ 0.587 * ((mainPixel & 0x0000ff00) >> 8) + 0.114 * (mainPixel & 0x000000ff);

		temp_numerator += subBrightness * mainBrightness;
		temp_subMultiplier += subBrightness * subBrightness;
		temp_mainMultiplier += mainBrightness * mainBrightness;

		i += blockDim.x * gridDim.x;
	}
	temp_numerators[tid] = temp_numerator;
	temp_subMultipliers[tid] = temp_subMultiplier;
	temp_mainMultipliers[tid] = temp_mainMultiplier;
	__syncthreads();
	for (int s = 1; s < blockDim.x; s *= 2)
	{
		//проверuть, участвует лu нить на данном шаге
		if (tid % (2 * s) == 0)
		{
			temp_numerators[tid] += temp_numerators[tid + s];
			temp_subMultipliers[tid] += temp_subMultipliers[tid + s];
			temp_mainMultipliers[tid] += temp_mainMultipliers[tid + s];
		}
		__syncthreads();
	}
	if (tid == 0)
	{
		numerators[blockIdx.x] = temp_numerators[0];
		subMultipliers[blockIdx.x] = temp_subMultipliers[0];
		mainMultipliers[blockIdx.x] = temp_mainMultipliers[0];
	}
}

int getGrayImage(unsigned* srcImage, int width, int height)
{
	unsigned* dev_srcImage;
	size_t size = sizeof(unsigned) * width * height;
	if (cudaMalloc((void**)&dev_srcImage, size) != cudaError::cudaSuccess)
		return -1;
	if (cudaMemcpy(dev_srcImage, srcImage, size,
		cudaMemcpyHostToDevice) != cudaError::cudaSuccess)
		return -1;

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

	getGrayImage_kernel <<<blocks, threads>>> (dev_srcImage, width, height);

	if (cudaMemcpy(srcImage, dev_srcImage, size, 
		cudaMemcpyDeviceToHost) != cudaError::cudaSuccess)
		return -1;

	cudaFree(dev_srcImage);
	return 0;
}

int findImage(unsigned* mainImage, int mainWidth, int mainHeight,
	unsigned* subImage, int subWidth, int subHeight,
	double reflection, double compression, double stretch,
	double accuracy, int iterations, int &found_x, int &found_y)
{
	const int pointsCount = 3;
	const int coordsCount = 2;
	const int bestIndex = 0;
	const int middleIndex = 1;
	const int worstIndex = 2;

	//Получение рандомных координат x и y
#pragma region
	int maxX = mainWidth - subWidth;
	int maxY = mainHeight - subHeight;

	int blockSize;
	int gridSize;
	if (iterations * pointsCount * coordsCount < maxThreads)
	{
		blockSize = iterations * pointsCount * coordsCount;
		gridSize = 1;
	}
	else
	{
		blockSize = maxThreads;
		gridSize = (iterations * pointsCount * coordsCount) / maxThreads + 1;
	}
	dim3 threads(blockSize);
	dim3 blocks(gridSize);

	curandState_t* states;
	cudaMalloc((void**)&states, sizeof(curandState_t) * iterations * pointsCount * coordsCount);
	int* coords = new int[iterations * pointsCount * coordsCount];
	int* dev_coords;
	cudaMalloc((void**)&dev_coords, sizeof(int) * iterations * pointsCount * coordsCount);

	getRandomNumbers_kernel <<<blocks, threads>>> (time(0), states, iterations, pointsCount, coordsCount, maxX, maxY, dev_coords);
	cudaMemcpy(coords, dev_coords, sizeof(int) * iterations * pointsCount * coordsCount, cudaMemcpyDeviceToHost);

#pragma endregion

	//Разделение координат на x и y
#pragma region
	if (iterations * pointsCount < maxThreads)
	{
		blockSize = iterations * pointsCount;
		gridSize = 1;
	}
	else
	{
		blockSize = maxThreads;
		gridSize = (iterations * pointsCount) / maxThreads + 1;
	}
	threads = dim3(blockSize);
	blocks = dim3(gridSize);

	int* xCoords = new int[iterations * pointsCount];
	int* dev_xCoords;
	cudaMalloc((void**)&dev_xCoords, sizeof(int) * iterations * pointsCount);
	int* yCoords = new int[iterations * pointsCount];
	int* dev_yCoords;
	cudaMalloc((void**)&dev_yCoords, sizeof(int) * iterations * pointsCount);
	cudaMalloc((void**)&dev_coords, sizeof(int) * iterations * pointsCount * coordsCount);
	cudaMemcpy(dev_coords, coords, sizeof(int) * iterations * pointsCount * coordsCount, cudaMemcpyHostToDevice);

	getCoords <<<threads, blocks>>> (iterations, pointsCount, coordsCount, dev_coords, dev_xCoords, dev_yCoords);
	cudaMemcpy(xCoords, dev_xCoords, sizeof(int) * iterations * pointsCount, cudaMemcpyDeviceToHost);
	cudaMemcpy(yCoords, dev_yCoords, sizeof(int) * iterations * pointsCount, cudaMemcpyDeviceToHost);

	cudaFree(states);
	cudaFree(dev_coords);
	cudaFree(dev_xCoords);
	cudaFree(dev_yCoords);
#pragma endregion

	//Получение значений функции для всех точек
#pragma region
	if (subWidth * subHeight < maxThreads)
	{
		threads = dim3(subWidth * subHeight);
		blocks = dim3();
	}
	else
	{
		threads = dim3(maxThreads);
		blocks = dim3((subWidth * subHeight + maxThreads - 1) / maxThreads);
	}

	unsigned* dev_mainImage;
	cudaMalloc((void**)&dev_mainImage, sizeof(unsigned) * mainWidth * mainHeight);
	cudaMemcpy(dev_mainImage, mainImage, sizeof(unsigned) * mainWidth * mainHeight, cudaMemcpyHostToDevice);

	unsigned* dev_subImage;
	cudaMalloc((void**)&dev_subImage, sizeof(unsigned) * subWidth * subHeight);
	cudaMemcpy(dev_subImage, subImage, sizeof(unsigned) * subWidth * subHeight, cudaMemcpyHostToDevice);

	long double* functions = new long double[iterations * pointsCount];
	for (int i = 0; i < iterations * pointsCount; i++)
	{
		long double* numerators = new long double[subWidth * subHeight];
		long double* dev_numerators;
		cudaMalloc((void**)&dev_numerators, sizeof(long double) * blocks.x);
		long double* subMultipliers = new long double[blocks.x];
		long double* dev_subMultipliers;
		cudaMalloc((void**)&dev_subMultipliers, sizeof(long double) * blocks.x);
		long double* mainMultipliers = new long double[blocks.x];
		long double* dev_mainMultipliers;
		cudaMalloc((void**)&dev_mainMultipliers, sizeof(long double) * blocks.x);

		int x = xCoords[i];
		int y = yCoords[i];

		getFunctionParameters <<<blocks, threads>>> (xCoords[i], yCoords[i], dev_mainImage,
			mainWidth, mainHeight, dev_subImage, subWidth, subHeight, dev_numerators, 
			dev_subMultipliers, dev_mainMultipliers);

		cudaMemcpy(numerators, dev_numerators, sizeof(long double) * blocks.x, cudaMemcpyDeviceToHost);
		cudaMemcpy(subMultipliers, dev_subMultipliers, sizeof(long double) * blocks.x, cudaMemcpyDeviceToHost);
		cudaMemcpy(mainMultipliers, dev_mainMultipliers, sizeof(long double) * blocks.x, cudaMemcpyDeviceToHost);

		long double numerator = 0;
		long double subMultiplier = 0;
		long double mainMultiplier = 0;
		for (int i = 0; i < blocks.x; i++)
		{
			numerator += numerators[i];
			subMultiplier += subMultipliers[i];
			mainMultiplier += mainMultipliers[i];
		}

		long double function = numerator / (sqrt(subMultiplier * mainMultiplier));
		functions[i] = function;

		cudaFree(dev_numerators);
		cudaFree(dev_subMultipliers);
		cudaFree(dev_mainMultipliers);
	}
	cudaFree(dev_mainImage);
	cudaFree(dev_subImage);
#pragma endregion

	//Проведение итераций для всех наборов точек
	long double found_f = 0;
	int bestX, bestY, midX, midY, worstX, worstY;
	long double bestF, midF, worstF;
	for (int i = 0; i < iterations; i++)
	{
		int p_xCoords[pointsCount];
		int p_yCoords[pointsCount];
		long double funcs[pointsCount];
		for (int j = 0; j < pointsCount; j++)
		{
			p_xCoords[j] = xCoords[i * pointsCount + j];
			p_yCoords[j] = yCoords[i * pointsCount + j];
			funcs[j] = functions[i * pointsCount + j];
		}

		bool isFound = false;
		int count = 0;
		while (!isFound)
		{
			SortCoords(p_xCoords, p_yCoords, funcs, pointsCount);
			bestX = p_xCoords[bestIndex];
			bestY = p_yCoords[bestIndex];
			bestF = funcs[bestIndex];
			midX = p_xCoords[middleIndex];
			midY = p_yCoords[middleIndex];
			midF = funcs[middleIndex];
			worstX = p_xCoords[worstIndex];
			worstY = p_yCoords[worstIndex];
			worstF = funcs[worstIndex];



			break;//
		}
	}


	found_x = bestX;
	found_y = bestY;



	return 0;
}

void SortCoords(int* xCoords, int* yCoords, long double* functions, int pointsCount)
{
	for (int i = 0; i < pointsCount; i++)
	{
		for (int j = i + 1; j < pointsCount; j++)
		{
			if (functions[i] < functions[j])
			{
				long double temp_f = functions[i];
				int temp_x = xCoords[i];
				int temp_y = yCoords[i];

				functions[i] = functions[j];
				xCoords[i] = xCoords[j];
				yCoords[i] = yCoords[j];

				functions[j] = temp_f;
				xCoords[j] = temp_x;
				yCoords[j] = temp_y;
			}
		}
	}
}

