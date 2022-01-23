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
	int blocksCount;
	int threadsCount;
	if (subWidth * subHeight < maxThreads)
	{
		blockSize = subWidth * subHeight;
		gridSize = 1;
	}
	else
	{
		blockSize = maxThreads;
		gridSize = (subWidth * subHeight + maxThreads - 1) / maxThreads;
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
		functions[i] = GetFunctionValue(xCoords[i], yCoords[i], dev_mainImage, mainWidth,
			mainHeight, dev_subImage, subWidth, subHeight, blockSize, gridSize);
		//functions[i] = tempGetFunctionValue(xCoords[i], yCoords[i], mainImage, mainWidth, mainHeight, subImage, subWidth, subHeight);
	}
#pragma endregion

	//int temp_x = 150;
	//int temp_y = 190;
	//long double temp_f = GetFunctionValue(temp_x, temp_y, dev_mainImage, mainWidth,
		//mainHeight, dev_subImage, subWidth, subHeight, blockSize, gridSize);
	//long double temp_f = tempGetFunctionValue(temp_x, temp_y, mainImage, mainWidth, mainHeight, subImage, subWidth, subHeight);

	//Проведение итераций для всех наборов точек
	long double found_f = 0;
	int best_x, best_y, middle_x, middle_y, worst_x, worst_y;
	long double best_f, middle_f, worst_f;
	for (int i = 0; i < iterations; i++)
	{
		int* p_xCoords = new int[pointsCount];
		int* p_yCoords = new int[pointsCount];
		long double funcs[pointsCount];
		for (int j = 0; j < pointsCount; j++)
		{
			p_xCoords[j] = xCoords[i * pointsCount + j];
			p_yCoords[j] = yCoords[i * pointsCount + j];
			funcs[j] = functions[i * pointsCount + j];
		}

		bool isFound = false;
		while (!isFound)
		{
			SortCoords(p_xCoords, p_yCoords, funcs, pointsCount);
			best_x = p_xCoords[bestIndex];
			best_y = p_yCoords[bestIndex];
			best_f = funcs[bestIndex];
			middle_x = p_xCoords[middleIndex];
			middle_y = p_yCoords[middleIndex];
			middle_f = funcs[middleIndex];
			worst_x = p_xCoords[worstIndex];
			worst_y = p_yCoords[worstIndex];
			worst_f = funcs[worstIndex];

			//Расчёт центра тяжести
			int gravityCenter_x = (best_x + middle_x) / (pointsCount - 1);
			int gravityCenter_y = (best_y + middle_y) / (pointsCount - 1);
			long double gravityCenter_f = GetFunctionValue(gravityCenter_x, gravityCenter_y, dev_mainImage, mainWidth,
				mainHeight, dev_subImage, subWidth, subHeight, blockSize, gridSize);
			//long double gravityCenter_f = tempGetFunctionValue(gravityCenter_x, gravityCenter_y, mainImage, mainWidth, mainHeight, subImage, subWidth, subHeight);

			//Провера окончания
			bool isImageFound = IsImageFound(funcs, pointsCount, gravityCenter_f, accuracy);
			if (isImageFound)
			{
				isFound = true;

				if (funcs[bestIndex] > found_f)
				{
					found_x = p_xCoords[bestIndex];
					found_y = p_yCoords[bestIndex];
					found_f = funcs[bestIndex];
				}

				break;
			}

			if (abs(gravityCenter_x - worst_x) <= 1 && abs(gravityCenter_y - worst_y) <= 1)
				break;

			//"Отражение"
			int reflected_x = GetReflectedCoord(gravityCenter_x, worst_x, reflection, maxX);
			int reflected_y = GetReflectedCoord(gravityCenter_y, worst_y, reflection, maxY);

			long double reflected_f = GetFunctionValue(reflected_x, reflected_y, dev_mainImage, mainWidth,
				mainHeight, dev_subImage, subWidth, subHeight, blockSize, gridSize);
			//long double reflected_f = tempGetFunctionValue(reflected_x, reflected_y, mainImage, mainWidth, mainHeight, subImage, subWidth, subHeight);

			if (reflected_f >= best_f) //"Растяжение"
			{
				int stretched_x = GetStretchedCoord(gravityCenter_x, reflected_x, stretch, maxX);
				int stretched_y = GetStretchedCoord(gravityCenter_y, reflected_y, stretch, maxY);

				long double stretched_f = GetFunctionValue(stretched_x, stretched_y, dev_mainImage, mainWidth,
					mainHeight, dev_subImage, subWidth, subHeight, blockSize, gridSize);
				//long double stretched_f = tempGetFunctionValue(stretched_x, stretched_y, mainImage, mainWidth, mainHeight, subImage, subWidth, subHeight);

				if (stretched_f > best_f)
				{
					p_xCoords[worstIndex] = stretched_x;
					p_yCoords[worstIndex] = stretched_y;
					funcs[worstIndex] = stretched_f;
				}
				else
				{
					p_xCoords[worstIndex] = reflected_x;
					p_yCoords[worstIndex] = reflected_y;
					funcs[worstIndex] = reflected_f;
				}
			}
			else if (reflected_f >= worst_f && reflected_f < middle_f) //"Сжатие"
			{
				int compressed_x = GetCompressedCoord(gravityCenter_x, worst_x, compression, maxX);
				int compressed_y = GetCompressedCoord(gravityCenter_y, worst_y, compression, maxY);
				
				long double compressed_f = GetFunctionValue(compressed_x, compressed_y, dev_mainImage, mainWidth,
					mainHeight, dev_subImage, subWidth, subHeight, blockSize, gridSize);
				//long double compressed_f = tempGetFunctionValue(compressed_x, compressed_y, mainImage, mainWidth, mainHeight, subImage, subWidth, subHeight);

				p_xCoords[worstIndex] = compressed_x;
				p_yCoords[worstIndex] = compressed_y;
				funcs[worstIndex] = compressed_f;
			}
			else if (reflected_f >= middle_f && reflected_f < best_f) //Замена наихудшей вершины
			{
				p_xCoords[worstIndex] = reflected_x;
				p_yCoords[worstIndex] = reflected_y;
				funcs[worstIndex] = reflected_f;
			}
			else //"Редукция"
			{
				p_xCoords = GetReducedCoords(p_xCoords, best_x, pointsCount, maxX);
				p_yCoords = GetReducedCoords(p_yCoords, best_y, pointsCount, maxY);

				for (int i = 0; i < pointsCount; i++)
					funcs[i] = GetFunctionValue(p_xCoords[i], p_yCoords[i], dev_mainImage, mainWidth,
						mainHeight, dev_subImage, subWidth, subHeight, blockSize, gridSize);
				//for (int i = 0; i < pointsCount; i++)
					//funcs[i] = tempGetFunctionValue(p_xCoords[i], p_yCoords[i], mainImage, mainWidth, mainHeight, subImage, subWidth, subHeight);
			}

		}
	}
	cudaFree(dev_mainImage);
	cudaFree(dev_subImage);

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

