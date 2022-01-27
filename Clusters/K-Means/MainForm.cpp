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
	KMeans::MainForm form;
	Application::Run(% form);

	srand(time(NULL));
}

void KMeans::MainForm::InitializeImage(struct KMeans::img* image, PictureBox^ pbSource)
{
	image->width = pbSource->Image->Width;
	image->height = pbSource->Image->Height;
	image->image = new unsigned[image->width * image->height];
	System::Drawing::Bitmap^ bitmap = gcnew System::Drawing::Bitmap(pbSource->Image);
	for (int j = 0; j < image->height; j++)
		for (int i = 0; i < image->width; i++)
			image->image[j * image->width + i] = bitmap->GetPixel(i, j).ToArgb();
}

struct KMeans::img* KMeans::MainForm::GetImageCopy(struct KMeans::img* image)
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

void KMeans::MainForm::ShowImage(struct KMeans::img* image, PictureBox^ pictureBox)
{
	Bitmap^ bitmap = gcnew Bitmap(image->width, image->height);
	for (int j = 0; j < image->height; j++)
		for (int i = 0; i < image->width; i++)
			bitmap->SetPixel(i, j, Color::FromArgb(image->image[j * image->width + i]));
	pictureBox->Image = bitmap;
}

void KMeans::MainForm::ShowResult(double leadTime)
{
	String^ leadTime_str = "Lead time: " + (leadTime).ToString() + " (sec).";
	tbResult->Text = leadTime_str;
}

struct KMeans::point KMeans::MainForm::GetRandomPoint(int maxX, int maxY)
{
	int x = rand() % maxX;
	int y = rand() % maxY;

	point newPoint;
	newPoint.x = x;
	newPoint.y = y;

	return newPoint;
}

long double KMeans::MainForm::GetEuclideanDistance(unsigned color1, unsigned color2)
{
	unsigned red1 = color1 & 0x00FF0000 >> 0x10;
	unsigned green1 = color1 & 0x0000FF00 >> 0x8;
	unsigned blue1 = color1 & 0x000000FF;
	unsigned red2 = color2 & 0x00FF0000 >> 0x10;
	unsigned green2 = color2 & 0x0000FF00 >> 0x8;
	unsigned blue2 = color2 & 0x000000FF;

	return sqrt(pow(red1 - red2, 2.0) + pow(green1 - green2, 2.0) + pow(blue1 - blue2, 2.0));
}

struct KMeans::img* KMeans::MainForm::Calculate(struct KMeans::img* image, int clustersCount)
{
	img* clusteredImage = image;
	unsigned* img = clusteredImage->image;
	int width = clusteredImage->width;
	int height = clusteredImage->height;

	int* pointsClusters = new int[width * height];
	for (int i = 0; i < width * height; i++)
		pointsClusters[i] = -1;

	//Получаем случайные кластеры
	struct point* centers = new point[clustersCount];
	for (int i = 0; i < clustersCount; i++)
		centers[i] = GetRandomPoint(width, height);

	bool complited = false;
	while (!complited) //Пока не прекратятся изменения
	{
		int changesCount = 0;

		for (int i = 0; i < height; i++)
		{
			for (int j = 0; j < width; j++)
			{
				//Для каждой точки
				unsigned current_color = img[i * width + j];

				//Ищем ближайший кластер
				unsigned cluster_color = img[centers[0].y * width + centers[0].x];
				int nearestCluster_id = 0;

				long double minDistance = GetEuclideanDistance(current_color, cluster_color);
				for (int k = 1; k < clustersCount; k++)
				{
					cluster_color = img[centers[k].y * width + centers[k].x];
					long double cl_distance = GetEuclideanDistance(current_color, cluster_color);
					if (cl_distance < minDistance)
					{
						minDistance = cl_distance;
						nearestCluster_id = k;
					}
				}

				//Помещаем точку в ближайший кластер
				int currentCluster = pointsClusters[i * width + j];
				if (nearestCluster_id != currentCluster)
				{
					pointsClusters[i * width + j] = nearestCluster_id;
					changesCount++;
				}
			}
		}

		if (changesCount == 0) //Если изменений не было
			complited = true; //то точки распределены по кластерам
	}

	//Вычисляем цвета каждого кластера
	unsigned* colors = new unsigned[clustersCount];
	for (int i = 0; i < clustersCount; i++)
		colors[i] = 0;
	for (int i = 0; i < width * height; i++)
	{
		unsigned pixel = img[i];
		int cluster = pointsClusters[i];

		colors[cluster] |= pixel;
		//colors[cluster] |= (pixel & 0x00FF0000 >> 0x10 + pixel & 0x0000FF00 >> 0x8 + pixel & 0x000000FF);
	}
	for (int i = 0; i < clustersCount; i++)
		colors[i] = 0xFF000000 | colors[i];
		//colors[i] = 0xFF000000 | (colors[i] << 0x10) | (colors[i] << 0x8) | colors[i];

	//Перекраска исходного изображения
	for (int i = 0; i < width * height; i++)
		img[i] = colors[pointsClusters[i]];

	return clusteredImage;
}

