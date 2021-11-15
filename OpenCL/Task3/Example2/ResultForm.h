#include <CL/cl.h>
#include <fstream>
#include <string.h>
#pragma once

namespace Example2 {

	using namespace System;
	using namespace System::ComponentModel;
	using namespace System::Collections;
	using namespace System::Windows::Forms;
	using namespace System::Data;
	using namespace System::Drawing;

	public ref class ResultForm : public System::Windows::Forms::Form
	{
	public:
		ResultForm(Bitmap^ image, Bitmap^ redImage,
			Bitmap^ greenImage, Bitmap^ blueImage)
		{
			InitializeComponent();
			SetImages(image, redImage, greenImage, blueImage);
		}

	protected:
		~ResultForm()
		{
			if (components)
			{
				delete components;
			}
		}
	private: System::Windows::Forms::TableLayoutPanel^ tlpPictures;
	private: System::Windows::Forms::PictureBox^ pbSource;
	private: System::Windows::Forms::PictureBox^ pbRed;
	private: System::Windows::Forms::PictureBox^ pbGreen;
	private: System::Windows::Forms::PictureBox^ pbBlue;
	protected:

	private:
		System::ComponentModel::Container ^components;

#pragma region Windows Form Designer generated code
		void InitializeComponent(void)
		{
			this->tlpPictures = (gcnew System::Windows::Forms::TableLayoutPanel());
			this->pbSource = (gcnew System::Windows::Forms::PictureBox());
			this->pbRed = (gcnew System::Windows::Forms::PictureBox());
			this->pbGreen = (gcnew System::Windows::Forms::PictureBox());
			this->pbBlue = (gcnew System::Windows::Forms::PictureBox());
			this->tlpPictures->SuspendLayout();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->pbSource))->BeginInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->pbRed))->BeginInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->pbGreen))->BeginInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->pbBlue))->BeginInit();
			this->SuspendLayout();
			// 
			// tlpPictures
			// 
			this->tlpPictures->ColumnCount = 2;
			this->tlpPictures->ColumnStyles->Add((gcnew System::Windows::Forms::ColumnStyle(System::Windows::Forms::SizeType::Percent, 50)));
			this->tlpPictures->ColumnStyles->Add((gcnew System::Windows::Forms::ColumnStyle(System::Windows::Forms::SizeType::Percent, 50)));
			this->tlpPictures->Controls->Add(this->pbSource, 0, 0);
			this->tlpPictures->Controls->Add(this->pbRed, 1, 0);
			this->tlpPictures->Controls->Add(this->pbGreen, 0, 1);
			this->tlpPictures->Controls->Add(this->pbBlue, 1, 1);
			this->tlpPictures->Dock = System::Windows::Forms::DockStyle::Fill;
			this->tlpPictures->Location = System::Drawing::Point(0, 0);
			this->tlpPictures->Name = L"tlpPictures";
			this->tlpPictures->RowCount = 2;
			this->tlpPictures->RowStyles->Add((gcnew System::Windows::Forms::RowStyle(System::Windows::Forms::SizeType::Percent, 50)));
			this->tlpPictures->RowStyles->Add((gcnew System::Windows::Forms::RowStyle(System::Windows::Forms::SizeType::Percent, 50)));
			this->tlpPictures->Size = System::Drawing::Size(384, 361);
			this->tlpPictures->TabIndex = 0;
			// 
			// pbSource
			// 
			this->pbSource->Dock = System::Windows::Forms::DockStyle::Fill;
			this->pbSource->Location = System::Drawing::Point(3, 3);
			this->pbSource->Name = L"pbSource";
			this->pbSource->Size = System::Drawing::Size(186, 174);
			this->pbSource->SizeMode = System::Windows::Forms::PictureBoxSizeMode::Zoom;
			this->pbSource->TabIndex = 0;
			this->pbSource->TabStop = false;
			// 
			// pbRed
			// 
			this->pbRed->Dock = System::Windows::Forms::DockStyle::Fill;
			this->pbRed->Location = System::Drawing::Point(195, 3);
			this->pbRed->Name = L"pbRed";
			this->pbRed->Size = System::Drawing::Size(186, 174);
			this->pbRed->SizeMode = System::Windows::Forms::PictureBoxSizeMode::Zoom;
			this->pbRed->TabIndex = 1;
			this->pbRed->TabStop = false;
			// 
			// pbGreen
			// 
			this->pbGreen->Dock = System::Windows::Forms::DockStyle::Fill;
			this->pbGreen->Location = System::Drawing::Point(3, 183);
			this->pbGreen->Name = L"pbGreen";
			this->pbGreen->Size = System::Drawing::Size(186, 175);
			this->pbGreen->SizeMode = System::Windows::Forms::PictureBoxSizeMode::Zoom;
			this->pbGreen->TabIndex = 2;
			this->pbGreen->TabStop = false;
			// 
			// pbBlue
			// 
			this->pbBlue->Dock = System::Windows::Forms::DockStyle::Fill;
			this->pbBlue->Location = System::Drawing::Point(195, 183);
			this->pbBlue->Name = L"pbBlue";
			this->pbBlue->Size = System::Drawing::Size(186, 175);
			this->pbBlue->SizeMode = System::Windows::Forms::PictureBoxSizeMode::Zoom;
			this->pbBlue->TabIndex = 3;
			this->pbBlue->TabStop = false;
			// 
			// ResultForm
			// 
			this->AutoScaleDimensions = System::Drawing::SizeF(6, 13);
			this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
			this->ClientSize = System::Drawing::Size(384, 361);
			this->Controls->Add(this->tlpPictures);
			this->MaximizeBox = false;
			this->MaximumSize = System::Drawing::Size(400, 400);
			this->MinimizeBox = false;
			this->MinimumSize = System::Drawing::Size(400, 400);
			this->Name = L"ResultForm";
			this->StartPosition = System::Windows::Forms::FormStartPosition::CenterScreen;
			this->Text = L"RGB Filter";
			this->tlpPictures->ResumeLayout(false);
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->pbSource))->EndInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->pbRed))->EndInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->pbGreen))->EndInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->pbBlue))->EndInit();
			this->ResumeLayout(false);

		}

	private:
		void SetImages(Bitmap^ image, Bitmap^ redImage,
			Bitmap^ greenImage, Bitmap^ blueImage)
		{
			pbSource->Image = image;
			pbRed->Image = redImage;
			pbGreen->Image = greenImage;
			pbBlue->Image = blueImage;
		}

#pragma endregion
	};
}
