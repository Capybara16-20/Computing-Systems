#include "MainForm.h"
#include <windows.h>
#include <ctime>  
#include <string>
#include <omp.h>
#pragma once
using namespace System;
using namespace System::Windows::Forms;

[STAThread]
void main(array<String^>^ args)
{
	Application::EnableVisualStyles();
	Application::SetCompatibleTextRenderingDefault(false);
	DeformablePolyhedronMethod::MainForm form;
	Application::Run(% form);

	srand(time(NULL));
}

struct DeformablePolyhedronMethod::img* DeformablePolyhedronMethod::MainForm::
	GetImageCopy(struct DeformablePolyhedronMethod::img* image)
{
	int height = image->height;
	int width = image->width;

	img* copy = new img();
	unsigned* copyImage = new unsigned[width * height];
	for (int i = 0; i < width * height; i++)
		copyImage[i] = image->image[i];
	copy->image = copyImage;
	copy->height = height;
	copy->width = width;

	return copy;
}

void DeformablePolyhedronMethod::MainForm::
	InitializeImage(struct DeformablePolyhedronMethod::img* image, PictureBox^ pbSource)
{
	image->width = pbSource->Image->Width;
	image->height = pbSource->Image->Height;
	image->image = new unsigned[image->width * image->height];
	System::Drawing::Bitmap^ bitmap = gcnew System::Drawing::Bitmap(pbSource->Image);
	for (int j = 0; j < image->height; j++)
		for (int i = 0; i < image->width; i++)
			image->image[j * image->width + i] = bitmap->GetPixel(i, j).ToArgb();
}

struct DeformablePolyhedronMethod::point DeformablePolyhedronMethod::MainForm::
	Calculate(struct DeformablePolyhedronMethod::img* mainImage, 
		struct DeformablePolyhedronMethod::img* subImage, 
		double reflection, double compression, double stretch, 
		double accuracy, int iterations)
{
	const int pointsCount = 3;
	const int bestIndex = 0;
	const int middleIndex = 1;
	const int worstIndex = 2;

	mainImage = GetGrayImage(mainImage);
	subImage = GetGrayImage(subImage);

	int maxX = mainImage->width - subImage->width;
	int maxY = mainImage->height - subImage->height;

	struct point found_p;
	long double found_f = 0;
	for (int i = 0; i < iterations; i++)
	{
		struct point* points = new point[pointsCount];
		long double* functions = new long double[pointsCount];
		for (int i = 0; i < pointsCount; i++)
		{
			points[i] = GetRandomPoint(maxX, maxY);
			functions[i] = GetFunctionValue(points[i], mainImage, subImage);
		}

		struct point best_p;
		long double best_f;
		struct point middle_p;
		long double middle_f;
		struct point worst_p;
		long double worst_f;

		bool isFound = false;
		while (!isFound)
		{
			SortPoints(points, functions, pointsCount);
			best_p = points[bestIndex];
			best_f = functions[bestIndex];
			middle_p = points[middleIndex];
			middle_f = functions[middleIndex];
			worst_p = points[worstIndex];
			worst_f = functions[worstIndex];

			struct point gravityCenter_p = GetGravityCenter(points, pointsCount);
			long double gravityCenter_f = GetFunctionValue(gravityCenter_p, mainImage, subImage);

			long double currentAccuracy;
			bool isImageFound = IsImageFound(functions, pointsCount, gravityCenter_f, accuracy, currentAccuracy);
			if (isImageFound)
			{
				isFound = true;

				if (functions[bestIndex] > found_f)
				{
					found_p = points[bestIndex];
					found_f = functions[bestIndex];
				}

				break;
			}

			if (gravityCenter_p.x == worst_p.x && gravityCenter_p.y == worst_p.y)
				break;

			struct point reflected_p = GetReflectedPoint(gravityCenter_p, worst_p, reflection, maxX, maxY); //"отражение"
			long double reflected_f = GetFunctionValue(reflected_p, mainImage, subImage);

			if (reflected_f >= best_f) //"растяжение"
			{
				struct point stretched_p = GetStretchedPoint(gravityCenter_p, reflected_p, stretch, maxX, maxY);
				long double stretched_f = GetFunctionValue(stretched_p, mainImage, subImage);

				if (stretched_f > best_f)
				{
					points[worstIndex] = stretched_p;
					functions[worstIndex] = stretched_f;
				}
				else
				{
					points[worstIndex] = reflected_p;
					functions[worstIndex] = reflected_f;
				}
			}
			else if (reflected_f >= worst_f && reflected_f < middle_f) //"сжатие"
			{
				struct point compressed_p = GetCompressedPoint(gravityCenter_p, worst_p, compression, maxX, maxY);
				long double compressed_f = GetFunctionValue(compressed_p, mainImage, subImage);

				points[worstIndex] = compressed_p;
				functions[worstIndex] = compressed_f;
			}
			else if (reflected_f >= middle_f && reflected_f < best_f) //замена наихудшей вершины
			{
				points[worstIndex] = reflected_p;
				functions[worstIndex] = reflected_f;
			}
			else //"редукция"
			{
				points = GetReducedPoints(points, best_p, pointsCount, maxX, maxY);
				for (int i = 0; i < pointsCount; i++)
					functions[i] = GetFunctionValue(points[i], mainImage, subImage);
			}
		}
	}

	return found_p;
}

