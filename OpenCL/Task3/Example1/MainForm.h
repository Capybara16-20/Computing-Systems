#include <CL/cl.h>
#include <fstream>
#include <string.h>

#pragma once
namespace Example1 
{

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
	private: System::Windows::Forms::ToolStripMenuItem^ openToolStripMenuItem;
	private: System::Windows::Forms::ToolStripMenuItem^ menuOpen;
	private: System::Windows::Forms::ToolStripMenuItem^ menuSave;
	private: System::Windows::Forms::ToolStripMenuItem^ menuInitialize;
	private: System::Windows::Forms::ToolStripMenuItem^ menuCalculate;
	private: System::Windows::Forms::ToolStripSeparator^ toolStripSeparator1;
	private: System::Windows::Forms::ToolStripMenuItem^ menuExit;
	private: System::Windows::Forms::SplitContainer^ scPictures;
	private: System::Windows::Forms::Panel^ pSourse;
	private: System::Windows::Forms::Panel^ pResult;
	private: System::Windows::Forms::PictureBox^ pbSource;
	private: System::Windows::Forms::PictureBox^ pbResult;
	private: System::Windows::Forms::OpenFileDialog^ ofdSource;

	private: System::Windows::Forms::SaveFileDialog^ sfdResult;
	protected:

	protected:

