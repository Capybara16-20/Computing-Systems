#include<windows.h>
#include "Kernel.cuh"
struct img
{
	int width;
	int height;
	unsigned* image;
};

struct point
{
	int x;
	int y;

	point()
	{
		x = y = 0;
	}

	point(int _x, int _y) : x(_x), y(_y) {}

	point operator+(point& other)
	{
		return point(x + other.x, y + other.y);
	}

	point operator-(point& other)
	{
		return point(x - other.x, y - other.y);
	}

	point operator*(double value)
	{
		return point(x * value, y * value);
	}

	point operator/(double value)
	{
		return point(x / value, y / value);
	}
};

void* calculate(void* mainImage, void* subImage,
	double reflection, double compression, double stretch,
	double accuracy, int iterations)
{
	struct img* mainIm = (img*)mainImage;
	struct img* subIm = (img*)subImage;

	if (getGrayImage(mainIm->image, mainIm->width, mainIm->height) == -1)
		return NULL;
	if (getGrayImage(subIm->image, subIm->width, subIm->height) == -1)
		return NULL;

	int found_x;
	int found_y;
	if (findImage(mainIm->image, mainIm->width, mainIm->height, subIm->image, 
		subIm->width, subIm->height, reflection, compression, stretch,
		accuracy, iterations, found_x, found_y))
		return NULL;
	struct point* found_p = new point(found_x, found_y);

	return found_p;
}

BOOL APIENTRY DllMain(HMODULE hModule,
	DWORD ul_reason_for_call,
	LPVOID lpReserved)
{
	switch (ul_reason_for_call)
	{
	case DLL_PROCESS_ATTACH:
	case DLL_THREAD_ATTACH:
	case DLL_THREAD_DETACH:
	case DLL_PROCESS_DETACH:
		break;
	}
	return TRUE;
}