struct DeformablePolyhedronMethod::point* DeformablePolyhedronMethod::MainForm::
	GetReducedPoints(struct DeformablePolyhedronMethod::point* points, 
	struct DeformablePolyhedronMethod::point best_p, int pointsCount, int maxX, int maxY)
{
	struct point* reducedPoints = new point[pointsCount];
	for (int i = 0; i < pointsCount; i++)
	{
		struct point reduced_p = best_p + (points[i] - best_p) * 0.5;
		
		if (reduced_p.x > maxX)
			reduced_p.x = maxX;
		if (reduced_p.x < 0)
			reduced_p.x = 0;
		if (reduced_p.y > maxY)
			reduced_p.y = maxY;
		if (reduced_p.y < 0)
			reduced_p.y = 0;

		reducedPoints[i] = reduced_p;
	}

	return reducedPoints;
}

struct DeformablePolyhedronMethod::point DeformablePolyhedronMethod::MainForm::
	GetCompressedPoint(struct DeformablePolyhedronMethod::point gravityCenter_p,
		struct DeformablePolyhedronMethod::point worst_p, double compression, int maxX, int maxY)
{
	struct point compressed_p = gravityCenter_p + (worst_p - gravityCenter_p) * compression;

	if (compressed_p.x > maxX)
		compressed_p.x = maxX;
	if (compressed_p.x < 0)
		compressed_p.x = 0;
	if (compressed_p.y > maxY)
		compressed_p.y = maxY;
	if (compressed_p.y < 0)
		compressed_p.y = 0;

	return compressed_p;
}

struct DeformablePolyhedronMethod::point DeformablePolyhedronMethod::MainForm::
	GetStretchedPoint(struct DeformablePolyhedronMethod::point gravityCenter_p,
		struct DeformablePolyhedronMethod::point reflected_p, double stretch, int maxX, int maxY)
{
	struct point stretched_p = gravityCenter_p + (reflected_p - gravityCenter_p) * stretch;

	if (stretched_p.x > maxX)
		stretched_p.x = maxX;
	if (stretched_p.x < 0)
		stretched_p.x = 0;
	if (stretched_p.y > maxY)
		stretched_p.y = maxY;
	if (stretched_p.y < 0)
		stretched_p.y = 0;

	return stretched_p;
}

struct DeformablePolyhedronMethod::point DeformablePolyhedronMethod::MainForm::
	GetReflectedPoint(struct DeformablePolyhedronMethod::point gravityCenter_p, 
		struct DeformablePolyhedronMethod::point worst_p, double reflection, int maxX, int maxY)
{
	struct point reflected_p = gravityCenter_p + (gravityCenter_p - worst_p) * reflection;
	
