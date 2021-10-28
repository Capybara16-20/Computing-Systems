#pragma once
#include "ResultForm.h"

namespace Example2 {

	using namespace System;
	using namespace System::ComponentModel;
	using namespace System::Collections;
	using namespace System::Windows::Forms;
	using namespace System::Data;
	using namespace System::Drawing;

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

	private: System::Windows::Forms::ToolStripMenuItem^ fileToolStripMenuItem;
	private: System::Windows::Forms::ToolStripMenuItem^ openToolStripMenuItem;
	private: System::Windows::Forms::ToolStripMenuItem^ calculateToolStripMenuItem;
	private: System::Windows::Forms::ToolStripSeparator^ toolStripSeparator1;
	private: System::Windows::Forms::ToolStripMenuItem^ exitToolStripMenuItem;
	private: System::Windows::Forms::Panel^ pSource;
	private: System::Windows::Forms::PictureBox^ pbSource;
	private: System::Windows::Forms::OpenFileDialog^ ofdSource;

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
			this->calculateToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->toolStripSeparator1 = (gcnew System::Windows::Forms::ToolStripSeparator());
			this->exitToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->pSource = (gcnew System::Windows::Forms::Panel());
			this->pbSource = (gcnew System::Windows::Forms::PictureBox());
			this->ofdSource = (gcnew System::Windows::Forms::OpenFileDialog());
			this->menuStrip->SuspendLayout();
			this->pSource->SuspendLayout();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->pbSource))->BeginInit();
			this->SuspendLayout();
			// 
			// menuStrip
			// 
			this->menuStrip->Items->AddRange(gcnew cli::array< System::Windows::Forms::ToolStripItem^  >(1) { this->fileToolStripMenuItem });
			this->menuStrip->Location = System::Drawing::Point(0, 0);
			this->menuStrip->Name = L"menuStrip";
			this->menuStrip->Size = System::Drawing::Size(684, 24);
			this->menuStrip->TabIndex = 1;
			this->menuStrip->Text = L"menuStrip1";
			// 
			// fileToolStripMenuItem
			// 
			this->fileToolStripMenuItem->DropDownItems->AddRange(gcnew cli::array< System::Windows::Forms::ToolStripItem^  >(4) {
				this->openToolStripMenuItem,
					this->calculateToolStripMenuItem, this->toolStripSeparator1, this->exitToolStripMenuItem
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
			// pSource
			// 
			this->pSource->AutoScroll = true;
			this->pSource->Controls->Add(this->pbSource);
			this->pSource->Dock = System::Windows::Forms::DockStyle::Fill;
			this->pSource->Location = System::Drawing::Point(0, 24);
			this->pSource->Name = L"pSource";
			this->pSource->Size = System::Drawing::Size(684, 437);
			this->pSource->TabIndex = 2;
			// 
			// pbSource
			// 
			this->pbSource->Dock = System::Windows::Forms::DockStyle::Fill;
			this->pbSource->Location = System::Drawing::Point(0, 0);
			this->pbSource->Name = L"pbSource";
			this->pbSource->Size = System::Drawing::Size(684, 437);
			this->pbSource->SizeMode = System::Windows::Forms::PictureBoxSizeMode::Zoom;
			this->pbSource->TabIndex = 0;
			this->pbSource->TabStop = false;
			// 
			// ofdSource
			// 
			this->ofdSource->FileName = L"openFileDialog1";
			// 
			// MainForm
			// 
			this->AutoScaleDimensions = System::Drawing::SizeF(6, 13);
			this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
			this->AutoSize = true;
			this->ClientSize = System::Drawing::Size(684, 461);
			this->Controls->Add(this->pSource);
			this->Controls->Add(this->menuStrip);
			this->MainMenuStrip = this->menuStrip;
			this->MaximizeBox = false;
			this->MinimizeBox = false;
			this->Name = L"MainForm";
			this->StartPosition = System::Windows::Forms::FormStartPosition::CenterScreen;
			this->Text = L"RGB Filter";
			this->menuStrip->ResumeLayout(false);
			this->menuStrip->PerformLayout();
			this->pSource->ResumeLayout(false);
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->pbSource))->EndInit();
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
		}
	private:
		System::Void exitToolStripMenuItem_Click(System::Object^ sender, System::EventArgs^ e)
		{
			this->Close();
		}
	private:
		System::Void calculateToolStripMenuItem_Click(System::Object^ sender, System::EventArgs^ e)
		{
			if (pbSource->Image != nullptr)
			{
				struct img* image = new img();
				struct img* redImage = new img();
				struct img* greenImage = new img();
				struct img* blueImage = new img();
				InitializeImage(image, pbSource);
				InitializeImage(redImage, pbSource);
				InitializeImage(greenImage, pbSource);
				InitializeImage(blueImage, pbSource);

				ResultForm^ form = gcnew ResultForm(image, redImage, greenImage, blueImage);
				form->ShowDialog();
			}
		}

	private:
		void InitializeImage(struct img* image, PictureBox^ pbSource)
		{
			image->width = pbSource->Image->Width;
			image->height = pbSource->Image->Height;
			image->image = new unsigned[image->width * image->height];
			Bitmap^ bitmap = gcnew Bitmap(pbSource->Image);
			for (int j = 0; j < image->height; j++)
				for (int i = 0; i < image->width; i++)
					image->image[j * image->width + i] = bitmap->GetPixel(i, j).ToArgb();
		}
	};
}
