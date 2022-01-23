#ifndef KERNEL
#define KERNEL
int getGrayImage(unsigned* srcImage, int width, int height);
int findImage(unsigned* mainImage, int mainWidth, int mainHeight,
	unsigned* subImage, int subWidth, int subHeight,
	double reflection, double compression, double stretch,
	double accuracy, int iterations, int& found_x, int& found_y);
long double GetFunctionValue(int x, int y, unsigned* mainImage, int mainWidth, int mainHeight,
	unsigned* subImage, int subWidth, int subHeight, int blocks, int threads);
void SortCoords(int* xCoords, int* yCoords, long double* functions, int pointsCount);
bool IsImageFound(long double* functions, int pointsCount,
	long double gravityCenter_f, double accuracy);
int GetReflectedCoord(int gravityCenter, int worst, double reflection, int max);
int GetStretchedCoord(int gravityCenter, int reflected, double stretch, int max);
int GetCompressedCoord(int gravityCenter, int reflected, double stretch, int max);
int* GetReducedCoords(int* coords, int best, int pointsCount, int max);
#endif