	if (reflected_p.x > maxX)
		reflected_p.x = maxX;
	if (reflected_p.x < 0)
		reflected_p.x = 0;
	if (reflected_p.y > maxY)
		reflected_p.y = maxY;
	if (reflected_p.y < 0)
		reflected_p.y = 0;

	return reflected_p;
}

bool DeformablePolyhedronMethod::MainForm::
	IsImageFound(long double* functions, int pointsCount, 
		long double gravityCenter_f, double accuracy, long double& currentAccuracy)
{
	currentAccuracy = 0;
	for (int i = 0; i < pointsCount; i++)
		currentAccuracy += pow(functions[i] - gravityCenter_f, 2);
	currentAccuracy /= pointsCount;
	currentAccuracy = sqrt(currentAccuracy);

	return currentAccuracy <= accuracy;
}

struct DeformablePolyhedronMethod::point DeformablePolyhedronMethod::MainForm::
	GetGravityCenter(DeformablePolyhedronMethod::point* points, int pointsCount)
{
	int count = pointsCount - 1;
	struct point gravityCenter_p = points[0];
	for (int i = 1; i < count; i++)
		gravityCenter_p = gravityCenter_p + points[i];

	return gravityCenter_p / count;
}

void DeformablePolyhedronMethod::MainForm::
	SortPoints(struct DeformablePolyhedronMethod::point* points, 
	long double* functions, int pointsCount)
{
	for (int i = 0; i < pointsCount; i++)
	{
		for (int j = i + 1; j < pointsCount; j++)
		{
			if (functions[i] < functions[j])
			{
				long double temp_f = functions[i];
				struct point temp_p = points[i];

				functions[i] = functions[j];
				points[i] = points[j];

				functions[j] = temp_f;
				points[j] = temp_p;
			}
		}
	}
}

struct DeformablePolyhedronMethod::point DeformablePolyhedronMethod::MainForm::
	GetRandomPoint(int maxX, int maxY)
{
	int x = rand() % maxX;
	int y = rand() % maxY;

	point newPoint;
	newPoint.x = x;
	newPoint.y = y;

	return newPoint;
}

struct DeformablePolyhedronMethod::img* DeformablePolyhedronMethod::MainForm::
	GetGrayImage(struct DeformablePolyhedronMethod::img* image)
{
	int width = image->width;
	int height = image->height;

	img* newImage = GetImageCopy(image);
	unsigned* grayImage = newImage->image;
	for (int i = 0; i < height * width; i++)
	{
		unsigned pixel = grayImage[i];
		unsigned gray = ((pixel & 0x00FF0000 >> 0x10) +
			(pixel & 0x0000FF00 >> 0x8) + (pixel & 0x000000FF)) / 3;
		grayImage[i] = 0xFF000000 | (gray << 0x10) | (gray << 0x8) | gray;
	}

	return newImage;
}

long double DeformablePolyhedronMethod::MainForm::GetFunctionValue
	(struct DeformablePolyhedronMethod::point point,
	struct DeformablePolyhedronMethod::img* mainImage,
	struct DeformablePolyhedronMethod::img* subImage)
{
	int mainHeight = mainImage->height;
	int mainWidth = mainImage->width;
	int subHeight = subImage->height;
	int subWidth = subImage->width;
	unsigned* main = mainImage->image;
	unsigned* sub = subImage->image;
	int y = point.y;
	int x = point.x;

	long double numerator = 0;
	long double subMultiplier = 0;
	long double mainMultiplier = 0;
	for (int i = 0; i < subHeight; i++)
	{
		for (int j = 0; j < subWidth; j++)
		{
			long double subBrightness = GetBrightness(sub[i * subWidth + j]);
			long double mainBrightness = GetBrightness(main[mainWidth * (y + i) + x + j]);
			numerator += subBrightness * mainBrightness;
			subMultiplier += pow(subBrightness, 2);
			mainMultiplier += pow(mainBrightness, 2);
		}
	}
	long double denominator = sqrt(subMultiplier * mainMultiplier);

