#pragma once
#include<windows.h>

namespace Example2 {

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
	/// Summary for ResultForm
	/// </summary>
	public ref class ResultForm : public System::Windows::Forms::Form
	{
	public:
		ResultForm(struct img* image, struct img* imageRed,
			struct img* imageGreen, struct img* imageBlue)
		{
			InitializeComponent();
			SetImages(image, imageRed, imageGreen, imageBlue);
		}

	protected:
		/// <summary>
		/// Clean up any resources being used.
		/// </summary>
		~ResultForm()
		{
			if (components)
			{
				delete components;
			}
		}
	private: System::Windows::Forms::TableLayoutPanel^ tlpResult;
	private: System::Windows::Forms::Panel^ pRGB;
	private: System::Windows::Forms::Panel^ pRed;
	private: System::Windows::Forms::Panel^ pGreen;
	private: System::Windows::Forms::Panel^ pBlue;
	private: System::Windows::Forms::PictureBox^ pbRGB;
	private: System::Windows::Forms::PictureBox^ pbRed;
	private: System::Windows::Forms::PictureBox^ pbGreen;
	private: System::Windows::Forms::PictureBox^ pbBlue;
	protected:

	private:
		/// <summary>
		/// Required designer variable.
		/// </summary>
		System::ComponentModel::Container ^components;

#pragma region Windows Form Designer generated code
		/// <summary>
		/// Required method for Designer support - do not modify
		/// the contents of this method with the code editor.
		/// </summary>
		void InitializeComponent(void)
		{
			this->tlpResult = (gcnew System::Windows::Forms::TableLayoutPanel());
			this->pRGB = (gcnew System::Windows::Forms::Panel());
			this->pbRGB = (gcnew System::Windows::Forms::PictureBox());
			this->pRed = (gcnew System::Windows::Forms::Panel());
			this->pbRed = (gcnew System::Windows::Forms::PictureBox());
			this->pGreen = (gcnew System::Windows::Forms::Panel());
			this->pbGreen = (gcnew System::Windows::Forms::PictureBox());
			this->pBlue = (gcnew System::Windows::Forms::Panel());
			this->pbBlue = (gcnew System::Windows::Forms::PictureBox());
			this->tlpResult->SuspendLayout();
			this->pRGB->SuspendLayout();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->pbRGB))->BeginInit();
			this->pRed->SuspendLayout();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->pbRed))->BeginInit();
			this->pGreen->SuspendLayout();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->pbGreen))->BeginInit();
			this->pBlue->SuspendLayout();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->pbBlue))->BeginInit();
			this->SuspendLayout();
			// 
			// tlpResult
			// 
			this->tlpResult->ColumnCount = 2;
			this->tlpResult->ColumnStyles->Add((gcnew System::Windows::Forms::ColumnStyle(System::Windows::Forms::SizeType::Percent, 50)));
			this->tlpResult->ColumnStyles->Add((gcnew System::Windows::Forms::ColumnStyle(System::Windows::Forms::SizeType::Percent, 50)));
			this->tlpResult->Controls->Add(this->pRGB, 0, 0);
			this->tlpResult->Controls->Add(this->pRed, 1, 0);
			this->tlpResult->Controls->Add(this->pGreen, 0, 1);
			this->tlpResult->Controls->Add(this->pBlue, 1, 1);
			this->tlpResult->Dock = System::Windows::Forms::DockStyle::Fill;
			this->tlpResult->Location = System::Drawing::Point(0, 0);
			this->tlpResult->Name = L"tlpResult";
			this->tlpResult->RowCount = 2;
			this->tlpResult->RowStyles->Add((gcnew System::Windows::Forms::RowStyle(System::Windows::Forms::SizeType::Percent, 50)));
			this->tlpResult->RowStyles->Add((gcnew System::Windows::Forms::RowStyle(System::Windows::Forms::SizeType::Percent, 50)));
			this->tlpResult->Size = System::Drawing::Size(384, 361);
			this->tlpResult->TabIndex = 0;
			// 
			// pRGB
			// 
			this->pRGB->Controls->Add(this->pbRGB);
			this->pRGB->Dock = System::Windows::Forms::DockStyle::Fill;
			this->pRGB->Location = System::Drawing::Point(3, 3);
			this->pRGB->Name = L"pRGB";
			this->pRGB->Size = System::Drawing::Size(186, 174);
			this->pRGB->TabIndex = 0;
			// 
			// pbRGB
			// 
			this->pbRGB->Dock = System::Windows::Forms::DockStyle::Fill;
			this->pbRGB->Location = System::Drawing::Point(0, 0);
			this->pbRGB->Name = L"pbRGB";
			this->pbRGB->Size = System::Drawing::Size(186, 174);
			this->pbRGB->SizeMode = System::Windows::Forms::PictureBoxSizeMode::Zoom;
			this->pbRGB->TabIndex = 0;
			this->pbRGB->TabStop = false;
			// 
			// pRed
			// 
			this->pRed->Controls->Add(this->pbRed);
			this->pRed->Dock = System::Windows::Forms::DockStyle::Fill;
			this->pRed->Location = System::Drawing::Point(195, 3);
			this->pRed->Name = L"pRed";
			this->pRed->Size = System::Drawing::Size(186, 174);
			this->pRed->TabIndex = 1;
			// 
			// pbRed
			// 
			this->pbRed->Dock = System::Windows::Forms::DockStyle::Fill;
			this->pbRed->Location = System::Drawing::Point(0, 0);
			this->pbRed->Name = L"pbRed";
			this->pbRed->Size = System::Drawing::Size(186, 174);
			this->pbRed->SizeMode = System::Windows::Forms::PictureBoxSizeMode::Zoom;
			this->pbRed->TabIndex = 0;
			this->pbRed->TabStop = false;
			// 
			// pGreen
			// 
			this->pGreen->Controls->Add(this->pbGreen);
			this->pGreen->Dock = System::Windows::Forms::DockStyle::Fill;
			this->pGreen->Location = System::Drawing::Point(3, 183);
			this->pGreen->Name = L"pGreen";
			this->pGreen->Size = System::Drawing::Size(186, 175);
			this->pGreen->TabIndex = 2;
			// 
			// pbGreen
			// 
			this->pbGreen->Dock = System::Windows::Forms::DockStyle::Fill;
			this->pbGreen->Location = System::Drawing::Point(0, 0);
			this->pbGreen->Name = L"pbGreen";
			this->pbGreen->Size = System::Drawing::Size(186, 175);
			this->pbGreen->SizeMode = System::Windows::Forms::PictureBoxSizeMode::Zoom;
			this->pbGreen->TabIndex = 0;
			this->pbGreen->TabStop = false;
			// 
			// pBlue
			// 
			this->pBlue->Controls->Add(this->pbBlue);
			this->pBlue->Dock = System::Windows::Forms::DockStyle::Fill;
			this->pBlue->Location = System::Drawing::Point(195, 183);
			this->pBlue->Name = L"pBlue";
			this->pBlue->Size = System::Drawing::Size(186, 175);
			this->pBlue->TabIndex = 3;
			// 
			// pbBlue
			// 
			this->pbBlue->Dock = System::Windows::Forms::DockStyle::Fill;
			this->pbBlue->Location = System::Drawing::Point(0, 0);
			this->pbBlue->Name = L"pbBlue";
			this->pbBlue->Size = System::Drawing::Size(186, 175);
			this->pbBlue->SizeMode = System::Windows::Forms::PictureBoxSizeMode::Zoom;
			this->pbBlue->TabIndex = 0;
			this->pbBlue->TabStop = false;
			// 
			// ResultForm
			// 
			this->AutoScaleDimensions = System::Drawing::SizeF(6, 13);
			this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
			this->ClientSize = System::Drawing::Size(384, 361);
			this->Controls->Add(this->tlpResult);
			this->FormBorderStyle = System::Windows::Forms::FormBorderStyle::FixedDialog;
			this->MaximizeBox = false;
			this->MaximumSize = System::Drawing::Size(400, 400);
			this->MinimizeBox = false;
			this->MinimumSize = System::Drawing::Size(400, 400);
			this->Name = L"ResultForm";
			this->StartPosition = System::Windows::Forms::FormStartPosition::CenterScreen;
			this->Text = L"RGB Filter";
			this->tlpResult->ResumeLayout(false);
			this->pRGB->ResumeLayout(false);
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->pbRGB))->EndInit();
			this->pRed->ResumeLayout(false);
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->pbRed))->EndInit();
			this->pGreen->ResumeLayout(false);
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->pbGreen))->EndInit();
			this->pBlue->ResumeLayout(false);
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->pbBlue))->EndInit();
			this->ResumeLayout(false);

		}
