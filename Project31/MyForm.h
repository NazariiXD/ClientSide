#pragma once
#include <iostream>
#include <winsock2.h>
#include <ws2tcpip.h>
#pragma comment(lib, "ws2_32.lib")
#include <Windows.h>
#include <vector>
#include <fstream>
#include <sstream>
#include <string>
#include <thread>

using namespace std;

const int PORT = 12345;


namespace Project31 {

	using namespace System;
	using namespace System::ComponentModel;
	using namespace System::Collections;
	using namespace System::Windows::Forms;
	using namespace System::Data;
	using namespace System::Drawing;
	using namespace System::Threading;
	/// <summary>
	/// Summary for MyForm
	/// </summary>
	public ref class MyForm : public System::Windows::Forms::Form
	{
	public:
		SOCKET clientSocket;
		bool wf = false;
		bool sp = false;
		bool er = false;
	private: System::Windows::Forms::PictureBox^ pictureBox1;

	private: System::Windows::Forms::PictureBox^ pictureBox3;
	private: System::Windows::Forms::PictureBox^ pictureBox2;

	public:
		bool isListening = false;

	public:
		MyForm(void)
		{
			InitializeComponent();
			sockaddr_in serverAddress{};
			// Use inet_pton to convert an IP address from a string representation
			if (inet_pton(AF_INET, "127.0.0.1", &(serverAddress.sin_addr)) != 1) {
				cerr << "Invalid IP address" << endl;
				WSACleanup();
				return;
			}
			serverAddress.sin_port = htons(PORT);
			serverAddress.sin_family = AF_INET;

			// Creating server socket
			// AF_INET - internet socket
			// SOCK_STREAM - stream socket (with creating a connection)
			// NULL - default TCP protocol
		


			clientSocket = socket(AF_INET, SOCK_STREAM, NULL);
			if (clientSocket == INVALID_SOCKET) {
				cerr << "Error creating socket" << endl;
				WSACleanup();
				return;
			}

			if (connect(clientSocket, reinterpret_cast<sockaddr*>(&serverAddress), sizeof(serverAddress)) != 0) {
				cerr << "Connection failed" << std::endl;
				closesocket(clientSocket);
				WSACleanup();
				return;
			}


			
			//
			//TODO: Add the constructor code here
			//



		}

	public: void obtainingData() {

		while (true) {
			char* buffer = new char[1000];
			recv(clientSocket, buffer, 1000, 0);
			char version = buffer[0];
			std::string str = buffer;
			str.erase(0, 1);
			if (version == '0') {
				tb_wf->Text = gcnew String(str.c_str());
			}
			else if (version == '1') {
				tb_sp->Text = gcnew String(str.c_str());
			}
			else if (version == '2') {
				tb_er->Text = gcnew String(str.c_str());
			}
			else {
				throw -1;
			}
			memset(buffer, '\0', 1000);


		}
	}
	private: System::Void btn_wf_Click(System::Object^ sender, System::EventArgs^ e) {
		if (!isListening) {
			Listening();
			isListening = true;
		}
		if (wf) {
			btn_wf->Text = "Subscribe";
			tb_wf->Text = "You dont subscribe yet.";
			char* strin2 = new char[2];
			strin2 = "00";
			send(clientSocket, strin2, 2, 0);
			wf = false;
		}
		else {
			btn_wf->Text = "Unsubscribe";
			tb_wf->Text = "";
			char* strin2 = new char[2];
			strin2 = "01";
			send(clientSocket, strin2, 2, 0);
			wf = true;
		}
	}
		   System::Void btn_sp_Click(System::Object^ sender, System::EventArgs^ e) {
			   if (!isListening) {
				   Listening();
				   isListening = true;
			   }
			   if (sp) {
				   btn_sp->Text = "Subscribe";
				   tb_sp->Text = "You dont subscribe yet.";
				   char* strin2 = new char[2];
				   strin2 = "10";
				   send(clientSocket, strin2, 2, 0);
				   sp = false;
			   }
			   else {
				   btn_sp->Text = "Unsubscribe";
				   tb_sp->Text = "";
				   char* strin2 = new char[2];
				   strin2 = "11";
				   send(clientSocket, strin2, 2, 0);
				   //kill thread?
				   sp = true;
			   }
		   }
		   System::Void btn_er_Click(System::Object^ sender, System::EventArgs^ e) {
			   if (!isListening) {
				   Listening();
				   isListening = true;
			   }
			   if (er) {
				   btn_er->Text = "Subscribe";
				   tb_er->Text = "You dont subscribe yet.";
				   char* strin2 = new char[2];
				   strin2 = "20";
				   send(clientSocket, strin2, 2, 0);
				   er = false;
			   }
			   else {
				   btn_er->Text = "Unsubscribe";
				   tb_er->Text = "";
				   char* strin2 = new char[2];
				   strin2 = "21";
				   send(clientSocket, strin2, 2, 0);
				   er = true;
			   }
		   }
		   void Listening() {
			   Thread^ MyThread = nullptr;
			   ThreadStart^ ThreadMethod = gcnew ThreadStart(this, &MyForm::obtainingData);

			   MyThread = gcnew Thread(ThreadMethod);

			   MyThread->Start();
		   }

	private: 
		System::Void MyForm_FormClosing(System::Object^ sender, System::Windows::Forms::FormClosingEventArgs^ e) {
		closesocket(clientSocket);
	}
	


	protected:
		/// <summary>
		/// Clean up any resources being used.
		/// </summary>
		~MyForm()
		{
			if (components)
			{
				delete components;
			}
		}

	private: System::Windows::Forms::Label^ label1;
	protected:
	private: System::Windows::Forms::Label^ label2;
	private: System::Windows::Forms::Label^ label3;
	private: System::Windows::Forms::Label^ label4;
	private: System::Windows::Forms::RichTextBox^ tb_wf;
	private: System::Windows::Forms::RichTextBox^ tb_sp;
	private: System::Windows::Forms::RichTextBox^ tb_er;
	private: System::Windows::Forms::Button^ btn_wf;
	private: System::Windows::Forms::Button^ btn_sp;
	private: System::Windows::Forms::Button^ btn_er;

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
			System::ComponentModel::ComponentResourceManager^ resources = (gcnew System::ComponentModel::ComponentResourceManager(MyForm::typeid));
			this->label1 = (gcnew System::Windows::Forms::Label());
			this->label2 = (gcnew System::Windows::Forms::Label());
			this->label3 = (gcnew System::Windows::Forms::Label());
			this->label4 = (gcnew System::Windows::Forms::Label());
			this->tb_wf = (gcnew System::Windows::Forms::RichTextBox());
			this->tb_sp = (gcnew System::Windows::Forms::RichTextBox());
			this->tb_er = (gcnew System::Windows::Forms::RichTextBox());
			this->btn_wf = (gcnew System::Windows::Forms::Button());
			this->btn_sp = (gcnew System::Windows::Forms::Button());
			this->btn_er = (gcnew System::Windows::Forms::Button());
			this->pictureBox1 = (gcnew System::Windows::Forms::PictureBox());
			this->pictureBox3 = (gcnew System::Windows::Forms::PictureBox());
			this->pictureBox2 = (gcnew System::Windows::Forms::PictureBox());
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->pictureBox1))->BeginInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->pictureBox3))->BeginInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->pictureBox2))->BeginInit();
			this->SuspendLayout();
			// 
			// label1
			// 
			this->label1->AutoSize = true;
			this->label1->Font = (gcnew System::Drawing::Font(L"Perpetua Titling MT", 18, System::Drawing::FontStyle::Bold, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(0)));
			this->label1->Location = System::Drawing::Point(386, 30);
			this->label1->Margin = System::Windows::Forms::Padding(4, 0, 4, 0);
			this->label1->Name = L"label1";
			this->label1->Size = System::Drawing::Size(248, 36);
			this->label1->TabIndex = 0;
			this->label1->Text = L"Fomo Sniffer";
			this->label1->TextAlign = System::Drawing::ContentAlignment::MiddleCenter;
			// 
			// label2
			// 
			this->label2->AutoSize = true;
			this->label2->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 12, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(204)));
			this->label2->Location = System::Drawing::Point(73, 108);
			this->label2->Margin = System::Windows::Forms::Padding(4, 0, 4, 0);
			this->label2->Name = L"label2";
			this->label2->Size = System::Drawing::Size(212, 25);
			this->label2->TabIndex = 1;
			this->label2->Text = L"Weather forecast(60 s)";
			// 
			// label3
			// 
			this->label3->AutoSize = true;
			this->label3->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 12, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(204)));
			this->label3->Location = System::Drawing::Point(425, 397);
			this->label3->Margin = System::Windows::Forms::Padding(4, 0, 4, 0);
			this->label3->Name = L"label3";
			this->label3->Size = System::Drawing::Size(152, 25);
			this->label3->TabIndex = 2;
			this->label3->Text = L"Share price(1 s)";
			// 
			// label4
			// 
			this->label4->AutoSize = true;
			this->label4->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 12, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(204)));
			this->label4->Location = System::Drawing::Point(713, 108);
			this->label4->Margin = System::Windows::Forms::Padding(4, 0, 4, 0);
			this->label4->Name = L"label4";
			this->label4->Size = System::Drawing::Size(221, 25);
			this->label4->TabIndex = 3;
			this->label4->Text = L"Exchange rates(1440 s)";
			// 
			// tb_wf
			// 
			this->tb_wf->Location = System::Drawing::Point(32, 148);
			this->tb_wf->Margin = System::Windows::Forms::Padding(4);
			this->tb_wf->Name = L"tb_wf";
			this->tb_wf->Size = System::Drawing::Size(300, 127);
			this->tb_wf->TabIndex = 4;
			this->tb_wf->Text = L"";
			// 
			// tb_sp
			// 
			this->tb_sp->Location = System::Drawing::Point(359, 437);
			this->tb_sp->Margin = System::Windows::Forms::Padding(4);
			this->tb_sp->Name = L"tb_sp";
			this->tb_sp->Size = System::Drawing::Size(300, 127);
			this->tb_sp->TabIndex = 5;
			this->tb_sp->Text = L"";
			// 
			// tb_er
			// 
			this->tb_er->Location = System::Drawing::Point(684, 148);
			this->tb_er->Margin = System::Windows::Forms::Padding(4);
			this->tb_er->Name = L"tb_er";
			this->tb_er->Size = System::Drawing::Size(300, 127);
			this->tb_er->TabIndex = 6;
			this->tb_er->Text = L"";
			// 
			// btn_wf
			// 
			this->btn_wf->Location = System::Drawing::Point(32, 318);
			this->btn_wf->Margin = System::Windows::Forms::Padding(4);
			this->btn_wf->Name = L"btn_wf";
			this->btn_wf->Size = System::Drawing::Size(301, 58);
			this->btn_wf->TabIndex = 7;
			this->btn_wf->Text = L"Subscribe";
			this->btn_wf->UseVisualStyleBackColor = true;
			this->btn_wf->Click += gcnew System::EventHandler(this, &MyForm::btn_wf_Click);
			// 
			// btn_sp
			// 
			this->btn_sp->Location = System::Drawing::Point(359, 607);
			this->btn_sp->Margin = System::Windows::Forms::Padding(4);
			this->btn_sp->Name = L"btn_sp";
			this->btn_sp->Size = System::Drawing::Size(301, 58);
			this->btn_sp->TabIndex = 8;
			this->btn_sp->Text = L"Subscribe";
			this->btn_sp->UseVisualStyleBackColor = true;
			this->btn_sp->Click += gcnew System::EventHandler(this, &MyForm::btn_sp_Click);
			// 
			// btn_er
			// 
			this->btn_er->Location = System::Drawing::Point(684, 318);
			this->btn_er->Margin = System::Windows::Forms::Padding(4);
			this->btn_er->Name = L"btn_er";
			this->btn_er->Size = System::Drawing::Size(301, 58);
			this->btn_er->TabIndex = 9;
			this->btn_er->Text = L"Subscribe";
			this->btn_er->UseVisualStyleBackColor = true;
			this->btn_er->Click += gcnew System::EventHandler(this, &MyForm::btn_er_Click);
			// 
			// pictureBox1
			// 
			this->pictureBox1->ErrorImage = (cli::safe_cast<System::Drawing::Image^>(resources->GetObject(L"pictureBox1.ErrorImage")));
			this->pictureBox1->ImageLocation = L"patron_fomo_there.jpg";
			this->pictureBox1->InitialImage = (cli::safe_cast<System::Drawing::Image^>(resources->GetObject(L"pictureBox1.InitialImage")));
			this->pictureBox1->Location = System::Drawing::Point(57, 397);
			this->pictureBox1->Name = L"pictureBox1";
			this->pictureBox1->Size = System::Drawing::Size(275, 268);
			this->pictureBox1->SizeMode = System::Windows::Forms::PictureBoxSizeMode::Zoom;
			this->pictureBox1->TabIndex = 10;
			this->pictureBox1->TabStop = false;
			// 
			// pictureBox3
			// 
			this->pictureBox3->ErrorImage = (cli::safe_cast<System::Drawing::Image^>(resources->GetObject(L"pictureBox3.ErrorImage")));
			this->pictureBox3->ImageLocation = L"patron_fomo_found.jpg";
			this->pictureBox3->InitialImage = (cli::safe_cast<System::Drawing::Image^>(resources->GetObject(L"pictureBox3.InitialImage")));
			this->pictureBox3->Location = System::Drawing::Point(372, 108);
			this->pictureBox3->Name = L"pictureBox3";
			this->pictureBox3->Size = System::Drawing::Size(275, 268);
			this->pictureBox3->SizeMode = System::Windows::Forms::PictureBoxSizeMode::Zoom;
			this->pictureBox3->TabIndex = 12;
			this->pictureBox3->TabStop = false;
			// 
			// pictureBox2
			// 
			this->pictureBox2->ErrorImage = (cli::safe_cast<System::Drawing::Image^>(resources->GetObject(L"pictureBox2.ErrorImage")));
			this->pictureBox2->ImageLocation = L"stiker-sobaka-patron-na-raketi_f.jpg";
			this->pictureBox2->InitialImage = (cli::safe_cast<System::Drawing::Image^>(resources->GetObject(L"pictureBox2.InitialImage")));
			this->pictureBox2->Location = System::Drawing::Point(684, 397);
			this->pictureBox2->Name = L"pictureBox2";
			this->pictureBox2->Size = System::Drawing::Size(275, 268);
			this->pictureBox2->SizeMode = System::Windows::Forms::PictureBoxSizeMode::Zoom;
			this->pictureBox2->TabIndex = 13;
			this->pictureBox2->TabStop = false;
			// 
			// MyForm
			// 
			this->AutoScaleDimensions = System::Drawing::SizeF(8, 16);
			this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
			this->BackColor = System::Drawing::Color::Red;
			this->ClientSize = System::Drawing::Size(1024, 707);
			this->Controls->Add(this->pictureBox2);
			this->Controls->Add(this->pictureBox3);
			this->Controls->Add(this->pictureBox1);
			this->Controls->Add(this->btn_er);
			this->Controls->Add(this->btn_sp);
			this->Controls->Add(this->btn_wf);
			this->Controls->Add(this->tb_er);
			this->Controls->Add(this->tb_sp);
			this->Controls->Add(this->tb_wf);
			this->Controls->Add(this->label4);
			this->Controls->Add(this->label3);
			this->Controls->Add(this->label2);
			this->Controls->Add(this->label1);
			this->Icon = (cli::safe_cast<System::Drawing::Icon^>(resources->GetObject(L"$this.Icon")));
			this->Margin = System::Windows::Forms::Padding(4);
			this->Name = L"MyForm";
			this->Text = L"FOMO Sniffer";
			this->FormClosing += gcnew System::Windows::Forms::FormClosingEventHandler(this, &MyForm::MyForm_FormClosing);
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->pictureBox1))->EndInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->pictureBox3))->EndInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->pictureBox2))->EndInit();
			this->ResumeLayout(false);
			this->PerformLayout();

		}
#pragma endregion
	};
}