	return numerator / denominator;
}

long double DeformablePolyhedronMethod::MainForm::GetBrightness(unsigned pixel)
{
	unsigned red = (pixel & 0x00ff0000) >> 16;
	unsigned green = (pixel & 0x0000ff00) >> 8;
	unsigned blue = (pixel & 0x000000ff);

	long double brightness = 0.299 * red + 0.587 * green + 0.114 * blue;
	return brightness;
}

struct DeformablePolyhedronMethod::img* DeformablePolyhedronMethod::MainForm::
	GetImageWithPoint(struct DeformablePolyhedronMethod::img* srcImage,
		struct DeformablePolyhedronMethod::point point)
{
	int mainWidth = srcImage->width;
	int mainHeight = srcImage->height;
	int x = point.x;
	int y = point.y;

	img* newImage = GetImageCopy(srcImage);
	unsigned* image = newImage->image;
	for (int i = 0; i < 2; i++)
		for (int j = 0; j < 2; j++)
			image[mainWidth * (y + j) + x + i] = 0xFFFF0000;

	return newImage;
}

struct DeformablePolyhedronMethod::point DeformablePolyhedronMethod::MainForm::
	CalculateCUDA(struct DeformablePolyhedronMethod::img* mainImage,
		struct DeformablePolyhedronMethod::img* subImage,
		double reflection, double compression, double stretch,
		double accuracy, int iterations)
{
	HINSTANCE hGetProcIDDLL = LoadLibrary(L"DPMCUDA.dll");
	if (!hGetProcIDDLL)
		throw gcnew Exception();

	typedef int(__stdcall* function)(void*, void*, double, double, double, double, int);
	function calculate = (function)GetProcAddress(hGetProcIDDLL, "calculate");
	if (!calculate)
		throw gcnew Exception();

	struct point* found_p = (point*)calculate((void*)mainImage, (void*)subImage,
		reflection, compression, stretch, accuracy, iterations);

	FreeLibrary(hGetProcIDDLL);

	return *found_p;
}

