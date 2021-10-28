#include<iostream>
#include<CL/cl.h>
#include<iostream>
#include<fstream>
#include <string.h>
#include <string>
#include "iomanip"
using namespace std;

#define SIZE 1000

cl_device_id* configureDevices();
int convertToString(const char* fileName, string& s);
void example1(cl_device_id* devices);
void disposeDevices(cl_device_id* devices);
void example2(cl_device_id* devices);

int main(int argc, char* argv[])
{
	cl_device_id* devices = configureDevices();
	//example1(devices);
	example2(devices);
	disposeDevices(devices);
}

cl_device_id* configureDevices()
{
	//Запрос доступных платформ и устройств
	cl_uint numberOfPlatforms;
	cl_platform_id platform = NULL;
	cl_int status = clGetPlatformIDs(0, NULL, &numberOfPlatforms);
	if (numberOfPlatforms > 0)
	{
		cl_platform_id* platforms = new cl_platform_id[numberOfPlatforms];
		status = clGetPlatformIDs(numberOfPlatforms, platforms, NULL);
		platform = platforms[0];
		delete[] platforms;
	}
	cl_uint numberOfDevices = 0;
	cl_device_id* devices;
	status = clGetDeviceIDs(platform, CL_DEVICE_TYPE_ALL, 0,
		NULL, &numberOfDevices);
	if (status != CL_SUCCESS)
	{
		cout << "Не найдено устройство, поддерживающее OpenCL" << endl;
		exit(1);
	}
	devices = new cl_device_id[numberOfDevices];
	status = clGetDeviceIDs(platform, CL_DEVICE_TYPE_ALL,
		numberOfDevices, devices, NULL);
	if (status != CL_SUCCESS)
	{
		cout << "Не найдено устройство, поддерживающее OpenCL" << endl;
		exit(1);
	}
	return devices;
}

void disposeDevices(cl_device_id* devices)
{
	delete[] devices;
}

void example1(cl_device_id* devices)
{
	char tempStr[1024];
	cout << "Device info" << endl;
	cl_uint status = clGetDeviceInfo(*devices, CL_DEVICE_NAME,
		sizeof(tempStr), tempStr, 0);
	cout << "Device name : " << tempStr << endl;
	status = clGetDeviceInfo(*devices, CL_DEVICE_VENDOR,
		sizeof(tempStr), tempStr, 0);
	cout << "Vendor : " << tempStr << endl;
	status = clGetDeviceInfo(*devices, CL_DRIVER_VERSION,
		sizeof(tempStr), tempStr, 0);
	cout << "Driver version : " << tempStr << endl;
	status = clGetDeviceInfo(*devices, CL_DEVICE_OPENCL_C_VERSION,
		sizeof(tempStr), tempStr, 0);
	cout << "OpenCL C version: " << tempStr << endl;
	cl_uint maxComputeUnits = 0;
	status = clGetDeviceInfo(*devices, CL_DEVICE_MAX_COMPUTE_UNITS,
		sizeof(maxComputeUnits), &maxComputeUnits, 0);
	cout << "Max compute units: " << maxComputeUnits << endl;
	cl_uint maxWorkItemDimensions = 0;
	status = clGetDeviceInfo(*devices, CL_DEVICE_MAX_COMPUTE_UNITS,
		sizeof(maxWorkItemDimensions), &maxWorkItemDimensions, 0);
	cout << "Max work-item dimensions : " << maxWorkItemDimensions << endl;
	cl_uint maxClockFrequency;
	status = clGetDeviceInfo(*devices, CL_DEVICE_MAX_CLOCK_FREQUENCY,
		sizeof(maxClockFrequency), &maxClockFrequency, 0);
	cout << "Max clock fraquency : " << maxClockFrequency << " MHz" << endl;
	cl_uint maxWorkItemGroupSize;
	status = clGetDeviceInfo(*devices, CL_DEVICE_MAX_WORK_GROUP_SIZE,
		sizeof(maxWorkItemGroupSize), &maxWorkItemGroupSize, 0);
	cout << "Max work-item group size : " << maxWorkItemGroupSize << endl;
	cl_ulong maxMemSize;
	status = clGetDeviceInfo(*devices, CL_DEVICE_GLOBAL_MEM_SIZE,
		sizeof(maxMemSize), &maxMemSize, 0);
	cout << "Global mem size : " << (maxMemSize / 1048576) << " Mbytes." << endl;
	status = clGetDeviceInfo(*devices, CL_DEVICE_EXTENSIONS,
		sizeof(tempStr), tempStr, 0);
	cout << "Extentions : " << tempStr << endl;
}