	private:
		cl_device_id* devices;
		System::ComponentModel::Container ^components;

#pragma region Windows Form Designer generated code
		void InitializeComponent(void)
		{
			this->menu = (gcnew System::Windows::Forms::MenuStrip());
			this->openToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->menuOpen = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->menuSave = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->menuInitialize = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->menuCalculate = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->toolStripSeparator1 = (gcnew System::Windows::Forms::ToolStripSeparator());
			this->menuExit = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->scPictures = (gcnew System::Windows::Forms::SplitContainer());
			this->pSourse = (gcnew System::Windows::Forms::Panel());
			this->pbSource = (gcnew System::Windows::Forms::PictureBox());
			this->pResult = (gcnew System::Windows::Forms::Panel());
			this->pbResult = (gcnew System::Windows::Forms::PictureBox());
			this->ofdSource = (gcnew System::Windows::Forms::OpenFileDialog());
			this->sfdResult = (gcnew System::Windows::Forms::SaveFileDialog());
			this->menu->SuspendLayout();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->scPictures))->BeginInit();
			this->scPictures->Panel1->SuspendLayout();
			this->scPictures->Panel2->SuspendLayout();
			this->scPictures->SuspendLayout();
			this->pSourse->SuspendLayout();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->pbSource))->BeginInit();
			this->pResult->SuspendLayout();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->pbResult))->BeginInit();
			this->SuspendLayout();
			// 
			// menu
			// 
			this->menu->Items->AddRange(gcnew cli::array< System::Windows::Forms::ToolStripItem^  >(1) { this->openToolStripMenuItem });
			this->menu->Location = System::Drawing::Point(0, 0);
			this->menu->Name = L"menu";
			this->menu->Size = System::Drawing::Size(684, 24);
			this->menu->TabIndex = 0;
			this->menu->Text = L"menuStrip1";
			// 
			// openToolStripMenuItem
			// 
			this->openToolStripMenuItem->DropDownItems->AddRange(gcnew cli::array< System::Windows::Forms::ToolStripItem^  >(6) {
				this->menuOpen,
					this->menuSave, this->menuInitialize, this->menuCalculate, this->toolStripSeparator1, this->menuExit
			});
			this->openToolStripMenuItem->Name = L"openToolStripMenuItem";
			this->openToolStripMenuItem->Size = System::Drawing::Size(37, 20);
			this->openToolStripMenuItem->Text = L"File";
			// 
			// menuOpen
			// 
			this->menuOpen->Name = L"menuOpen";
			this->menuOpen->Size = System::Drawing::Size(155, 22);
			this->menuOpen->Text = L"Open";
			this->menuOpen->Click += gcnew System::EventHandler(this, &MainForm::menuOpen_Click);
			// 
			// menuSave
			// 
			this->menuSave->Enabled = false;
			this->menuSave->Name = L"menuSave";
			this->menuSave->Size = System::Drawing::Size(155, 22);
			this->menuSave->Text = L"Save";
			this->menuSave->Click += gcnew System::EventHandler(this, &MainForm::menuSave_Click);
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
			// scPictures
			// 
			this->scPictures->Dock = System::Windows::Forms::DockStyle::Fill;
			this->scPictures->Location = System::Drawing::Point(0, 24);
			this->scPictures->Name = L"scPictures";
			// 
			// scPictures.Panel1
			// 
			this->scPictures->Panel1->Controls->Add(this->pSourse);
			// 
			// scPictures.Panel2
			// 
			this->scPictures->Panel2->Controls->Add(this->pResult);
			this->scPictures->Size = System::Drawing::Size(684, 337);
			this->scPictures->SplitterDistance = 340;
			this->scPictures->TabIndex = 1;
			// 
			// pSourse
			// 
			this->pSourse->Controls->Add(this->pbSource);
			this->pSourse->Dock = System::Windows::Forms::DockStyle::Fill;
			this->pSourse->Location = System::Drawing::Point(0, 0);
			this->pSourse->Name = L"pSourse";
			this->pSourse->Size = System::Drawing::Size(340, 337);
			this->pSourse->TabIndex = 0;
			// 
			// pbSource
			// 
			this->pbSource->Dock = System::Windows::Forms::DockStyle::Fill;
			this->pbSource->Location = System::Drawing::Point(0, 0);
			this->pbSource->Name = L"pbSource";
			this->pbSource->Size = System::Drawing::Size(340, 337);
			this->pbSource->SizeMode = System::Windows::Forms::PictureBoxSizeMode::Zoom;
			this->pbSource->TabIndex = 0;
			this->pbSource->TabStop = false;
			// 
			// pResult
			// 
			this->pResult->Controls->Add(this->pbResult);
			this->pResult->Dock = System::Windows::Forms::DockStyle::Fill;
			this->pResult->Location = System::Drawing::Point(0, 0);
			this->pResult->Name = L"pResult";
			this->pResult->Size = System::Drawing::Size(340, 337);
			this->pResult->TabIndex = 0;
			// 
			// pbResult
			// 
			this->pbResult->Dock = System::Windows::Forms::DockStyle::Fill;
			this->pbResult->Location = System::Drawing::Point(0, 0);
			this->pbResult->Name = L"pbResult";
			this->pbResult->Size = System::Drawing::Size(340, 337);
			this->pbResult->SizeMode = System::Windows::Forms::PictureBoxSizeMode::Zoom;
			this->pbResult->TabIndex = 0;
			this->pbResult->TabStop = false;
			// 
			// ofdSource
			// 
			this->ofdSource->FileName = L"openFileDialog1";
			// 
			// MainForm
			// 
			this->AutoScaleDimensions = System::Drawing::SizeF(6, 13);
			this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
			this->ClientSize = System::Drawing::Size(684, 361);
			this->Controls->Add(this->scPictures);
			this->Controls->Add(this->menu);
			this->MainMenuStrip = this->menu;
			this->MaximizeBox = false;
			this->MaximumSize = System::Drawing::Size(700, 400);
			this->MinimizeBox = false;
			this->MinimumSize = System::Drawing::Size(700, 400);
			this->Name = L"MainForm";
			this->StartPosition = System::Windows::Forms::FormStartPosition::CenterScreen;
			this->Text = L"MainForm";
			this->menu->ResumeLayout(false);
			this->menu->PerformLayout();
			this->scPictures->Panel1->ResumeLayout(false);
			this->scPictures->Panel2->ResumeLayout(false);
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->scPictures))->EndInit();
			this->scPictures->ResumeLayout(false);
			this->pSourse->ResumeLayout(false);
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->pbSource))->EndInit();
			this->pResult->ResumeLayout(false);
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->pbResult))->EndInit();
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
		System::Void menuSave_Click(System::Object^ sender, System::EventArgs^ e) 
		{
			sfdResult->Title = "Save image";
			sfdResult->Filter = "Images (*.bmp, *.jpg, *.jpeg) |* .jpg; *.bmp; *.jpeg";
			if (sfdResult->ShowDialog() == System::Windows::Forms::DialogResult::OK)
			{
				Bitmap^ bitmap = gcnew Bitmap(pbResult->Image);
				bitmap->Save(sfdResult->FileName);
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
			const char* fileName = "example1.cl";
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
			Bitmap^ bmpInputImage = gcnew Bitmap(pbSource->Image);
			unsigned char* inputImagePixels = new unsigned char[imageHeight *
				imageWidth * 4];
			unsigned char* ch = &inputImagePixels[0];
			for (int i = 0; i < imageHeight; i++)
			{
				for (int j = 0; j < imageWidth; j++)
				{
					*ch++ = bmpInputImage->GetPixel(j, i).A;
					*ch++ = bmpInputImage->GetPixel(j, i).R;
					*ch++ = bmpInputImage->GetPixel(j, i).G;
					*ch++ = bmpInputImage->GetPixel(j, i).B;
				}
			}
			cl_mem inputImage2DBuffer = clCreateImage2D(context,
				CL_MEM_COPY_HOST_PTR | CL_MEM_READ_ONLY,
				imageFormat, imageWidth, imageHeight, 0,
				(void*)inputImagePixels, &status);
			cl_mem outputImage2DBuffer = clCreateImage2D(context,
				CL_MEM_WRITE_ONLY, imageFormat, imageWidth,
				imageHeight, 0, NULL, &status);
			cl_kernel kernel = clCreateKernel(program, "example1", &status);
			status = clSetKernelArg(kernel, 0, sizeof(cl_mem),
					(void*)&inputImage2DBuffer);
			status = clSetKernelArg(kernel, 1, sizeof(cl_mem),
				(void*)&outputImage2DBuffer);
			size_t global_work_size[] = { imageWidth, imageHeight };
			status = clEnqueueNDRangeKernel(commandQueue, kernel, 2, NULL,
				global_work_size, NULL, 0, NULL, NULL);
			size_t origin[3] = { 0, 0, 0 };
			size_t region[3] = { imageWidth, imageHeight, 1 };
			unsigned char* outputImagePixels = new unsigned char[imageWidth *
				imageHeight * 4];
			status = clEnqueueReadImage(commandQueue, outputImage2DBuffer,
				CL_TRUE, origin, region, 0, 0, outputImagePixels, 0, NULL, NULL);
			Bitmap^ bmpOutputImage = gcnew Bitmap(imageWidth, imageHeight);
			unsigned char* p = &outputImagePixels[0];
			for (int i = 0; i < imageHeight; i++)
			{
				for (int j = 0; j < imageWidth; j++)
				{
					unsigned int temp = *p++ << 24;
					temp |= (*p++ << 16);
					temp |= (*p++ << 8);
					temp |= *p++;
					bmpOutputImage->SetPixel(j, i, Color::FromArgb(temp));
				}
			}
			pbResult->Image = bmpOutputImage;
			menuSave->Enabled = true;
			clReleaseMemObject(outputImage2DBuffer);
			clReleaseMemObject(inputImage2DBuffer);
			delete[]inputImagePixels;
			delete[]outputImagePixels;
			clReleaseKernel(kernel);
			clReleaseCommandQueue(commandQueue);
			clReleaseContext(context);
			clReleaseProgram(program);
		}
	};
}