#pragma endregion

	private:
		void SetImages(struct img* image, struct img* imageRed, 
			struct img* imageGreen, struct img* imageBlue)
		{
			HINSTANCE hGetProcIDDLL = LoadLibrary(L"Example2CUDA.dll");
			if (!hGetProcIDDLL)
				throw gcnew Exception();
			typedef void(__stdcall* function)(void*, void*, void*, void*);
			function calc = (function)GetProcAddress(hGetProcIDDLL, "calculate");
			if (!calc)
				throw gcnew Exception();
			calc((void*)image, imageRed, imageGreen, imageBlue);
			Bitmap^ bitmapRGB = gcnew Bitmap(image->width, image->height);
			Bitmap^ bitmapRed = gcnew Bitmap(image->width, image->height);
			Bitmap^ bitmapGreen = gcnew Bitmap(image->width, image->height);
			Bitmap^ bitmapBlue = gcnew Bitmap(image->width, image->height);
			for (int j = 0; j < image->height; j++)
			{
				for (int i = 0; i < image->width; i++)
				{
					bitmapRGB->SetPixel(i, j, Color::FromArgb(image->image[j * image->width + i]));
					bitmapRed->SetPixel(i, j, Color::FromArgb(imageRed->image[j * imageRed->width + i]));
					bitmapGreen->SetPixel(i, j, Color::FromArgb(imageGreen->image[j * imageGreen->width + i]));
					bitmapBlue->SetPixel(i, j, Color::FromArgb(imageBlue->image[j * imageBlue->width + i]));
				}
			}
			pbRGB->Image = bitmapRGB;
			pbRed->Image = bitmapRed;
			pbGreen->Image = bitmapGreen;
			pbBlue->Image = bitmapBlue;
			FreeLibrary(hGetProcIDDLL);
		}
	};
}