struct DeformablePolyhedronMethod::point DeformablePolyhedronMethod::MainForm::
	CalculateOpenMP(struct DeformablePolyhedronMethod::img* mainImage,
		struct DeformablePolyhedronMethod::img* subImage,
		double reflection, double compression, double stretch,
		double accuracy, int iterations)
{
	const int pointsCount = 3;
	const int bestIndex = 0;
	const int middleIndex = 1;
	const int worstIndex = 2;

	mainImage = GetGrayImage_OpenMP(mainImage);
	subImage = GetGrayImage_OpenMP(subImage);

	int maxX = mainImage->width - subImage->width;
	int maxY = mainImage->height - subImage->height;

	struct point found_p;
	long double found_f = 0;

#pragma omp parallel shared(maxX, maxY, found_p, found_f)
	{
		struct point* points;
		long double* functions;

		struct point best_p;
		long double best_f;
		struct point middle_p;
		long double middle_f;
		struct point worst_p;
		long double worst_f;
		bool isFound;

		struct point gravityCenter_p;
		long double gravityCenter_f;
		bool isImageFound;
		long double currentAccuracy;

		struct point reflected_p;
		long double reflected_f;
		struct point stretched_p;
		long double stretched_f;
		struct point compressed_p;
		long double compressed_f;

#pragma omp for private(points, functions, best_p, best_f, middle_p, middle_f, worst_p, worst_f, isFound, gravityCenter_p, gravityCenter_f, currentAccuracy, reflected_p, reflected_f, stretched_p, stretched_f, compressed_p, compressed_f)
		for (int i = 0; i < iterations; i++)
		{
			points = new point[pointsCount];
			functions = new long double[pointsCount];
			for (int i = 0; i < pointsCount; i++)
			{
				points[i] = GetRandomPoint(maxX, maxY);
				functions[i] = GetFunctionValue_OpenMP(points[i], mainImage, subImage);
			}

			isFound = false;
			while (!isFound)
			{
				SortPoints(points, functions, pointsCount);
				best_p = points[bestIndex];
				best_f = functions[bestIndex];
				middle_p = points[middleIndex];
				middle_f = functions[middleIndex];
				worst_p = points[worstIndex];
				worst_f = functions[worstIndex];

				gravityCenter_p = GetGravityCenter(points, pointsCount);
				gravityCenter_f = GetFunctionValue_OpenMP(gravityCenter_p, mainImage, subImage);

				currentAccuracy;
				isImageFound = IsImageFound(functions, pointsCount, gravityCenter_f, accuracy, currentAccuracy);
				if (isImageFound)
				{
					isFound = true;

					if (functions[bestIndex] > found_f)
					{
						found_p = points[bestIndex];
						found_f = functions[bestIndex];
					}

					break;
				}

				if (gravityCenter_p.x == worst_p.x && gravityCenter_p.y == worst_p.y)
					break;

				reflected_p = GetReflectedPoint(gravityCenter_p, worst_p, reflection, maxX, maxY); //"отражение"
				reflected_f = GetFunctionValue_OpenMP(reflected_p, mainImage, subImage);

				if (reflected_f >= best_f) //"растяжение"
				{
					stretched_p = GetStretchedPoint(gravityCenter_p, reflected_p, stretch, maxX, maxY);
					stretched_f = GetFunctionValue_OpenMP(stretched_p, mainImage, subImage);

					if (stretched_f > best_f)
					{
						points[worstIndex] = stretched_p;
						functions[worstIndex] = stretched_f;
					}
					else
					{
						points[worstIndex] = reflected_p;
						functions[worstIndex] = reflected_f;
					}
				}
				else if (reflected_f >= worst_f && reflected_f < middle_f) //"сжатие"
				{
					compressed_p = GetCompressedPoint(gravityCenter_p, worst_p, compression, maxX, maxY);
					compressed_f = GetFunctionValue_OpenMP(compressed_p, mainImage, subImage);

					points[worstIndex] = compressed_p;
					functions[worstIndex] = compressed_f;
				}
				else if (reflected_f >= middle_f && reflected_f < best_f) //замена наихудшей вершины
				{
					points[worstIndex] = reflected_p;
					functions[worstIndex] = reflected_f;
				}
				else //"редукция"
				{
					points = GetReducedPoints(points, best_p, pointsCount, maxX, maxY);
					for (int i = 0; i < pointsCount; i++)
						functions[i] = GetFunctionValue_OpenMP(points[i], mainImage, subImage);
				}
			}
		}
	}

	return found_p;
}

struct DeformablePolyhedronMethod::img* DeformablePolyhedronMethod::MainForm::
	GetGrayImage_OpenMP(struct DeformablePolyhedronMethod::img* image)
{
	int width = image->width;
	int height = image->height;

	img* newImage = GetImageCopy_OpenMP(image);
	unsigned* grayImage = newImage->image;

	unsigned pixel;
	unsigned gray;
#pragma omp parallel 
	{
#pragma omp for private (pixel, gray)
		for (int i = 0; i < height * width; i++)
		{
			pixel = grayImage[i];
			gray = ((pixel & 0x00FF0000 >> 0x10) +
				(pixel & 0x0000FF00 >> 0x8) + (pixel & 0x000000FF)) / 3;
			grayImage[i] = 0xFF000000 | (gray << 0x10) | (gray << 0x8) | gray;
		}
	}

	return newImage;
}