void example2(cl_device_id* devices)
{
	//Создание контекста вычислений
	cl_context context = clCreateContext(NULL, 1, devices, NULL, NULL, NULL); //Создание контекста

	//Создание очереди команд
	cl_command_queue commandQueue = clCreateCommandQueue(context, devices[0], 0, NULL); //Создание очереди команд
	
	//Создание программного объекта
	const char* fileName = "example1.cl";
	std::string sourceStr;
	cl_int status = convertToString(fileName, sourceStr);
	const char* source = sourceStr.c_str();
	size_t sizeOfSource[] = { strlen(source) };
	cl_program program = clCreateProgramWithSource(context, 1, 
		&source, sizeOfSource, &status); //Создание программы
	status = clBuildProgram(program, 1, devices, NULL, NULL, NULL); //Сборка программы
	
	//Инициализация данных и создание объектов памяти
	cl_float* input = new cl_float[SIZE];
	for (int i = 0; i < SIZE; i++)
		input[i] = i;
	cl_float* result = new cl_float[SIZE];
	cl_mem inputBuffer = clCreateBuffer(context,
		CL_MEM_READ_WRITE | CL_MEM_COPY_HOST_PTR, sizeof(cl_float) *
		SIZE, input, &status); //Создание буфера
	cl_mem resultBuffer = clCreateBuffer(context,
		CL_MEM_READ_WRITE, sizeof(cl_float) * SIZE, NULL, &status); //Создание буфера

	//Создание объекта ядра
	cl_kernel kernel = clCreateKernel(program, "example1", &status); //Получение ядра

	//Запуск ядра на выполнение
	status = clSetKernelArg(kernel, 0, sizeof(cl_mem), (void*)&inputBuffer); //Установка буфера в качестве аргумента ядра
	status = clSetKernelArg(kernel, 1, sizeof(cl_mem), (void*)&resultBuffer); //Установка буфера в качестве аргумента ядра
	const size_t global_work_size[1] = { SIZE };
	status = clEnqueueNDRangeKernel(commandQueue, kernel, 1,
		NULL, global_work_size, NULL, 0, NULL, NULL); //Запуск ядра
	clFinish(commandQueue); //Ожидаем завершения операций
	
	//Загрузка результатов вычислений
	cl_float* output = new cl_float[SIZE];
	clEnqueueReadBuffer(commandQueue, resultBuffer, CL_TRUE, 0,
		sizeof(cl_float) * SIZE, output, 0, NULL, NULL); //Чтение буфера
	for (int i = 0; i < SIZE; i++)
		cout << setw(9) << left << output[i] << " ";
	
	//Удаление объектов и освобождение памяти
	clReleaseKernel(kernel);
	clReleaseMemObject(inputBuffer);
	clReleaseMemObject(resultBuffer);
	clReleaseProgram(program);
	clReleaseCommandQueue(commandQueue);
	clReleaseContext(context);
}

int convertToString(const char* fileName, string& s)
{
	size_t size;
	char* str;
	fstream f(fileName, (std::fstream::in | std::fstream::binary));
	if (f.is_open())
	{
		size_t fileSize;
		f.seekg(0, fstream::end);
		size = fileSize = (size_t)f.tellg();
		f.seekg(0, fstream::beg);
		str = new char[size + 1];
		if (!str)
		{
			f.close();
			return 0;
		}
		f.read(str, fileSize);
		f.close();
		str[size] = '\0';
		s = str;
		delete[] str;
		return 0;
	}
	return -1;
}
