#include<windows.h>
#include "Kernel.cuh"

struct img
{
	int width;
	int height;
	unsigned* image;
};

void calculate(void* image, void* redImage,
	void* greenImage, void* blueImage)
{
	struct img* im = (img*)image;
	struct img* redIm = (img*)redImage;
	struct img* greenIm = (img*)greenImage;
	struct img* blueIm = (img*)blueImage;
	calc(im->image, redIm->image, greenIm->image,
		blueIm->image, im->width, im->height);
}

BOOL APIENTRY DllMain(HMODULE hModule,
	DWORD ul_reason_for_call, LPVOID lpReserved)
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