struct KMeans::img* KMeans::MainForm::CalculateOpenMP(struct KMeans::img* image, int clustersCount)
{
	img* clusteredImage = image;
	unsigned* img = clusteredImage->image;
	int width = clusteredImage->width;
	int height = clusteredImage->height;

	int* pointsClusters = new int[width * height];
#pragma omp parallel for
	for (int i = 0; i < width * height; i++)
		pointsClusters[i] = -1;

	//Получаем случайные кластеры
	struct point* centers = new point[clustersCount];
#pragma omp parallel for
	for (int i = 0; i < clustersCount; i++)
		centers[i] = GetRandomPoint(width, height);

	bool complited = false;
	while (!complited) //Пока не прекратятся изменения
	{
		int changesCount = 0;
		unsigned current_color;
		unsigned cluster_color;
		int nearestCluster_id;
		long double minDistance;
		int currentCluster;

#pragma omp parallel firstprivate(changesCount) private(current_color, cluster_color, nearestCluster_id, minDistance, currentCluster) shared(complited)
		{
#pragma omp for
			for (int i = 0; i < height; i++)
			{
				for (int j = 0; j < width; j++)
				{
					//Для каждой точки
					current_color = img[i * width + j];

					//Ищем ближайший кластер
					cluster_color = img[centers[0].y * width + centers[0].x];
					nearestCluster_id = 0;

					minDistance = GetEuclideanDistance(current_color, cluster_color);
					for (int k = 1; k < clustersCount; k++)
					{
						cluster_color = img[centers[k].y * width + centers[k].x];
						long double cl_distance = GetEuclideanDistance(current_color, cluster_color);
						if (cl_distance < minDistance)
						{
							minDistance = cl_distance;
							nearestCluster_id = k;
						}
					}

					//Помещаем точку в ближайший кластер
					currentCluster = pointsClusters[i * width + j];
					if (nearestCluster_id != currentCluster)
					{
						pointsClusters[i * width + j] = nearestCluster_id;
						changesCount++;
					}
				}
			}
		}

		if (changesCount == 0) //Если изменений не было
			complited = true; //то точки распределены по кластерам
	}

	//Вычисляем цвета каждого кластера
	unsigned* colors = new unsigned[clustersCount];
#pragma omp for
	for (int i = 0; i < clustersCount; i++)
		colors[i] = 0xFF000000;
#pragma omp for
	for (int i = 0; i < width * height; i++)
	{
		unsigned pixel = img[i];
		int cluster = pointsClusters[i];

		colors[cluster] |= pixel;
		//colors[cluster] |= (pixel & 0x00FF0000 >> 0x10 + pixel & 0x0000FF00 >> 0x8 + pixel & 0x000000FF);
	}
#pragma omp for
	for (int i = 0; i < clustersCount; i++)
		colors[i] = 0xFF000000 | colors[i];
		//colors[i] = 0xFF000000 | (colors[i] << 0x10) | (colors[i] << 0x8) | colors[i];

	//Перекраска исходного изображения
#pragma omp for
	for (int i = 0; i < width * height; i++)
		img[i] = colors[pointsClusters[i]];

	return clusteredImage;
}

struct KMeans::img* KMeans::MainForm::CalculateCUDA(struct KMeans::img* image, int clustersCount)
{
	img* clusteredImage = image;

	HINSTANCE hGetProcIDDLL = LoadLibrary(L"KMeansCUDA.dll");
	if (!hGetProcIDDLL)
		throw gcnew Exception();
	typedef int(__stdcall* function)(void*, int);
	function calc = (function)GetProcAddress(hGetProcIDDLL, "calculate");
	if (!calc)
		throw gcnew Exception();
	clusteredImage = (img*)calc((void*)image, clustersCount);
	FreeLibrary(hGetProcIDDLL);

	return clusteredImage;
}

Void KMeans::MainForm::menuCalculate_Click(System::Object^ sender, System::EventArgs^ e)
{
	int clustersCount = Convert::ToInt32(nudClustersCount->Value);

	struct img* image = GetImageCopy(Image);
	ShowImage(image, pbPicture);

	struct img* clusteredImage;
	clock_t start = clock();
	if (rbLinear->Checked)
	{
		clusteredImage = Calculate(image, clustersCount);
	}
	else if (rbOpenMP->Checked)
	{
		clusteredImage = CalculateOpenMP(image, clustersCount);
	}
	else
	{
		clusteredImage = CalculateCUDA(image, clustersCount);
	}
	clock_t end = clock();
	clock_t time = end - start;
	double leadTime = time / 1000.0;

	ShowImage(clusteredImage, pbPicture);
	ShowResult(leadTime);
}

Void KMeans::MainForm::menuOpen_Click(System::Object^ sender, System::EventArgs^ e)
{
	ofdPicture->Filter = "Images (bmp, jpg, jpeg) | *.jpg;*.bmp;*.jpeg";
	ofdPicture->Title = "Select image file";
	if (ofdPicture->ShowDialog() == System::Windows::Forms::DialogResult::OK)
	{
		pbPicture->Image = Bitmap::FromFile(ofdPicture->FileName);
		menuCalculate->Enabled = true;

		InitializeImage(Image, pbPicture);
	}
}

Void KMeans::MainForm::menuExit_Click(System::Object^ sender, System::EventArgs^ e)
{
	this->Close();
}