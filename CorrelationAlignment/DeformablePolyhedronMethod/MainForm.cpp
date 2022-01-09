#include "MainForm.h"
#include<windows.h>
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
}

void DeformablePolyhedronMethod::MainForm::ShowResult(String^ message)
{
	this->tbResult->Text = message;
}

void DeformablePolyhedronMethod::MainForm::Calculate()
{

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
	}
}

Void DeformablePolyhedronMethod::MainForm::
	menuCalculate_Click(System::Object^ sender, System::EventArgs^ e)
{
	struct img* mainImage = new img();
	struct img* subImage = new img();
	InitializeImage(mainImage, pbMain);
	InitializeImage(subImage, pbSub);

	/*
	// Получаем доступ к функции из DLL библиотеки
	HINSTANCE hGetProcIDDLL = LoadLibrary(L"Example1CUDA.dll");
	if (!hGetProcIDDLL)
		throw gcnew Exception();
	typedef int(__stdcall* function)(void*);
	function calc = (function)GetProcAddress(hGetProcIDDLL, "calculate");
	if (!calc)
		throw gcnew Exception();
	// Выполняем функцию из DLL библиотеки
	mainImage = (img*)calc((void*)mainImage);
	// Формируем результат для отображения в CLR компоненте
	bitmap = gcnew Bitmap(image->width, image->height);
	for (int j = 0; j < image->height; j++)
		for (int i = 0; i < image->width; i++)
			bitmap->SetPixel(i, j, Color::FromArgb(image->image[j * image->width + i]));
	pbResult->Image = bitmap;
	FreeLibrary(hGetProcIDDLL);*/

	Calculate();
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

Void DeformablePolyhedronMethod::MainForm::
	menuExit_Click(System::Object^ sender, System::EventArgs^ e)
{
	this->Close();
}