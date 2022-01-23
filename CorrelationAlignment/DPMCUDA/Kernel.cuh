#ifndef KERNEL
#define KERNEL
int getGrayImage(unsigned* srcImage, int width, int height);
int findImage(unsigned* mainImage, int mainWidth, int mainHeight,
	unsigned* subImage, int subWidth, int subHeight,
	double reflection, double compression, double stretch,
	double accuracy, int iterations, int& found_x, int& found_y);
void SortCoords(int* xCoords, int* yCoords, long double* functions, int pointsCount);
#endif
