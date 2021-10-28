#pragma once
#include<windows.h>

namespace Example1 {

	using namespace System;
	using namespace System::ComponentModel;
	using namespace System::Collections;
	using namespace System::Windows::Forms;
	using namespace System::Data;
	using namespace System::Drawing;
	
	struct img
	{
		int width;
		int height;
		unsigned* image;
	};

	/// <summary>
	/// Summary for MainForm
	/// </summary>
	public ref class MainForm : public System::Windows::Forms::Form
	{
	public:
		MainForm(void)
		{
			InitializeComponent();
			//
			//TODO: Add the constructor code here
			//
		}

	protected:
		/// <summary>
		/// Clean up any resources being used.
		/// </summary>
		~MainForm()
		{
			if (components)
			{
				delete components;
			}
		}
	private: System::Windows::Forms::MenuStrip^ menuStrip;
	protected:

	protected:
	private: System::Windows::Forms::ToolStripMenuItem^ fileToolStripMenuItem;
	private: System::Windows::Forms::ToolStripMenuItem^ openToolStripMenuItem;
	private: System::Windows::Forms::ToolStripMenuItem^ saveToolStripMenuItem;
	private: System::Windows::Forms::ToolStripMenuItem^ calculateToolStripMenuItem;
	private: System::Windows::Forms::ToolStripSeparator^ toolStripSeparator1;
	private: System::Windows::Forms::ToolStripMenuItem^ exitToolStripMenuItem;
	private: System::Windows::Forms::SplitContainer^ scPictures;
	private: System::Windows::Forms::PictureBox^ pbSource;
	private: System::Windows::Forms::PictureBox^ pbResult;
	private: System::Windows::Forms::OpenFileDialog^ ofdSource;
	private: System::Windows::Forms::SaveFileDialog^ sfdResult;




	private:
		/// <summary>
		/// Required designer variable.
		/// </summary>
		System::ComponentModel::Container^ components;

#pragma region Windows Form Designer generated code
		/// <summary>
		/// Required method for Designer support - do not modify
		/// the contents of this method with the code editor.
		/// </summary>
		void InitializeComponent(void)
		{
			this->menuStrip = (gcnew System::Windows::Forms::MenuStrip());
			this->fileToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->openToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->saveToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->calculateToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->toolStripSeparator1 = (gcnew System::Windows::Forms::ToolStripSeparator());
			this->exitToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->scPictures = (gcnew System::Windows::Forms::SplitContainer());
			this->pbSource = (gcnew System::Windows::Forms::PictureBox());
			this->pbResult = (gcnew System::Windows::Forms::PictureBox());
			this->ofdSource = (gcnew System::Windows::Forms::OpenFileDialog());
			this->sfdResult = (gcnew System::Windows::Forms::SaveFileDialog());
			this->menuStrip->SuspendLayout();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->scPictures))->BeginInit();
			this->scPictures->Panel1->SuspendLayout();
			this->scPictures->Panel2->SuspendLayout();
			this->scPictures->SuspendLayout();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->pbSource))->BeginInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->pbResult))->BeginInit();
			this->SuspendLayout();
			// 
			// menuStrip
			// 
			this->menuStrip->Items->AddRange(gcnew cli::array< System::Windows::Forms::ToolStripItem^  >(1) { this->fileToolStripMenuItem });
			this->menuStrip->Location = System::Drawing::Point(0, 0);
			this->menuStrip->Name = L"menuStrip";
			this->menuStrip->Size = System::Drawing::Size(684, 24);
			this->menuStrip->TabIndex = 0;
			this->menuStrip->Text = L"menuStrip1";
			// 
			// fileToolStripMenuItem
			// 
			this->fileToolStripMenuItem->DropDownItems->AddRange(gcnew cli::array< System::Windows::Forms::ToolStripItem^  >(5) {
				this->openToolStripMenuItem,
					this->saveToolStripMenuItem, this->calculateToolStripMenuItem, this->toolStripSeparator1, this->exitToolStripMenuItem
			});
			this->fileToolStripMenuItem->Name = L"fileToolStripMenuItem";
			this->fileToolStripMenuItem->Size = System::Drawing::Size(37, 20);
			this->fileToolStripMenuItem->Text = L"File";
			// 
			// openToolStripMenuItem
			// 
			this->openToolStripMenuItem->Name = L"openToolStripMenuItem";
			this->openToolStripMenuItem->Size = System::Drawing::Size(180, 22);
			this->openToolStripMenuItem->Text = L"Open";
			this->openToolStripMenuItem->Click += gcnew System::EventHandler(this, &MainForm::openToolStripMenuItem_Click);
			// 
			// saveToolStripMenuItem
			// 
			this->saveToolStripMenuItem->Name = L"saveToolStripMenuItem";
			this->saveToolStripMenuItem->Size = System::Drawing::Size(180, 22);
			this->saveToolStripMenuItem->Text = L"Save";
			this->saveToolStripMenuItem->Click += gcnew System::EventHandler(this, &MainForm::saveToolStripMenuItem_Click);
			// 
			// calculateToolStripMenuItem
			// 
			this->calculateToolStripMenuItem->Name = L"calculateToolStripMenuItem";
			this->calculateToolStripMenuItem->Size = System::Drawing::Size(180, 22);
			this->calculateToolStripMenuItem->Text = L"Calculate";
			this->calculateToolStripMenuItem->Click += gcnew System::EventHandler(this, &MainForm::calculateToolStripMenuItem_Click);
			// 
			// toolStripSeparator1
			// 
			this->toolStripSeparator1->Name = L"toolStripSeparator1";
			this->toolStripSeparator1->Size = System::Drawing::Size(177, 6);
			// 
			// exitToolStripMenuItem
			// 
			this->exitToolStripMenuItem->Name = L"exitToolStripMenuItem";
			this->exitToolStripMenuItem->Size = System::Drawing::Size(180, 22);
			this->exitToolStripMenuItem->Text = L"Exit";
			this->exitToolStripMenuItem->Click += gcnew System::EventHandler(this, &MainForm::exitToolStripMenuItem_Click);
			// 
			// scPictures
			// 
			this->scPictures->Dock = System::Windows::Forms::DockStyle::Fill;
			this->scPictures->FixedPanel = System::Windows::Forms::FixedPanel::Panel1;
			this->scPictures->IsSplitterFixed = true;
			this->scPictures->Location = System::Drawing::Point(0, 24);
			this->scPictures->Name = L"scPictures";
			// 
			// scPictures.Panel1
			// 
			this->scPictures->Panel1->Controls->Add(this->pbSource);
			// 
			// scPictures.Panel2
			// 
			this->scPictures->Panel2->Controls->Add(this->pbResult);
			this->scPictures->Size = System::Drawing::Size(684, 337);
			this->scPictures->SplitterDistance = 340;
			this->scPictures->TabIndex = 1;
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
			this->Controls->Add(this->menuStrip);
			this->MainMenuStrip = this->menuStrip;
			this->MaximizeBox = false;
			this->MaximumSize = System::Drawing::Size(700, 400);
			this->MinimizeBox = false;
			this->MinimumSize = System::Drawing::Size(700, 400);
			this->Name = L"MainForm";
			this->StartPosition = System::Windows::Forms::FormStartPosition::CenterScreen;
			this->Text = L"Photo Filter";
			this->menuStrip->ResumeLayout(false);
			this->menuStrip->PerformLayout();
			this->scPictures->Panel1->ResumeLayout(false);
			this->scPictures->Panel2->ResumeLayout(false);
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->scPictures))->EndInit();
			this->scPictures->ResumeLayout(false);
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->pbSource))->EndInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->pbResult))->EndInit();
			this->ResumeLayout(false);
			this->PerformLayout();

		}