struct DeformablePolyhedronMethod::img* DeformablePolyhedronMethod::MainForm::
	GetImageCopy_OpenMP(struct DeformablePolyhedronMethod::img* image)
{
	int height = image->height;
	int width = image->width;

	img* copy = new img();
	unsigned* copyImage = new unsigned[width * height];
#pragma omp parallel 
	{
#pragma omp for
		for (int i = 0; i < width * height; i++)
			copyImage[i] = image->image[i];
	}
	copy->image = copyImage;
	copy->height = height;
	copy->width = width;

	return copy;
}

long double DeformablePolyhedronMethod::MainForm::GetFunctionValue_OpenMP
(struct DeformablePolyhedronMethod::point point,
	struct DeformablePolyhedronMethod::img* mainImage,
	struct DeformablePolyhedronMethod::img* subImage)
{
	int mainHeight = mainImage->height;
	int mainWidth = mainImage->width;
	int subHeight = subImage->height;
	int subWidth = subImage->width;
	unsigned* main = mainImage->image;
	unsigned* sub = subImage->image;
	int y = point.y;
	int x = point.x;

	long double numerator = 0;
	long double subMultiplier = 0;
	long double mainMultiplier = 0;

	long double subBrightness;
	long double mainBrightness;
#pragma omp parallel private(subBrightness, mainBrightness) shared(mainHeight, mainWidth, subHeight, subWidth, y, x) reduction(+:numerator) reduction(+:mainMultiplier) reduction(+:subMultiplier)
	{
#pragma omp for
		for (int i = 0; i < subHeight; i++)
		{
			for (int j = 0; j < subWidth; j++)
			{
				subBrightness = GetBrightness(sub[i * subWidth + j]);
				mainBrightness = GetBrightness(main[mainWidth * (y + i) + x + j]);

				numerator += subBrightness * mainBrightness;
				subMultiplier += pow(subBrightness, 2);
				mainMultiplier += pow(mainBrightness, 2);
			}
		}
	}
	
	long double func = numerator / sqrt(subMultiplier * mainMultiplier);
	return numerator / sqrt(subMultiplier * mainMultiplier);
}

void DeformablePolyhedronMethod::MainForm::ShowImage
	(struct DeformablePolyhedronMethod::img* image, PictureBox^ pictureBox)
{
	Bitmap^ bitmap = gcnew Bitmap(image->width, image->height);
	for (int j = 0; j < image->height; j++)
		for (int i = 0; i < image->width; i++)
			bitmap->SetPixel(i, j, Color::FromArgb(image->image[j * image->width + i]));
	pictureBox->Image = bitmap;
}

void DeformablePolyhedronMethod::MainForm::ShowFoundImage
	(struct DeformablePolyhedronMethod::img* mainImage,
	struct DeformablePolyhedronMethod::img* subImage,
	struct DeformablePolyhedronMethod::point point)
{
	int mainWidth = mainImage->width;
	int mainHeight = mainImage->height;
	int subWidth = subImage->width;
	int subHeight = subImage->height;
	int x = point.x;
	int y = point.y;

	img* newImage = GetImageCopy(mainImage);
	unsigned* image = newImage->image;
	for (int i = 0; i < subWidth; i++)
	{
		image[mainWidth * y + x + i] = 0xFFFF0000;
		image[mainWidth * (y + subHeight) + x + i] = 0xFFFF0000;
	}
	for (int i = 0; i < subHeight; i++)
	{
		image[mainWidth * (y + i) + x] = 0xFFFF0000;
		image[mainWidth * (y + i) + x + subWidth] = 0xFFFF0000;
	}

	ShowImage(newImage, pbMain);
}

void DeformablePolyhedronMethod::MainForm::ShowResult
	(struct DeformablePolyhedronMethod::point found_p, double leadTime)
{
	String^ coordinates_str = "Coordinates: (" + found_p.x.ToString() + "," + found_p.y.ToString() + ");" + "\r\n";
	tbResult->Text = coordinates_str;
	String^ leadTime_str = "Lead time: " + (leadTime).ToString() + " (sec).";
	tbResult->Text += leadTime_str;
}

