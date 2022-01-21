#pragma once

namespace DeformablePolyhedronMethod {

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

		point(int _x, int _y) : x(_x), y(_y){}

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
	};

	public ref class MainForm : public System::Windows::Forms::Form
	{
	public:
		MainForm(void)
		{
			InitializeComponent();
			ChangeTheme(lightThemeColor);
		}

	protected:
		~MainForm()
		{
			if (components)
			{
				delete components;
			}
		}

	private: System::Drawing::Color darkThemeColor = System::Drawing::Color::DarkGray;
	private: System::Drawing::Color lightThemeColor = System::Drawing::Color::LightGray;
	private: System::Drawing::Color blueThemeColor = System::Drawing::Color::SteelBlue;
	private: System::Drawing::Color coralThemeColor = System::Drawing::Color::Coral;
	private: System::Drawing::Color greenThemeColor = System::Drawing::Color::SeaGreen;
	private: System::Windows::Forms::MenuStrip^ menu;
	private: System::Windows::Forms::ToolStripMenuItem^ fileToolStripMenuItem;
	private: System::Windows::Forms::ToolStripMenuItem^ menuOpenMain;
	private: System::Windows::Forms::ToolStripMenuItem^ menuOpenSub;
	private: System::Windows::Forms::ToolStripMenuItem^ menuCalculate;
	private: System::Windows::Forms::ToolStripSeparator^ toolStripSeparator;
	private: System::Windows::Forms::ToolStripMenuItem^ menuExit;
	private: System::Windows::Forms::ToolStripMenuItem^ viewToolStripMenuItem;
	private: System::Windows::Forms::ToolStripMenuItem^ themesToolStripMenuItem;
	private: System::Windows::Forms::ToolStripMenuItem^ menuLightTheme;
	private: System::Windows::Forms::ToolStripMenuItem^ menuDarkTheme;
	private: System::Windows::Forms::ToolStripMenuItem^ menuBlueTheme;
	private: System::Windows::Forms::TableLayoutPanel^ tlpImages;
	private: System::Windows::Forms::TableLayoutPanel^ tableLayoutPanel1;
	private: System::Windows::Forms::Panel^ pSubImage;
	private: System::Windows::Forms::Panel^ pOutput;
	private: System::Windows::Forms::Label^ lResult;
	private: System::Windows::Forms::TextBox^ tbResult;
	private: System::Windows::Forms::Panel^ pMainImage;
	private: System::Windows::Forms::ToolStripMenuItem^ menuLCoralTheme;
	private: System::Windows::Forms::ToolStripMenuItem^ menuGreenTheme;
	private: System::Windows::Forms::OpenFileDialog^ ofdImage;
	private: System::Windows::Forms::PictureBox^ pbSub;
	private: System::Windows::Forms::PictureBox^ pbMain;
	private: System::Windows::Forms::Panel^ pInput;
	private: System::Windows::Forms::Label^ lStretch;
	private: System::Windows::Forms::Label^ lCompression;
	private: System::Windows::Forms::TextBox^ tbStretch;
	private: System::Windows::Forms::TextBox^ tbCompression;
	private: System::Windows::Forms::TextBox^ tbReflection;
	private: System::Windows::Forms::Label^ lReflection;
	private: System::Windows::Forms::TextBox^ tbAccuracy;
	private: System::Windows::Forms::Label^ lAccuracy;
	private: System::Windows::Forms::Panel^ pMethod;
	private: System::Windows::Forms::RadioButton^ rbSimple;
	private: System::Windows::Forms::RadioButton^ rbOpenMP;
	private: System::Windows::Forms::RadioButton^ rbCuda;
	private: System::Windows::Forms::TextBox^ tbIterations;
	private: System::Windows::Forms::Label^ lbIterations;
	private: System::ComponentModel::Container^ components;

#pragma region Windows Form Designer generated code
		void InitializeComponent(void)
		{
			System::ComponentModel::ComponentResourceManager^ resources = (gcnew System::ComponentModel::ComponentResourceManager(MainForm::typeid));
			this->menu = (gcnew System::Windows::Forms::MenuStrip());
			this->fileToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->menuOpenMain = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->menuOpenSub = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->menuCalculate = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->toolStripSeparator = (gcnew System::Windows::Forms::ToolStripSeparator());
			this->menuExit = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->viewToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->themesToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->menuLightTheme = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->menuDarkTheme = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->menuBlueTheme = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->menuLCoralTheme = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->menuGreenTheme = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->tlpImages = (gcnew System::Windows::Forms::TableLayoutPanel());
			this->tableLayoutPanel1 = (gcnew System::Windows::Forms::TableLayoutPanel());
			this->pSubImage = (gcnew System::Windows::Forms::Panel());
			this->pbSub = (gcnew System::Windows::Forms::PictureBox());
			this->pOutput = (gcnew System::Windows::Forms::Panel());
			this->tbResult = (gcnew System::Windows::Forms::TextBox());
			this->lResult = (gcnew System::Windows::Forms::Label());
			this->pInput = (gcnew System::Windows::Forms::Panel());
			this->pMethod = (gcnew System::Windows::Forms::Panel());
			this->rbSimple = (gcnew System::Windows::Forms::RadioButton());
			this->rbOpenMP = (gcnew System::Windows::Forms::RadioButton());
			this->rbCuda = (gcnew System::Windows::Forms::RadioButton());
			this->lAccuracy = (gcnew System::Windows::Forms::Label());
			this->tbAccuracy = (gcnew System::Windows::Forms::TextBox());
			this->lStretch = (gcnew System::Windows::Forms::Label());
			this->lCompression = (gcnew System::Windows::Forms::Label());
			this->tbStretch = (gcnew System::Windows::Forms::TextBox());
			this->tbCompression = (gcnew System::Windows::Forms::TextBox());
			this->tbReflection = (gcnew System::Windows::Forms::TextBox());
			this->lReflection = (gcnew System::Windows::Forms::Label());
			this->pMainImage = (gcnew System::Windows::Forms::Panel());
			this->pbMain = (gcnew System::Windows::Forms::PictureBox());
			this->ofdImage = (gcnew System::Windows::Forms::OpenFileDialog());
			this->lbIterations = (gcnew System::Windows::Forms::Label());
			this->tbIterations = (gcnew System::Windows::Forms::TextBox());
			this->menu->SuspendLayout();
			this->tlpImages->SuspendLayout();
			this->tableLayoutPanel1->SuspendLayout();
			this->pSubImage->SuspendLayout();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->pbSub))->BeginInit();
			this->pOutput->SuspendLayout();
			this->pInput->SuspendLayout();
			this->pMethod->SuspendLayout();
			this->pMainImage->SuspendLayout();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->pbMain))->BeginInit();
			this->SuspendLayout();
			// 
			// menu
			// 
			this->menu->Items->AddRange(gcnew cli::array< System::Windows::Forms::ToolStripItem^  >(2) {
				this->fileToolStripMenuItem,
					this->viewToolStripMenuItem
			});
			this->menu->Location = System::Drawing::Point(0, 0);
			this->menu->Name = L"menu";
			this->menu->Size = System::Drawing::Size(643, 24);
			this->menu->TabIndex = 0;
			this->menu->Text = L"menuStrip1";
			// 
			// fileToolStripMenuItem
			// 
			this->fileToolStripMenuItem->DropDownItems->AddRange(gcnew cli::array< System::Windows::Forms::ToolStripItem^  >(5) {
				this->menuOpenMain,
					this->menuOpenSub, this->menuCalculate, this->toolStripSeparator, this->menuExit
			});
			this->fileToolStripMenuItem->Name = L"fileToolStripMenuItem";
			this->fileToolStripMenuItem->Size = System::Drawing::Size(37, 20);
			this->fileToolStripMenuItem->Text = L"File";
			// 
			// menuOpenMain
			// 
			this->menuOpenMain->Name = L"menuOpenMain";
			this->menuOpenMain->ShortcutKeys = static_cast<System::Windows::Forms::Keys>((System::Windows::Forms::Keys::Control | System::Windows::Forms::Keys::O));
			this->menuOpenMain->Size = System::Drawing::Size(236, 22);
			this->menuOpenMain->Text = L"Open main image";
			this->menuOpenMain->Click += gcnew System::EventHandler(this, &MainForm::menuOpenMain_Click);
			// 
			// menuOpenSub
			// 
			this->menuOpenSub->Name = L"menuOpenSub";
			this->menuOpenSub->ShortcutKeys = static_cast<System::Windows::Forms::Keys>(((System::Windows::Forms::Keys::Control | System::Windows::Forms::Keys::Shift)
				| System::Windows::Forms::Keys::O));
			this->menuOpenSub->Size = System::Drawing::Size(236, 22);
			this->menuOpenSub->Text = L"Open sub image";
			this->menuOpenSub->Click += gcnew System::EventHandler(this, &MainForm::menuOpenSub_Click);
			// 
			// menuCalculate
			// 
			this->menuCalculate->Enabled = false;
			this->menuCalculate->Name = L"menuCalculate";
			this->menuCalculate->ShortcutKeys = static_cast<System::Windows::Forms::Keys>((System::Windows::Forms::Keys::Control | System::Windows::Forms::Keys::E));
			this->menuCalculate->Size = System::Drawing::Size(236, 22);
			this->menuCalculate->Text = L"Calculate";
			this->menuCalculate->Click += gcnew System::EventHandler(this, &MainForm::menuCalculate_Click);
			// 
			// toolStripSeparator
			// 
			this->toolStripSeparator->Name = L"toolStripSeparator";
			this->toolStripSeparator->Size = System::Drawing::Size(233, 6);
			// 
			// menuExit
			// 
			this->menuExit->Name = L"menuExit";
			this->menuExit->ShortcutKeys = static_cast<System::Windows::Forms::Keys>((System::Windows::Forms::Keys::Control | System::Windows::Forms::Keys::W));
			this->menuExit->Size = System::Drawing::Size(236, 22);
			this->menuExit->Text = L"Exit";
			this->menuExit->Click += gcnew System::EventHandler(this, &MainForm::menuExit_Click);
			// 
			// viewToolStripMenuItem
			// 
			this->viewToolStripMenuItem->DropDownItems->AddRange(gcnew cli::array< System::Windows::Forms::ToolStripItem^  >(1) { this->themesToolStripMenuItem });
			this->viewToolStripMenuItem->Name = L"viewToolStripMenuItem";
			this->viewToolStripMenuItem->Size = System::Drawing::Size(44, 20);
			this->viewToolStripMenuItem->Text = L"View";
			// 
			// themesToolStripMenuItem
			// 
			this->themesToolStripMenuItem->DropDownItems->AddRange(gcnew cli::array< System::Windows::Forms::ToolStripItem^  >(5) {
				this->menuLightTheme,
					this->menuDarkTheme, this->menuBlueTheme, this->menuLCoralTheme, this->menuGreenTheme
			});
			this->themesToolStripMenuItem->Name = L"themesToolStripMenuItem";
			this->themesToolStripMenuItem->Size = System::Drawing::Size(115, 22);
			this->themesToolStripMenuItem->Text = L"Themes";
			// 
			// menuLightTheme
			// 
			this->menuLightTheme->BackColor = System::Drawing::SystemColors::Control;
			this->menuLightTheme->Image = (cli::safe_cast<System::Drawing::Image^>(resources->GetObject(L"menuLightTheme.Image")));
			this->menuLightTheme->Name = L"menuLightTheme";
			this->menuLightTheme->Size = System::Drawing::Size(105, 22);
			this->menuLightTheme->Text = L"Light";
			this->menuLightTheme->Click += gcnew System::EventHandler(this, &MainForm::menuLightTheme_Click);
			// 
			// menuDarkTheme
			// 
			this->menuDarkTheme->BackColor = System::Drawing::SystemColors::Control;
			this->menuDarkTheme->Image = (cli::safe_cast<System::Drawing::Image^>(resources->GetObject(L"menuDarkTheme.Image")));
			this->menuDarkTheme->Name = L"menuDarkTheme";
			this->menuDarkTheme->Size = System::Drawing::Size(105, 22);
			this->menuDarkTheme->Text = L"Dark";
			this->menuDarkTheme->Click += gcnew System::EventHandler(this, &MainForm::menuDarkTheme_Click);
			// 
			// menuBlueTheme
			// 
			this->menuBlueTheme->BackColor = System::Drawing::SystemColors::Control;
			this->menuBlueTheme->Image = (cli::safe_cast<System::Drawing::Image^>(resources->GetObject(L"menuBlueTheme.Image")));
			this->menuBlueTheme->Name = L"menuBlueTheme";
			this->menuBlueTheme->Size = System::Drawing::Size(105, 22);
			this->menuBlueTheme->Text = L"Blue";
			this->menuBlueTheme->Click += gcnew System::EventHandler(this, &MainForm::menuBlueTheme_Click);
			// 
			// menuLCoralTheme
			// 
			this->menuLCoralTheme->BackColor = System::Drawing::SystemColors::Control;
			this->menuLCoralTheme->Image = (cli::safe_cast<System::Drawing::Image^>(resources->GetObject(L"menuLCoralTheme.Image")));
			this->menuLCoralTheme->Name = L"menuLCoralTheme";
			this->menuLCoralTheme->Size = System::Drawing::Size(105, 22);
			this->menuLCoralTheme->Text = L"Coral";
			this->menuLCoralTheme->Click += gcnew System::EventHandler(this, &MainForm::menuLCoralTheme_Click);
			// 
			// menuGreenTheme
			// 
			this->menuGreenTheme->BackColor = System::Drawing::SystemColors::Control;
			this->menuGreenTheme->Image = (cli::safe_cast<System::Drawing::Image^>(resources->GetObject(L"menuGreenTheme.Image")));
			this->menuGreenTheme->Name = L"menuGreenTheme";
			this->menuGreenTheme->Size = System::Drawing::Size(105, 22);
			this->menuGreenTheme->Text = L"Green";
			this->menuGreenTheme->Click += gcnew System::EventHandler(this, &MainForm::menuGreenTheme_Click);
			// 
			// tlpImages
			// 
			this->tlpImages->ColumnCount = 2;
			this->tlpImages->ColumnStyles->Add((gcnew System::Windows::Forms::ColumnStyle(System::Windows::Forms::SizeType::Absolute, 206)));
			this->tlpImages->ColumnStyles->Add((gcnew System::Windows::Forms::ColumnStyle(System::Windows::Forms::SizeType::Percent, 100)));
			this->tlpImages->Controls->Add(this->tableLayoutPanel1, 0, 0);
			this->tlpImages->Controls->Add(this->pMainImage, 1, 0);
			this->tlpImages->Dock = System::Windows::Forms::DockStyle::Fill;
			this->tlpImages->Location = System::Drawing::Point(0, 24);
			this->tlpImages->Name = L"tlpImages";
			this->tlpImages->RowCount = 1;
			this->tlpImages->RowStyles->Add((gcnew System::Windows::Forms::RowStyle(System::Windows::Forms::SizeType::Percent, 100)));
			this->tlpImages->Size = System::Drawing::Size(643, 437);
			this->tlpImages->TabIndex = 1;
			// 
			// tableLayoutPanel1
			// 
			this->tableLayoutPanel1->ColumnCount = 1;
			this->tableLayoutPanel1->ColumnStyles->Add((gcnew System::Windows::Forms::ColumnStyle(System::Windows::Forms::SizeType::Percent,
				100)));
			this->tableLayoutPanel1->Controls->Add(this->pSubImage, 0, 1);
			this->tableLayoutPanel1->Controls->Add(this->pOutput, 0, 2);
			this->tableLayoutPanel1->Controls->Add(this->pInput, 0, 0);
			this->tableLayoutPanel1->Dock = System::Windows::Forms::DockStyle::Fill;
			this->tableLayoutPanel1->Location = System::Drawing::Point(3, 3);
			this->tableLayoutPanel1->Name = L"tableLayoutPanel1";
			this->tableLayoutPanel1->RowCount = 3;
			this->tableLayoutPanel1->RowStyles->Add((gcnew System::Windows::Forms::RowStyle(System::Windows::Forms::SizeType::Percent, 68)));
			this->tableLayoutPanel1->RowStyles->Add((gcnew System::Windows::Forms::RowStyle(System::Windows::Forms::SizeType::Absolute, 200)));
			this->tableLayoutPanel1->RowStyles->Add((gcnew System::Windows::Forms::RowStyle(System::Windows::Forms::SizeType::Percent, 32)));
			this->tableLayoutPanel1->Size = System::Drawing::Size(200, 431);
			this->tableLayoutPanel1->TabIndex = 2;
			// 
			// pSubImage
			// 
			this->pSubImage->BorderStyle = System::Windows::Forms::BorderStyle::FixedSingle;
			this->pSubImage->Controls->Add(this->pbSub);
			this->pSubImage->Dock = System::Windows::Forms::DockStyle::Fill;
			this->pSubImage->Location = System::Drawing::Point(3, 160);
			this->pSubImage->Name = L"pSubImage";
			this->pSubImage->Size = System::Drawing::Size(194, 194);
			this->pSubImage->TabIndex = 0;
			// 
			// pbSub
			// 
			this->pbSub->Dock = System::Windows::Forms::DockStyle::Fill;
			this->pbSub->Location = System::Drawing::Point(0, 0);
			this->pbSub->Name = L"pbSub";
			this->pbSub->Size = System::Drawing::Size(192, 192);
			this->pbSub->SizeMode = System::Windows::Forms::PictureBoxSizeMode::Zoom;
			this->pbSub->TabIndex = 0;
			this->pbSub->TabStop = false;
			// 
			// pOutput
			// 
			this->pOutput->BorderStyle = System::Windows::Forms::BorderStyle::FixedSingle;
			this->pOutput->Controls->Add(this->tbResult);
			this->pOutput->Controls->Add(this->lResult);
			this->pOutput->Dock = System::Windows::Forms::DockStyle::Fill;
			this->pOutput->Location = System::Drawing::Point(3, 360);
			this->pOutput->Name = L"pOutput";
			this->pOutput->Size = System::Drawing::Size(194, 68);
			this->pOutput->TabIndex = 1;
			// 
			// tbResult
			// 
			this->tbResult->BorderStyle = System::Windows::Forms::BorderStyle::FixedSingle;
			this->tbResult->Enabled = false;
			this->tbResult->Font = (gcnew System::Drawing::Font(L"Calibri", 9, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(204)));
			this->tbResult->Location = System::Drawing::Point(9, 27);
			this->tbResult->Multiline = true;
			this->tbResult->Name = L"tbResult";
			this->tbResult->Size = System::Drawing::Size(174, 37);
			this->tbResult->TabIndex = 1;
			// 
			// lResult
			// 
			this->lResult->AutoSize = true;
			this->lResult->Font = (gcnew System::Drawing::Font(L"Calibri", 12, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(0)));
			this->lResult->Location = System::Drawing::Point(5, 5);
			this->lResult->Name = L"lResult";
			this->lResult->Size = System::Drawing::Size(120, 19);
			this->lResult->TabIndex = 0;
			this->lResult->Text = L"Alignment result:";
			// 
			// pInput
			// 
			this->pInput->BorderStyle = System::Windows::Forms::BorderStyle::FixedSingle;
			this->pInput->Controls->Add(this->tbIterations);
			this->pInput->Controls->Add(this->lbIterations);
			this->pInput->Controls->Add(this->pMethod);
			this->pInput->Controls->Add(this->lAccuracy);
			this->pInput->Controls->Add(this->tbAccuracy);
			this->pInput->Controls->Add(this->lStretch);
			this->pInput->Controls->Add(this->lCompression);
			this->pInput->Controls->Add(this->tbStretch);
			this->pInput->Controls->Add(this->tbCompression);
			this->pInput->Controls->Add(this->tbReflection);
			this->pInput->Controls->Add(this->lReflection);
			this->pInput->Dock = System::Windows::Forms::DockStyle::Fill;
			this->pInput->Location = System::Drawing::Point(3, 3);
			this->pInput->Name = L"pInput";
			this->pInput->Size = System::Drawing::Size(194, 151);
			this->pInput->TabIndex = 2;
			// 
			// pMethod
			// 
			this->pMethod->Controls->Add(this->rbSimple);
			this->pMethod->Controls->Add(this->rbOpenMP);
			this->pMethod->Controls->Add(this->rbCuda);
			this->pMethod->Location = System::Drawing::Point(3, 102);
			this->pMethod->Name = L"pMethod";
			this->pMethod->Size = System::Drawing::Size(185, 23);
			this->pMethod->TabIndex = 8;
			// 
			// rbSimple
			// 
			this->rbSimple->AutoSize = true;
			this->rbSimple->Location = System::Drawing::Point(128, 5);
			this->rbSimple->Name = L"rbSimple";
			this->rbSimple->Size = System::Drawing::Size(57, 17);
			this->rbSimple->TabIndex = 2;
			this->rbSimple->Text = L"Simple";
			this->rbSimple->UseVisualStyleBackColor = true;
			// 
			// rbOpenMP
			// 
			this->rbOpenMP->AutoSize = true;
			this->rbOpenMP->Location = System::Drawing::Point(60, 5);
			this->rbOpenMP->Name = L"rbOpenMP";
			this->rbOpenMP->Size = System::Drawing::Size(65, 17);
			this->rbOpenMP->TabIndex = 1;
			this->rbOpenMP->Text = L"OpenMP";
			this->rbOpenMP->UseVisualStyleBackColor = true;
			// 
			// rbCuda
			// 
			this->rbCuda->AutoSize = true;
			this->rbCuda->Checked = true;
			this->rbCuda->Location = System::Drawing::Point(5, 5);
			this->rbCuda->Name = L"rbCuda";
			this->rbCuda->Size = System::Drawing::Size(51, 17);
			this->rbCuda->TabIndex = 0;
			this->rbCuda->TabStop = true;
			this->rbCuda->Text = L"CUDA";
			this->rbCuda->UseVisualStyleBackColor = true;
			// 
			// lAccuracy
			// 
			this->lAccuracy->AutoSize = true;
			this->lAccuracy->BorderStyle = System::Windows::Forms::BorderStyle::FixedSingle;
			this->lAccuracy->Font = (gcnew System::Drawing::Font(L"Calibri", 11.25F, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(204)));
			this->lAccuracy->Location = System::Drawing::Point(3, 80);
			this->lAccuracy->Name = L"lAccuracy";
			this->lAccuracy->Size = System::Drawing::Size(84, 20);
			this->lAccuracy->TabIndex = 7;
			this->lAccuracy->Text = L"Accuracy (ε)";
			// 
			// tbAccuracy
			// 
			this->tbAccuracy->Font = (gcnew System::Drawing::Font(L"Calibri", 9, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(204)));
			this->tbAccuracy->Location = System::Drawing::Point(144, 80);
			this->tbAccuracy->Name = L"tbAccuracy";
			this->tbAccuracy->Size = System::Drawing::Size(45, 22);
			this->tbAccuracy->TabIndex = 6;
			this->tbAccuracy->Text = L"0.001";
			// 
			// lStretch
			// 
			this->lStretch->AutoSize = true;
			this->lStretch->Font = (gcnew System::Drawing::Font(L"Calibri", 11.25F, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(204)));
			this->lStretch->Location = System::Drawing::Point(5, 55);
			this->lStretch->Name = L"lStretch";
			this->lStretch->Size = System::Drawing::Size(72, 18);
			this->lStretch->TabIndex = 5;
			this->lStretch->Text = L"Stretch (γ)";
			// 
			// lCompression
			// 
			this->lCompression->AutoSize = true;
			this->lCompression->Font = (gcnew System::Drawing::Font(L"Calibri", 11.25F, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(204)));
			this->lCompression->Location = System::Drawing::Point(5, 30);
			this->lCompression->Name = L"lCompression";
			this->lCompression->Size = System::Drawing::Size(110, 18);
			this->lCompression->TabIndex = 4;
			this->lCompression->Text = L"Compression (β)";
			// 
			// tbStretch
			// 
			this->tbStretch->Font = (gcnew System::Drawing::Font(L"Calibri", 9, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(204)));
			this->tbStretch->Location = System::Drawing::Point(144, 55);
			this->tbStretch->Name = L"tbStretch";
			this->tbStretch->Size = System::Drawing::Size(45, 22);
			this->tbStretch->TabIndex = 3;
			this->tbStretch->Text = L"2";
			// 
			// tbCompression
			// 
			this->tbCompression->Font = (gcnew System::Drawing::Font(L"Calibri", 9, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(204)));
			this->tbCompression->Location = System::Drawing::Point(144, 30);
			this->tbCompression->Name = L"tbCompression";
			this->tbCompression->Size = System::Drawing::Size(45, 22);
			this->tbCompression->TabIndex = 2;
			this->tbCompression->Text = L"0.5";
			// 
			// tbReflection
			// 
			this->tbReflection->Font = (gcnew System::Drawing::Font(L"Calibri", 9, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(204)));
			this->tbReflection->Location = System::Drawing::Point(144, 5);
			this->tbReflection->Name = L"tbReflection";
			this->tbReflection->Size = System::Drawing::Size(45, 22);
			this->tbReflection->TabIndex = 1;
			this->tbReflection->Text = L"1";
			// 
			// lReflection
			// 
			this->lReflection->AutoSize = true;
			this->lReflection->Font = (gcnew System::Drawing::Font(L"Calibri", 11.25F, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(204)));
			this->lReflection->Location = System::Drawing::Point(5, 5);
			this->lReflection->Name = L"lReflection";
			this->lReflection->Size = System::Drawing::Size(92, 18);
			this->lReflection->TabIndex = 1;
			this->lReflection->Text = L"Reflection (α)";
			// 
			// pMainImage
			// 
			this->pMainImage->Anchor = System::Windows::Forms::AnchorStyles::None;
			this->pMainImage->BorderStyle = System::Windows::Forms::BorderStyle::FixedSingle;
			this->pMainImage->Controls->Add(this->pbMain);
			this->pMainImage->Location = System::Drawing::Point(211, 5);
			this->pMainImage->Name = L"pMainImage";
			this->pMainImage->Size = System::Drawing::Size(426, 426);
			this->pMainImage->TabIndex = 3;
			// 
			// pbMain
			// 
			this->pbMain->Dock = System::Windows::Forms::DockStyle::Fill;
			this->pbMain->Location = System::Drawing::Point(0, 0);
			this->pbMain->Name = L"pbMain";
			this->pbMain->Size = System::Drawing::Size(424, 424);
			this->pbMain->SizeMode = System::Windows::Forms::PictureBoxSizeMode::Zoom;
			this->pbMain->TabIndex = 0;
			this->pbMain->TabStop = false;
			// 
			// ofdImage
			// 
			this->ofdImage->FileName = L"openFileDialog1";
			// 
			// lbIterations
			// 
			this->lbIterations->AutoSize = true;
			this->lbIterations->Font = (gcnew System::Drawing::Font(L"Calibri", 11.25F, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(204)));
			this->lbIterations->Location = System::Drawing::Point(5, 125);
			this->lbIterations->Name = L"lbIterations";
			this->lbIterations->Size = System::Drawing::Size(67, 18);
			this->lbIterations->TabIndex = 9;
			this->lbIterations->Text = L"Iterations";
			// 
			// tbIterations
			// 
			this->tbIterations->Font = (gcnew System::Drawing::Font(L"Calibri", 9, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(204)));
			this->tbIterations->Location = System::Drawing::Point(144, 125);
			this->tbIterations->Name = L"tbIterations";
			this->tbIterations->Size = System::Drawing::Size(45, 22);
			this->tbIterations->TabIndex = 10;
			this->tbIterations->Text = L"100";
			// 
			// MainForm
			// 
			this->AutoScaleDimensions = System::Drawing::SizeF(6, 13);
			this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
			this->BackColor = System::Drawing::SystemColors::Control;
			this->ClientSize = System::Drawing::Size(643, 461);
			this->Controls->Add(this->tlpImages);
			this->Controls->Add(this->menu);
			this->Font = (gcnew System::Drawing::Font(L"Calibri", 8.25F, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(204)));
			this->Icon = (cli::safe_cast<System::Drawing::Icon^>(resources->GetObject(L"$this.Icon")));
			this->MainMenuStrip = this->menu;
			this->MaximizeBox = false;
			this->MaximumSize = System::Drawing::Size(659, 500);
			this->MinimizeBox = false;
			this->MinimumSize = System::Drawing::Size(659, 500);
			this->Name = L"MainForm";
			this->StartPosition = System::Windows::Forms::FormStartPosition::CenterScreen;
			this->Text = L"Correlation Alignment";
			this->menu->ResumeLayout(false);
			this->menu->PerformLayout();
			this->tlpImages->ResumeLayout(false);
			this->tableLayoutPanel1->ResumeLayout(false);
			this->pSubImage->ResumeLayout(false);
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->pbSub))->EndInit();
			this->pOutput->ResumeLayout(false);
			this->pOutput->PerformLayout();
			this->pInput->ResumeLayout(false);
			this->pInput->PerformLayout();
			this->pMethod->ResumeLayout(false);
			this->pMethod->PerformLayout();
			this->pMainImage->ResumeLayout(false);
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->pbMain))->EndInit();
			this->ResumeLayout(false);
			this->PerformLayout();

		}
