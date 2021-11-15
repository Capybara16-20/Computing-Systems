#include <CL/cl.h>
#include <fstream>
#include <string.h>
#include "ResultForm.h"
#pragma once

namespace Example2 {

	using namespace System;
	using namespace System::ComponentModel;
	using namespace System::Collections;
	using namespace System::Windows::Forms;
	using namespace System::Data;
	using namespace System::Drawing;

	public ref class MainForm : public System::Windows::Forms::Form
	{
	public:
		MainForm(void)
		{
			InitializeComponent();
		}

	protected:
		~MainForm()
		{
			if (components)
			{
				delete components;
			}
		}
	private: System::Windows::Forms::MenuStrip^ menu;
	protected:
	private: System::Windows::Forms::ToolStripMenuItem^ fileToolStripMenuItem;
	private: System::Windows::Forms::ToolStripMenuItem^ menuOpen;
	private: System::Windows::Forms::ToolStripMenuItem^ menuInitialize;
	private: System::Windows::Forms::ToolStripMenuItem^ menuCalculate;
	private: System::Windows::Forms::ToolStripSeparator^ toolStripSeparator1;
	private: System::Windows::Forms::ToolStripMenuItem^ menuExit;
	private: System::Windows::Forms::OpenFileDialog^ ofdSource;
	private: System::Windows::Forms::PictureBox^ pbSource;

	private:
		cl_device_id* devices;
		System::ComponentModel::Container^ components;

#pragma region Windows Form Designer generated code
		void InitializeComponent(void)
		{
			this->menu = (gcnew System::Windows::Forms::MenuStrip());
			this->fileToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->menuOpen = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->menuInitialize = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->menuCalculate = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->toolStripSeparator1 = (gcnew System::Windows::Forms::ToolStripSeparator());
			this->menuExit = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->ofdSource = (gcnew System::Windows::Forms::OpenFileDialog());
			this->pbSource = (gcnew System::Windows::Forms::PictureBox());
			this->menu->SuspendLayout();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->pbSource))->BeginInit();
			this->SuspendLayout();
			// 
			// menu
			// 
			this->menu->Items->AddRange(gcnew cli::array< System::Windows::Forms::ToolStripItem^  >(1) { this->fileToolStripMenuItem });
			this->menu->Location = System::Drawing::Point(0, 0);
			this->menu->Name = L"menu";
			this->menu->Size = System::Drawing::Size(684, 24);
			this->menu->TabIndex = 0;
			this->menu->Text = L"menuStrip1";
			// 
			// fileToolStripMenuItem
			// 
			this->fileToolStripMenuItem->DropDownItems->AddRange(gcnew cli::array< System::Windows::Forms::ToolStripItem^  >(5) {
				this->menuOpen,
					this->menuInitialize, this->menuCalculate, this->toolStripSeparator1, this->menuExit
			});
			this->fileToolStripMenuItem->Name = L"fileToolStripMenuItem";
			this->fileToolStripMenuItem->Size = System::Drawing::Size(37, 20);
			this->fileToolStripMenuItem->Text = L"File";
			// 
			// menuOpen
			// 
			this->menuOpen->Name = L"menuOpen";
			this->menuOpen->Size = System::Drawing::Size(155, 22);
			this->menuOpen->Text = L"Open";
			this->menuOpen->Click += gcnew System::EventHandler(this, &MainForm::menuOpen_Click);
			// 
			// menuInitialize
			// 
			this->menuInitialize->Name = L"menuInitialize";
			this->menuInitialize->Size = System::Drawing::Size(155, 22);
			this->menuInitialize->Text = L"Initialize Device";
			this->menuInitialize->Click += gcnew System::EventHandler(this, &MainForm::menuInitialize_Click);
			// 
			// menuCalculate
			// 
			this->menuCalculate->Enabled = false;
			this->menuCalculate->Name = L"menuCalculate";
			this->menuCalculate->Size = System::Drawing::Size(155, 22);
			this->menuCalculate->Text = L"Calculate";
			this->menuCalculate->Click += gcnew System::EventHandler(this, &MainForm::menuCalculate_Click);
			// 
			// toolStripSeparator1
			// 
			this->toolStripSeparator1->Name = L"toolStripSeparator1";
			this->toolStripSeparator1->Size = System::Drawing::Size(152, 6);
			// 
			// menuExit
			// 
			this->menuExit->Name = L"menuExit";
			this->menuExit->Size = System::Drawing::Size(155, 22);
			this->menuExit->Text = L"Exit";
			this->menuExit->Click += gcnew System::EventHandler(this, &MainForm::menuExit_Click);
			// 
			// ofdSource
			// 
			this->ofdSource->FileName = L"openFileDialog1";
			// 
			// pbSource
			// 
			this->pbSource->Dock = System::Windows::Forms::DockStyle::Fill;
			this->pbSource->Location = System::Drawing::Point(0, 24);
			this->pbSource->Name = L"pbSource";
			this->pbSource->Size = System::Drawing::Size(684, 437);
			this->pbSource->SizeMode = System::Windows::Forms::PictureBoxSizeMode::Zoom;
			this->pbSource->TabIndex = 1;
			this->pbSource->TabStop = false;
			// 
			// MainForm
			// 
			this->AutoScaleDimensions = System::Drawing::SizeF(6, 13);
			this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
			this->ClientSize = System::Drawing::Size(684, 461);
			this->Controls->Add(this->pbSource);
			this->Controls->Add(this->menu);
			this->MainMenuStrip = this->menu;
			this->MaximizeBox = false;
			this->MaximumSize = System::Drawing::Size(700, 500);
			this->MinimizeBox = false;
			this->MinimumSize = System::Drawing::Size(700, 500);
			this->Name = L"MainForm";
			this->StartPosition = System::Windows::Forms::FormStartPosition::CenterScreen;
			this->Text = L"RGB Filter";
			this->menu->ResumeLayout(false);
			this->menu->PerformLayout();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->pbSource))->EndInit();
			this->ResumeLayout(false);
			this->PerformLayout();

		}
