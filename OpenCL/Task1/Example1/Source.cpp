#include<iostream>
#include<CL/cl.h>
#include<iostream>
#include<fstream>
#include <string.h>
#include <string>
using namespace std;cl_device_id* configureDevices();
int convertToString(const char* fileName, string& s);
void example1(cl_device_id* devices);
void disposeDevices(cl_device_id* devices);

int main(int argc, char* argv[])
{
	cl_device_id* devices = configureDevices();
	example1(devices);
	disposeDevices(devices);
}

cl_device_id* configureDevices()
{
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
}void example1(cl_device_id* devices)
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