#pragma endregion
	private:
		System::Void openToolStripMenuItem_Click(System::Object^ sender, System::EventArgs^ e)
		{
			ofdSource->Filter = "Images (bmp, jpg, jpeg) |* .jpg; *.bmp; *.jpeg";
			ofdSource->Title = "Select image file";
			if (ofdSource->ShowDialog() == System::Windows::Forms::DialogResult::OK)
				pbSource->Image = Bitmap::FromFile(ofdSource->FileName);
		};
	private: 
		System::Void saveToolStripMenuItem_Click(System::Object^ sender, System::EventArgs^ e) 
		{
			if (pbResult->Image != nullptr)
			{
				sfdResult->Title = "Save image";
				sfdResult->Filter = "Images (*.bmp, *.jpg,* .jpeg) | *.jpg; *.bmp; *.jpeg";
				if (sfdResult->ShowDialog() == System::Windows::Forms::DialogResult::OK)
				{
					Bitmap^ bitmap = gcnew Bitmap(pbResult->Image);
					bitmap->Save(sfdResult->FileName);
				}
			}
		}
	private: 
		System::Void exitToolStripMenuItem_Click(System::Object^ sender, System::EventArgs^ e) 
		{
			this->Close();
		}
	private: 
		System::Void calculateToolStripMenuItem_Click(System::Object^ sender, System::EventArgs^ e) 
		{
			// Для начала подготовим наше входное изображение для обработки на CUDA
			struct img* image = new img();
			image->width = pbSource->Image->Width;
			image->height = pbSource->Image->Height;
			image->image = new unsigned[image->width * image->height];
			Bitmap^ bitmap = gcnew Bitmap(pbSource->Image);
			for (int j = 0; j < image->height; j++)
				for (int i = 0; i < image->width; i++)
					image->image[j * image->width + i] = bitmap->GetPixel(i, j).ToArgb();
			// Получаем доступ к функции из DLL библиотеки
			HINSTANCE hGetProcIDDLL = LoadLibrary(L"Example1CUDA.dll");
			if (!hGetProcIDDLL)
				throw gcnew Exception();
			typedef int(__stdcall* function)(void*);
			function calc = (function)GetProcAddress(hGetProcIDDLL, "calculate");
			if (!calc)
				throw gcnew Exception();
			// Выполняем функцию из DLL библиотеки
			image = (img*)calc((void*)image);
			// Формируем результат для отображения в CLR компоненте
			bitmap = gcnew Bitmap(image->width, image->height);
			for (int j = 0; j < image->height; j++)
				for (int i = 0; i < image->width; i++)
					bitmap->SetPixel(i, j, Color::FromArgb(image->image[j * image->width + i]));
			pbResult->Image = bitmap;
			FreeLibrary(hGetProcIDDLL);
		}
	};
}