#pragma endregion
	private:
		System::Void menuOpen_Click(System::Object^ sender, System::EventArgs^ e)
		{
			ofdSource->Filter = "Images (bmp, jpg, jpeg) | *.jpg;*.bmp;*.jpeg";
			ofdSource->Title = "Select image file";
			if (ofdSource->ShowDialog() == System::Windows::Forms::DialogResult::OK)
			{
				pbSource->Image = Bitmap::FromFile(ofdSource->FileName);
			}
		}
	private:
		System::Void menuInitialize_Click(System::Object^ sender, System::EventArgs^ e)
		{
			configureDevices();
		}
	private:
		System::Void menuCalculate_Click(System::Object^ sender, System::EventArgs^ e)
		{
			calculate();
		}
	private:
		System::Void menuExit_Click(System::Object^ sender, System::EventArgs^ e)
		{
			this->Close();
		}

		void configureDevices()
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
			status = clGetDeviceIDs(platform, CL_DEVICE_TYPE_ALL, 0, NULL, &numberOfDevices);
			if (status != CL_SUCCESS)
			{
				System::Windows::Forms::MessageBox::Show("Device not found!",
					"Error", System::Windows::Forms::MessageBoxButtons::OK);
				return;
			}
			this->devices = new cl_device_id[numberOfDevices];
			status = clGetDeviceIDs(platform, CL_DEVICE_TYPE_ALL,
				numberOfDevices, this->devices, NULL);
			if (status != CL_SUCCESS)
			{
				System::Windows::Forms::MessageBox::Show("Device not found!",
					"Error", System::Windows::Forms::MessageBoxButtons::OK);
				return;
			}

			menuCalculate->Enabled = true;
			System::Windows::Forms::MessageBox::Show("Device initialized!",
				"Success", System::Windows::Forms::MessageBoxButtons::OK);
		}

		int convertToString(const char* fileName, std::string& s)
		{
			size_t size;
			char* str;
			std::fstream f(fileName, (std::fstream::in |
				std::fstream::binary));
			if (f.is_open())
			{
				size_t fileSize;
				f.seekg(0, std::fstream::end);
				size = fileSize = (size_t)f.tellg();
				f.seekg(0, std::fstream::beg);
				str = new char[size + 1];
				if (!str) {
					f.close();
					return 0;
				}
				f.read(str, fileSize);
				f.close();
				str[size] = '\0';
				s = str;
				delete[]str;
				return 0;
			}
			return -1;
		}

		void calculate()
		{
			cl_int status = 0;
			cl_context context = clCreateContext(NULL, 1, devices, NULL, NULL,
				NULL);
			cl_command_queue commandQueue = clCreateCommandQueue(context,
				devices[0], 0, NULL);
			const char* fileName = "example2.cl";
			std::string sourceCode;
			status = convertToString(fileName, sourceCode);
			const char* source = sourceCode.c_str();
			size_t sizeOfSource[] = { strlen(source) };
			cl_program program = clCreateProgramWithSource(context, 1, &source,
				sizeOfSource, &status);
			status = clBuildProgram(program, 1, devices, NULL, NULL, NULL);
			cl_image_format* imageFormat = new cl_image_format;
			imageFormat->image_channel_order = CL_ARGB;
			imageFormat->image_channel_data_type = CL_UNSIGNED_INT8;
			size_t imageWidth = pbSource->Image->Width;
			size_t imageHeight = pbSource->Image->Height;
			Bitmap^ bmpImage = gcnew Bitmap(pbSource->Image);
			unsigned char* imagePixels = new unsigned char[imageHeight *
				imageWidth * 4];
			unsigned char* ch = &imagePixels[0];
			for (int i = 0; i < imageHeight; i++)
			{
				for (int j = 0; j < imageWidth; j++)
				{
					*ch++ = bmpImage->GetPixel(j, i).A;
					*ch++ = bmpImage->GetPixel(j, i).R;
					*ch++ = bmpImage->GetPixel(j, i).G;
					*ch++ = bmpImage->GetPixel(j, i).B;
				}
			}

			cl_mem inputImage2DBuffer = clCreateImage2D(context,
				CL_MEM_COPY_HOST_PTR | CL_MEM_READ_ONLY,
				imageFormat, imageWidth, imageHeight, 0,
				(void*)imagePixels, &status);
			cl_mem redImage2DBuffer = clCreateImage2D(context,
				CL_MEM_WRITE_ONLY, imageFormat, imageWidth,
				imageHeight, 0, NULL, &status);
			cl_mem greenImage2DBuffer = clCreateImage2D(context,
				CL_MEM_WRITE_ONLY, imageFormat, imageWidth,
				imageHeight, 0, NULL, &status);
			cl_mem blueImage2DBuffer = clCreateImage2D(context,
				CL_MEM_WRITE_ONLY, imageFormat, imageWidth,
				imageHeight, 0, NULL, &status);

			cl_kernel kernel = clCreateKernel(program, "example2", &status);
			status = clSetKernelArg(kernel, 0, sizeof(cl_mem),
				(void*)&inputImage2DBuffer);
			status = clSetKernelArg(kernel, 1, sizeof(cl_mem),
				(void*)&redImage2DBuffer);
			status = clSetKernelArg(kernel, 2, sizeof(cl_mem),
				(void*)&greenImage2DBuffer);
			status = clSetKernelArg(kernel, 3, sizeof(cl_mem),
				(void*)&blueImage2DBuffer);

			size_t global_work_size[] = { imageWidth, imageHeight };
			status = clEnqueueNDRangeKernel(commandQueue, kernel, 2, NULL,
				global_work_size, NULL, 0, NULL, NULL);

			unsigned char* redImagePixels = new unsigned char[imageWidth *
				imageHeight * 4];
			unsigned char* greenImagePixels = new unsigned char[imageWidth *
				imageHeight * 4];
			unsigned char* blueImagePixels = new unsigned char[imageWidth *
				imageHeight * 4];
			size_t origin[3] = { 0, 0, 0 };
			size_t region[3] = { imageWidth, imageHeight, 1 };
			status = clEnqueueReadImage(commandQueue, redImage2DBuffer,
				CL_TRUE, origin, region, 0, 0, redImagePixels, 0, NULL, NULL);
			status = clEnqueueReadImage(commandQueue, greenImage2DBuffer,
				CL_TRUE, origin, region, 0, 0, greenImagePixels, 0, NULL, NULL);
			status = clEnqueueReadImage(commandQueue, blueImage2DBuffer,
				CL_TRUE, origin, region, 0, 0, blueImagePixels, 0, NULL, NULL);

			Bitmap^ bmpRedImage = GetImage(imageWidth, imageHeight, &redImagePixels[0]);
			Bitmap^ bmpGreenImage = GetImage(imageWidth, imageHeight, &greenImagePixels[0]);
			Bitmap^ bmpBlueImage = GetImage(imageWidth, imageHeight, &blueImagePixels[0]);

			ResultForm^ form = gcnew ResultForm(bmpImage, bmpRedImage, bmpGreenImage, bmpBlueImage);
			form->ShowDialog();

			clReleaseMemObject(redImage2DBuffer);
			clReleaseMemObject(inputImage2DBuffer);
			delete[]imagePixels;
			delete[]redImagePixels;
			clReleaseKernel(kernel);
			clReleaseCommandQueue(commandQueue);
			clReleaseContext(context);
			clReleaseProgram(program);
		}

		Bitmap^ GetImage(size_t width, size_t height,
			unsigned char* imagePixels)
		{
			Bitmap^ bmpImage = gcnew Bitmap(width, height);
			unsigned char* pixel = &imagePixels[0];

			for (int i = 0; i < height; i++)
			{
				for (int j = 0; j < width; j++)
				{
					unsigned int temp = *pixel++ << 24;
					temp |= (*pixel++ << 16);
					temp |= (*pixel++ << 8);
					temp |= *pixel++;
					bmpImage->SetPixel(j, i, Color::FromArgb(temp));
				}
			}

			return bmpImage;
		}
	};
}