Void DeformablePolyhedronMethod::MainForm::
	menuCalculate_Click(System::Object^ sender, System::EventArgs^ e)
{
	double reflection = Convert::ToDouble(tbReflection->Text);
	double compression = Convert::ToDouble(tbCompression->Text);
	double stretch = Convert::ToDouble(tbStretch->Text);
	double accuracy = Convert::ToDouble(tbAccuracy->Text);
	int iterations = Convert::ToInt32(tbIterations->Text);

	struct img* mainImage = GetImageCopy(MainImage);
	struct img* subImage = GetImageCopy(SubImage);
	ShowImage(mainImage, pbMain);
	ShowImage(subImage, pbSub);

	struct point found_p;
	clock_t start = clock();
	if (rbCuda->Checked)
	{
		found_p = CalculateCUDA(mainImage, subImage, reflection, compression, stretch, accuracy, iterations);
	}
	else if (rbOpenMP->Checked)
	{
		found_p = CalculateOpenMP(mainImage, subImage, reflection, compression, stretch, accuracy, iterations);
	}
	else
	{
		found_p = Calculate(mainImage, subImage, reflection, compression, stretch, accuracy, iterations);
	}
	clock_t end = clock();
	clock_t time = end - start;
	double leadTime = time / 1000.0;

	ShowResult(found_p, leadTime);
	ShowFoundImage(mainImage, subImage, found_p);
	//ShowFoundImage(MainImage, SubImage, found_p);
}

void DeformablePolyhedronMethod::MainForm::ChangeTheme(Color color)
{
	for each (Control ^ control in this->Controls)
		control->BackColor = color;
}

Void DeformablePolyhedronMethod::MainForm::
	menuLightTheme_Click(System::Object^ sender, System::EventArgs^ e)
{
	ChangeTheme(lightThemeColor);
}

Void DeformablePolyhedronMethod::MainForm::
	menuDarkTheme_Click(System::Object^ sender, System::EventArgs^ e)
{
	ChangeTheme(darkThemeColor);
}

Void DeformablePolyhedronMethod::MainForm::
	menuBlueTheme_Click(System::Object^ sender, System::EventArgs^ e)
{
	ChangeTheme(blueThemeColor);
}

Void DeformablePolyhedronMethod::MainForm::
	menuLCoralTheme_Click(System::Object^ sender, System::EventArgs^ e)
{
	ChangeTheme(coralThemeColor);
}

Void DeformablePolyhedronMethod::MainForm::
	menuGreenTheme_Click(System::Object^ sender, System::EventArgs^ e)
{
	ChangeTheme(greenThemeColor);
}

Void DeformablePolyhedronMethod::MainForm::
	menuOpenMain_Click(System::Object^ sender, System::EventArgs^ e)
{
	ofdImage->Filter = "Images (bmp, jpg, jpeg) | *.jpg;*.bmp;*.jpeg";
	ofdImage->Title = "Select image file";
	if (ofdImage->ShowDialog() == System::Windows::Forms::DialogResult::OK)
	{
		pbMain->Image = Bitmap::FromFile(ofdImage->FileName);

		if (pbSub->Image != nullptr)
			menuCalculate->Enabled = true;

		InitializeImage(MainImage, pbMain);
	}
}

Void DeformablePolyhedronMethod::MainForm::
	menuOpenSub_Click(System::Object^ sender, System::EventArgs^ e)
{
	ofdImage->Filter = "Images (bmp, jpg, jpeg) | *.jpg;*.bmp;*.jpeg";
	ofdImage->Title = "Select sub image file";
	if (ofdImage->ShowDialog() == System::Windows::Forms::DialogResult::OK)
	{
		pbSub->Image = Bitmap::FromFile(ofdImage->FileName);

		if (pbMain->Image != nullptr)
			menuCalculate->Enabled = true;

		InitializeImage(SubImage, pbSub);
	}
}

Void DeformablePolyhedronMethod::MainForm::
	menuExit_Click(System::Object^ sender, System::EventArgs^ e)
{
	this->Close();
}