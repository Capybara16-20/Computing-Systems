#pragma once
#include <vector>

namespace KMeans {

	using namespace System;
	using namespace System::ComponentModel;
	using namespace System::Collections;
	using namespace System::Windows::Forms;
	using namespace System::Data;
	using namespace System::Drawing;

	public struct img
	{
		int width;
		int height;
		unsigned* image;
	};

	public struct point
	{
		int x;
		int y;

		point()
		{
			x = y = 0;
		}

		point(int _x, int _y) : x(_x), y(_y) {}

		point operator+(point& other)
		{
			return point(x + other.x, y + other.y);
		}

		point operator-(point& other)
		{
			return point(x - other.x, y - other.y);
		}

		point operator*(double value)
		{
			return point(x * value, y * value);
		}

		point operator/(double value)
		{
			return point(x / value, y / value);
		}

		bool operator== (const point& p)
		{
			if (&p == this) 
				return true;

			return (this->x == p.x) && (this->y == p.y);
		}
	};

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
		~MainForm()
		{
			if (components)
			{
				delete components;
			}
		}
	private: System::Windows::Forms::MenuStrip^ menuStrip;
	private: System::Windows::Forms::ToolStripMenuItem^ menuFile;
	private: System::Windows::Forms::ToolStripMenuItem^ menuOpen;
	private: System::Windows::Forms::ToolStripMenuItem^ menuCalculate;
	private: System::Windows::Forms::ToolStripSeparator^ toolStripSeparator1;
	private: System::Windows::Forms::ToolStripMenuItem^ menuExit;
	private: System::Windows::Forms::TableLayoutPanel^ tlpMain;
	private: System::Windows::Forms::Panel^ pPicture;
	private: System::Windows::Forms::PictureBox^ pbPicture;
	private: System::Windows::Forms::Panel^ pData;
	private: System::Windows::Forms::Label^ lClustersCount;
	private: System::Windows::Forms::TableLayoutPanel^ tlpData;
	private: System::Windows::Forms::Panel^ pResults;
	private: System::Windows::Forms::Panel^ pInput;
	private: System::Windows::Forms::OpenFileDialog^ ofdPicture;
	private: System::Windows::Forms::SaveFileDialog^ sfdPicture;
	private: System::Windows::Forms::Label^ lbResults;
	private: System::Windows::Forms::TextBox^ tbResult;
	private: System::Windows::Forms::NumericUpDown^ nudClustersCount;
	private: System::Windows::Forms::Panel^ pMethod;
	private: System::Windows::Forms::RadioButton^ rbCUDA;
	private: System::Windows::Forms::RadioButton^ rbOpenMP;
	private: System::Windows::Forms::RadioButton^ rbLinear;
	private:System::ComponentModel::Container ^components;

#pragma region Windows Form Designer generated code
		void InitializeComponent(void)
		{
			this->menuStrip = (gcnew System::Windows::Forms::MenuStrip());
			this->menuFile = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->menuOpen = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->menuCalculate = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->toolStripSeparator1 = (gcnew System::Windows::Forms::ToolStripSeparator());
			this->menuExit = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->tlpMain = (gcnew System::Windows::Forms::TableLayoutPanel());
			this->pPicture = (gcnew System::Windows::Forms::Panel());
			this->pbPicture = (gcnew System::Windows::Forms::PictureBox());
			this->pData = (gcnew System::Windows::Forms::Panel());
			this->tlpData = (gcnew System::Windows::Forms::TableLayoutPanel());
			this->pResults = (gcnew System::Windows::Forms::Panel());
			this->tbResult = (gcnew System::Windows::Forms::TextBox());
			this->lbResults = (gcnew System::Windows::Forms::Label());
			this->pInput = (gcnew System::Windows::Forms::Panel());
			this->pMethod = (gcnew System::Windows::Forms::Panel());
			this->rbCUDA = (gcnew System::Windows::Forms::RadioButton());
			this->rbOpenMP = (gcnew System::Windows::Forms::RadioButton());
			this->rbLinear = (gcnew System::Windows::Forms::RadioButton());
			this->nudClustersCount = (gcnew System::Windows::Forms::NumericUpDown());
			this->lClustersCount = (gcnew System::Windows::Forms::Label());
			this->ofdPicture = (gcnew System::Windows::Forms::OpenFileDialog());
			this->sfdPicture = (gcnew System::Windows::Forms::SaveFileDialog());
			this->menuStrip->SuspendLayout();
			this->tlpMain->SuspendLayout();
			this->pPicture->SuspendLayout();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->pbPicture))->BeginInit();
			this->pData->SuspendLayout();
			this->tlpData->SuspendLayout();
			this->pResults->SuspendLayout();
			this->pInput->SuspendLayout();
			this->pMethod->SuspendLayout();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->nudClustersCount))->BeginInit();
			this->SuspendLayout();
			// 
			// menuStrip
			// 
			this->menuStrip->Items->AddRange(gcnew cli::array< System::Windows::Forms::ToolStripItem^  >(1) { this->menuFile });
			this->menuStrip->Location = System::Drawing::Point(0, 0);
			this->menuStrip->Name = L"menuStrip";
			this->menuStrip->Size = System::Drawing::Size(709, 24);
			this->menuStrip->TabIndex = 0;
			this->menuStrip->Text = L"menuStrip1";
			// 
			// menuFile
			// 
			this->menuFile->DropDownItems->AddRange(gcnew cli::array< System::Windows::Forms::ToolStripItem^  >(4) {
				this->menuOpen, this->menuCalculate,
					this->toolStripSeparator1, this->menuExit
			});
			this->menuFile->Name = L"menuFile";
			this->menuFile->Size = System::Drawing::Size(37, 20);
			this->menuFile->Text = L"File";
			// 
			// menuOpen
			// 
			this->menuOpen->Name = L"menuOpen";
			this->menuOpen->ShortcutKeys = static_cast<System::Windows::Forms::Keys>((System::Windows::Forms::Keys::Control | System::Windows::Forms::Keys::O));
			this->menuOpen->Size = System::Drawing::Size(163, 22);
			this->menuOpen->Text = L"Open";
			this->menuOpen->Click += gcnew System::EventHandler(this, &MainForm::menuOpen_Click);
			// 
			// menuCalculate
			// 
			this->menuCalculate->Enabled = false;
			this->menuCalculate->Name = L"menuCalculate";
			this->menuCalculate->ShortcutKeys = static_cast<System::Windows::Forms::Keys>((System::Windows::Forms::Keys::Control | System::Windows::Forms::Keys::E));
			this->menuCalculate->Size = System::Drawing::Size(163, 22);
			this->menuCalculate->Text = L"Calculate";
			this->menuCalculate->Click += gcnew System::EventHandler(this, &MainForm::menuCalculate_Click);
			// 
			// toolStripSeparator1
			// 
			this->toolStripSeparator1->Name = L"toolStripSeparator1";
			this->toolStripSeparator1->Size = System::Drawing::Size(160, 6);
			// 
			// menuExit
			// 
			this->menuExit->Name = L"menuExit";
			this->menuExit->ShortcutKeys = static_cast<System::Windows::Forms::Keys>((System::Windows::Forms::Keys::Control | System::Windows::Forms::Keys::W));
			this->menuExit->Size = System::Drawing::Size(163, 22);
			this->menuExit->Text = L"Exit";
			this->menuExit->Click += gcnew System::EventHandler(this, &MainForm::menuExit_Click);
			// 
			// tlpMain
			// 
			this->tlpMain->ColumnCount = 2;
			this->tlpMain->ColumnStyles->Add((gcnew System::Windows::Forms::ColumnStyle(System::Windows::Forms::SizeType::Absolute, 200)));
			this->tlpMain->ColumnStyles->Add((gcnew System::Windows::Forms::ColumnStyle(System::Windows::Forms::SizeType::Percent, 100)));
			this->tlpMain->Controls->Add(this->pPicture, 1, 0);
			this->tlpMain->Controls->Add(this->pData, 0, 0);
			this->tlpMain->Dock = System::Windows::Forms::DockStyle::Fill;
			this->tlpMain->Location = System::Drawing::Point(0, 24);
			this->tlpMain->Name = L"tlpMain";
			this->tlpMain->RowCount = 1;
			this->tlpMain->RowStyles->Add((gcnew System::Windows::Forms::RowStyle(System::Windows::Forms::SizeType::Percent, 100)));
			this->tlpMain->Size = System::Drawing::Size(709, 440);
			this->tlpMain->TabIndex = 1;
			// 
			// pPicture
			// 
			this->pPicture->Controls->Add(this->pbPicture);
			this->pPicture->Dock = System::Windows::Forms::DockStyle::Fill;
			this->pPicture->Location = System::Drawing::Point(203, 3);
			this->pPicture->Name = L"pPicture";
			this->pPicture->Size = System::Drawing::Size(503, 434);
			this->pPicture->TabIndex = 0;
			// 
			// pbPicture
			// 
			this->pbPicture->BorderStyle = System::Windows::Forms::BorderStyle::FixedSingle;
			this->pbPicture->Dock = System::Windows::Forms::DockStyle::Fill;
			this->pbPicture->Location = System::Drawing::Point(0, 0);
			this->pbPicture->Name = L"pbPicture";
			this->pbPicture->Size = System::Drawing::Size(503, 434);
			this->pbPicture->SizeMode = System::Windows::Forms::PictureBoxSizeMode::Zoom;
			this->pbPicture->TabIndex = 0;
			this->pbPicture->TabStop = false;
			// 
			// pData
			// 
			this->pData->Controls->Add(this->tlpData);
			this->pData->Dock = System::Windows::Forms::DockStyle::Fill;
			this->pData->Location = System::Drawing::Point(3, 3);
			this->pData->Name = L"pData";
			this->pData->Size = System::Drawing::Size(194, 434);
			this->pData->TabIndex = 1;
			// 
			// tlpData
			// 
			this->tlpData->ColumnCount = 1;
			this->tlpData->ColumnStyles->Add((gcnew System::Windows::Forms::ColumnStyle(System::Windows::Forms::SizeType::Percent, 100)));
			this->tlpData->Controls->Add(this->pResults, 0, 1);
			this->tlpData->Controls->Add(this->pInput, 0, 0);
			this->tlpData->Dock = System::Windows::Forms::DockStyle::Fill;
			this->tlpData->Location = System::Drawing::Point(0, 0);
			this->tlpData->Name = L"tlpData";
			this->tlpData->RowCount = 2;
			this->tlpData->RowStyles->Add((gcnew System::Windows::Forms::RowStyle(System::Windows::Forms::SizeType::Absolute, 150)));
			this->tlpData->RowStyles->Add((gcnew System::Windows::Forms::RowStyle(System::Windows::Forms::SizeType::Percent, 100)));
			this->tlpData->Size = System::Drawing::Size(194, 434);
			this->tlpData->TabIndex = 2;
			// 
			// pResults
			// 
			this->pResults->BorderStyle = System::Windows::Forms::BorderStyle::FixedSingle;
			this->pResults->Controls->Add(this->tbResult);
			this->pResults->Controls->Add(this->lbResults);
			this->pResults->Dock = System::Windows::Forms::DockStyle::Fill;
			this->pResults->Location = System::Drawing::Point(3, 153);
			this->pResults->Name = L"pResults";
			this->pResults->Size = System::Drawing::Size(188, 278);
			this->pResults->TabIndex = 0;
			// 
			// tbResult
			// 
			this->tbResult->Enabled = false;
			this->tbResult->Location = System::Drawing::Point(14, 32);
			this->tbResult->Multiline = true;
			this->tbResult->Name = L"tbResult";
			this->tbResult->ReadOnly = true;
			this->tbResult->Size = System::Drawing::Size(156, 50);
			this->tbResult->TabIndex = 2;
			// 
			// lbResults
			// 
			this->lbResults->AutoSize = true;
			this->lbResults->Font = (gcnew System::Drawing::Font(L"Cambria", 12, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(204)));
			this->lbResults->Location = System::Drawing::Point(10, 10);
			this->lbResults->Name = L"lbResults";
			this->lbResults->Size = System::Drawing::Size(63, 19);
			this->lbResults->TabIndex = 1;
			this->lbResults->Text = L"Results:";
			// 
			// pInput
			// 
			this->pInput->BorderStyle = System::Windows::Forms::BorderStyle::FixedSingle;
			this->pInput->Controls->Add(this->pMethod);
			this->pInput->Controls->Add(this->nudClustersCount);
			this->pInput->Controls->Add(this->lClustersCount);
			this->pInput->Dock = System::Windows::Forms::DockStyle::Fill;
			this->pInput->Location = System::Drawing::Point(3, 3);
			this->pInput->Name = L"pInput";
			this->pInput->Size = System::Drawing::Size(188, 144);
			this->pInput->TabIndex = 1;
			// 
			// pMethod
			// 
			this->pMethod->Controls->Add(this->rbCUDA);
			this->pMethod->Controls->Add(this->rbOpenMP);
			this->pMethod->Controls->Add(this->rbLinear);
			this->pMethod->Dock = System::Windows::Forms::DockStyle::Bottom;
			this->pMethod->Location = System::Drawing::Point(0, 56);
			this->pMethod->Name = L"pMethod";
			this->pMethod->Size = System::Drawing::Size(186, 86);
			this->pMethod->TabIndex = 3;
			// 
			// rbCUDA
			// 
			this->rbCUDA->AutoSize = true;
			this->rbCUDA->Location = System::Drawing::Point(15, 60);
			this->rbCUDA->Name = L"rbCUDA";
			this->rbCUDA->Size = System::Drawing::Size(55, 17);
			this->rbCUDA->TabIndex = 2;
			this->rbCUDA->Text = L"CUDA";
			this->rbCUDA->UseVisualStyleBackColor = true;
			// 
			// rbOpenMP
			// 
			this->rbOpenMP->AutoSize = true;
			this->rbOpenMP->Location = System::Drawing::Point(15, 35);
			this->rbOpenMP->Name = L"rbOpenMP";
			this->rbOpenMP->Size = System::Drawing::Size(67, 17);
			this->rbOpenMP->TabIndex = 1;
			this->rbOpenMP->Text = L"OpenMP";
			this->rbOpenMP->UseVisualStyleBackColor = true;
			// 
			// rbLinear
			// 
			this->rbLinear->AutoSize = true;
			this->rbLinear->Checked = true;
			this->rbLinear->Location = System::Drawing::Point(15, 10);
			this->rbLinear->Name = L"rbLinear";
			this->rbLinear->Size = System::Drawing::Size(54, 17);
			this->rbLinear->TabIndex = 0;
			this->rbLinear->TabStop = true;
			this->rbLinear->Text = L"Linear";
			this->rbLinear->UseVisualStyleBackColor = true;
			// 
			// nudClustersCount
			// 
			this->nudClustersCount->Location = System::Drawing::Point(124, 10);
			this->nudClustersCount->Maximum = System::Decimal(gcnew cli::array< System::Int32 >(4) { 5000, 0, 0, 0 });
			this->nudClustersCount->Name = L"nudClustersCount";
			this->nudClustersCount->Size = System::Drawing::Size(47, 20);
			this->nudClustersCount->TabIndex = 2;
			this->nudClustersCount->Value = System::Decimal(gcnew cli::array< System::Int32 >(4) { 5, 0, 0, 0 });
			// 
			// lClustersCount
			// 
			this->lClustersCount->AutoSize = true;
			this->lClustersCount->Font = (gcnew System::Drawing::Font(L"Cambria", 12, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(204)));
			this->lClustersCount->Location = System::Drawing::Point(10, 10);
			this->lClustersCount->Name = L"lClustersCount";
			this->lClustersCount->Size = System::Drawing::Size(112, 19);
			this->lClustersCount->TabIndex = 0;
			this->lClustersCount->Text = L"Clusters count:";
			// 
			// ofdPicture
			// 
			this->ofdPicture->FileName = L"openFileDialog1";
			// 
			// MainForm
			// 
			this->AutoScaleDimensions = System::Drawing::SizeF(6, 13);
			this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
			this->ClientSize = System::Drawing::Size(709, 464);
			this->Controls->Add(this->tlpMain);
			this->Controls->Add(this->menuStrip);
			this->MainMenuStrip = this->menuStrip;
			this->Name = L"MainForm";
			this->StartPosition = System::Windows::Forms::FormStartPosition::CenterScreen;
			this->Text = L"K-Means";
			this->menuStrip->ResumeLayout(false);
			this->menuStrip->PerformLayout();
			this->tlpMain->ResumeLayout(false);
			this->pPicture->ResumeLayout(false);
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->pbPicture))->EndInit();
			this->pData->ResumeLayout(false);
			this->tlpData->ResumeLayout(false);
			this->pResults->ResumeLayout(false);
			this->pResults->PerformLayout();
			this->pInput->ResumeLayout(false);
			this->pInput->PerformLayout();
			this->pMethod->ResumeLayout(false);
			this->pMethod->PerformLayout();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->nudClustersCount))->EndInit();
			this->ResumeLayout(false);
			this->PerformLayout();

		}
#pragma endregion
	private:
		struct img* Image = new img();
		void InitializeImage(struct img* image, PictureBox^ pbSource);
		struct img* GetImageCopy(struct img* image);
		void ShowImage(struct img* image, PictureBox^ pictureBox);
		void ShowResult(double time);

		struct point GetRandomPoint(int maxX, int maxY);
		long double GetEuclideanDistance(unsigned color1, unsigned color2);
		struct img* Calculate(struct img* image, int clustersCount);
		struct img* CalculateOpenMP(struct img* image, int clustersCount);
		struct img* CalculateCUDA(struct img* image, int clustersCount);

		System::Void menuCalculate_Click(System::Object^ sender, System::EventArgs^ e);
		System::Void menuOpen_Click(System::Object^ sender, System::EventArgs^ e);
		System::Void menuExit_Click(System::Object^ sender, System::EventArgs^ e);
};
}