long double GetFunctionValue(int x, int y, unsigned* dev_mainImage, int mainWidth, int mainHeight,
	unsigned* dev_subImage, int subWidth, int subHeight, int blockSize, int gridSize)
{
	dim3 threads(blockSize);
	dim3 blocks(gridSize);

	long double* numerators = new long double[blocks.x];
	long double* dev_numerators;
	cudaMalloc((void**)&dev_numerators, sizeof(long double) * blocks.x);
	long double* subMultipliers = new long double[blocks.x];
	long double* dev_subMultipliers;
	cudaMalloc((void**)&dev_subMultipliers, sizeof(long double) * blocks.x);
	long double* mainMultipliers = new long double[blocks.x];
	long double* dev_mainMultipliers;
	cudaMalloc((void**)&dev_mainMultipliers, sizeof(long double) * blocks.x);

	getFunctionParameters <<<blocks, threads>>> (x, y, dev_mainImage,
		mainWidth, mainHeight, dev_subImage, subWidth, subHeight, dev_numerators,
		dev_subMultipliers, dev_mainMultipliers);

	cudaMemcpy(numerators, dev_numerators, sizeof(long double) * blocks.x, cudaMemcpyDeviceToHost);
	cudaMemcpy(subMultipliers, dev_subMultipliers, sizeof(long double) * blocks.x, cudaMemcpyDeviceToHost);
	cudaMemcpy(mainMultipliers, dev_mainMultipliers, sizeof(long double) * blocks.x, cudaMemcpyDeviceToHost);

	cudaFree(dev_numerators);
	cudaFree(dev_subMultipliers);
	cudaFree(dev_mainMultipliers);

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

	cudaFree(dev_numerators);
	cudaFree(dev_subMultipliers);
	cudaFree(dev_mainMultipliers);
	return function;
}