#pragma endregion
	private: 
		struct img* MainImage = new img();
		struct img* SubImage = new img();

		System::Void menuLightTheme_Click(System::Object^ sender, System::EventArgs^ e);
		System::Void menuDarkTheme_Click(System::Object^ sender, System::EventArgs^ e);
		System::Void menuBlueTheme_Click(System::Object^ sender, System::EventArgs^ e);
		System::Void menuLCoralTheme_Click(System::Object^ sender, System::EventArgs^ e);
		System::Void menuGreenTheme_Click(System::Object^ sender, System::EventArgs^ e);
		
		System::Void menuOpenMain_Click(System::Object^ sender, System::EventArgs^ e);
		System::Void menuOpenSub_Click(System::Object^ sender, System::EventArgs^ e);
		System::Void menuCalculate_Click(System::Object^ sender, System::EventArgs^ e);
		System::Void menuExit_Click(System::Object^ sender, System::EventArgs^ e);

		void ChangeTheme(System::Drawing::Color color);

		struct img* GetImageCopy(struct img* image);

		void InitializeImage(struct img* image, PictureBox^ pbSource);

		void Calculate(struct img* mainImage, struct img* subImage, double reflection, 
			double compression, double stretch, double accuracy, int iterations);

		long double GetFunctionValue(struct point point, struct img* mainImage, struct img* subImage);

		long double GetBrightness(unsigned pixel);

		struct point GetRandomPoint(int maxX, int maxY);

		struct img* GetGrayImage(struct img* image);

		void CalculateCUDA(struct img* mainImage, struct img* subImage, double reflection, 
			double compression, double stretch, double accuracy, int iterations);

		void CalculateOpenMP(struct img* mainImage, struct img* subImage, double reflection, 
			double compression, double stretch, double accuracy, int iterations);

		void ShowImage(struct img* image, PictureBox^ pictureBox);

		void ShowFoundImage(struct img* mainImage, struct img* subImage, struct point point);

		void ShowResult(String^ message);

		struct img* GetImageWithPoint(struct img* srcImage, struct point point);

		void SortPoints(struct point* points, long double* functions, int pointsCount);

		struct point GetGravityCenter(struct point* points, int pointsCount);

		bool IsImageFound(long double* functions, int pointsCount, long double gravityCenter_f, double accuracy, long double& currentAccuracy);

		struct point GetReflectedPoint(struct point gravityCenter_p, struct point worst_p, double reflection, int maxX, int maxY);

		struct point GetStretchedPoint(struct point gravityCenter_p, struct point reflected_p, double stretch, int maxX, int maxY);

		struct point GetCompressedPoint(struct point gravityCenter_p, struct point worst_p, double compression, int maxX, int maxY);

		struct point* GetReducedPoints(struct point* points, struct point best_p, int pointsCount, int maxX, int maxY);
	};
}
