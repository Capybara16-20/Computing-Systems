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

void example2(cl_device_id* devices)
{
	cl_context context = clCreateContext(NULL, 1, devices, NULL, NULL, NULL);
	cl_command_queue commandQueue = clCreateCommandQueue(context, devices[0], 0, NULL);
	const char* fileName = "example2.cl";
	std::string sourceStr;
	cl_int status = convertToString(fileName, sourceStr);
	const char* source = sourceStr.c_str();
	size_t sizeOfSource[] = { strlen(source) };
	cl_program program = clCreateProgramWithSource(context, 1,
		&source, sizeOfSource, &status);
	status = clBuildProgram(program, 1, devices, NULL, NULL, NULL);
	cl_float* a = new cl_float[SIZE];
	cl_float* b = new cl_float[SIZE];
	for (int i = 0; i < SIZE; i++)
	{
		a[i] = i;
		b[i] = i + 1;
	}
	cl_float* result = new cl_float[SIZE];
	cl_mem aBuffer = clCreateBuffer(context,
		CL_MEM_READ_WRITE | CL_MEM_COPY_HOST_PTR, sizeof(cl_float) *
		SIZE, a, &status);
	cl_mem bBuffer = clCreateBuffer(context,
		CL_MEM_READ_WRITE | CL_MEM_COPY_HOST_PTR, sizeof(cl_float) *
		SIZE, b, &status);
	cl_mem xBuffer = clCreateBuffer(context,
		CL_MEM_READ_WRITE, sizeof(cl_float) * SIZE, NULL, &status);
	cl_kernel kernel = clCreateKernel(program, "example2", &status);
	status = clSetKernelArg(kernel, 0, sizeof(cl_mem), (void*)&aBuffer);
	status = clSetKernelArg(kernel, 1, sizeof(cl_mem), (void*)&bBuffer);
	status = clSetKernelArg(kernel, 2, sizeof(cl_mem), (void*)&xBuffer);
	const size_t global_work_size[1] = { SIZE };
	status = clEnqueueNDRangeKernel(commandQueue, kernel, 1,
		NULL, global_work_size, NULL, 0, NULL, NULL);
	clFinish(commandQueue);
	cl_float* x = new cl_float[SIZE];
	clEnqueueReadBuffer(commandQueue, xBuffer, CL_TRUE, 0,
		sizeof(cl_float) * SIZE, x, 0, NULL, NULL);
	for (int i = 0; i < SIZE; i++)
		cout << fixed << setprecision(0) << x[i] << " ";
	clReleaseKernel(kernel);
	clReleaseMemObject(aBuffer);
	clReleaseMemObject(bBuffer);
	clReleaseMemObject(xBuffer);
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