bool IsImageFound(long double* functions, int pointsCount,
	long double gravityCenter_f, double accuracy)
{
	long double currentAccuracy = 0;
	for (int i = 0; i < pointsCount; i++)
		currentAccuracy += pow(functions[i] - gravityCenter_f, 2);
	currentAccuracy = sqrt(currentAccuracy / pointsCount);

	return currentAccuracy <= accuracy;
}

int GetReflectedCoord(int gravityCenter, int worst, double reflection, int max)
{
	int reflected = gravityCenter + (gravityCenter - worst) * reflection;

	if (reflected > max)
		reflected = max;
	if (reflected < 0)
		reflected = 0;

	return reflected;
}

int GetStretchedCoord(int gravityCenter, int reflected, double stretch, int max)
{
	int stretched = gravityCenter + (reflected - gravityCenter) * stretch;

	if (stretched > max)
		stretched = max;
	if (stretched < 0)
		stretched = 0;

	return reflected;
}

int GetCompressedCoord(int gravityCenter, int worst, double compression, int max)
{
	int compressed = gravityCenter + (worst - gravityCenter) * compression;

	if (compressed > max)
		compressed = max;
	if (compressed < 0)
		compressed = 0;

	return compressed;
}

int* GetReducedCoords(int* coords, int best, int pointsCount, int max)
{
	int* reducedCoords = new int[pointsCount];
	for (int i = 0; i < pointsCount; i++)
	{
		int reducedCoord = best + (coords[i] - best) * 0.5;

		if (reducedCoord > max)
			reducedCoord = max;
		if (reducedCoord < 0)
			reducedCoord = 0;

		reducedCoords[i] = reducedCoord;
	}

	return reducedCoords;
}

long double tempGetFunctionValue (int x, int y, unsigned* main, int mainWidth, int mainHeight,
	unsigned* sub, int subWidth, int subHeight)
{
	long double numerator = 0;
	for (int i = 0; i < subHeight; i++)
	{
		int check = 0;
		for (int j = 0; j < subWidth; j++)
		{
			long double subBrightness = tempGetBrightness(sub[i * subWidth + j]);
			long double mainBrightness = tempGetBrightness(main[mainWidth * (y + i) + x + j]);
			numerator += subBrightness * mainBrightness;
		}
	}

	long double denominator;
	long double subMultiplier = 0;
	long double mainMultiplier = 0;
	for (int i = 0; i < subHeight; i++)
	{
		for (int j = 0; j < subWidth; j++)
		{
			long double subBrightness = tempGetBrightness(sub[i * subWidth + j]);
			long double mainBrightness = tempGetBrightness(main[mainWidth * (y + i) + x + j]);
			subMultiplier += pow(subBrightness, 2);
			mainMultiplier += pow(mainBrightness, 2);
		}
	}
	denominator = sqrt(subMultiplier * mainMultiplier);

	return numerator / denominator;
}

long double tempGetBrightness(unsigned pixel)
{
	unsigned red = (pixel & 0x00ff0000) >> 16;
	unsigned green = (pixel & 0x0000ff00) >> 8;
	unsigned blue = (pixel & 0x000000ff);

	long double brightness = 0.299 * red + 0.587 * green + 0.114 * blue;
	return brightness;
}