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

	struct mailingData {
		short int type;
		char message[512];
	};
	short int mailiing_status;

	using namespace System;
	using namespace System::ComponentModel;
	using namespace System::Collections;
	using namespace System::Windows::Forms;
	using namespace System::Data;
	using namespace System::Drawing;
	using namespace System::Threading;
	

	public ref class MyForm : public System::Windows::Forms::Form
	{

	public:
		SOCKET clientSocket;
		bool wf = false;
		bool sp = false;
		bool er = false;	

	public:
		MyForm(void)
		{
			InitializeComponent();
			try {

				WSADATA wsaData;
				if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
					cerr << "Failed to initialize Winsock!" << endl;
					String^ err = "Failed to initialize Winsock!";
					throw err;
				}

				sockaddr_in serverAddress{};
				// Use inet_pton to convert an IP address from a string representation
				if (inet_pton(AF_INET, "127.0.0.1", &(serverAddress.sin_addr)) != 1) {
					cerr << "Invalid IP address" << endl;
					WSACleanup();
					String^ err = "Invalid IP address";
					throw err;

				}
				serverAddress.sin_port = htons(PORT);
				serverAddress.sin_family = AF_INET;

				// Creating client socket
				// AF_INET - internet socket
				// SOCK_STREAM - stream socket (with creating a connection)
				// NULL - default TCP protocol
				clientSocket = socket(AF_INET, SOCK_STREAM, 0);
				if (clientSocket == INVALID_SOCKET) {
					cerr << "Error creating socket" << endl;
					WSACleanup();
					String^ err = "Error creating socket";
					throw err;
				}

				if (connect(clientSocket, reinterpret_cast<sockaddr*>(&serverAddress), sizeof(serverAddress)) != 0) {
					cerr << "Connection failed" << std::endl;
					closesocket(clientSocket);
					WSACleanup();
					String^ err = "Connection failed";
					throw err;
				}
			}
			catch (String^ ex) {

				MessageBox::Show(ex, "Error", MessageBoxButtons::OK, MessageBoxIcon::Error);
				exit(-1);
			}

			mailiing_status = 0b000;
			Listening();
		}

	public: void obtainingData() {

		while (true) {

			mailingData receivedPacket;
			receivedPacket.type = 0b000;
			memset(receivedPacket.message, '\0', sizeof(receivedPacket.message));

			recv(clientSocket, reinterpret_cast<char*>(&receivedPacket), sizeof(mailingData), 0);


			if ((receivedPacket.type & 0b001) == 0b001) {

				tb_sp->Text = (gcnew String(receivedPacket.message));
			}
			else if ((receivedPacket.type & 0b010) == 0b010) {

				tb_er->Text = (gcnew String(receivedPacket.message));
			}
			else if ((receivedPacket.type & 0b100) == 0b100) {

				tb_wf->Text = (gcnew String(receivedPacket.message));
			}
			else {
				tb_sp->Text = (gcnew String("Ça korablem :)))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))"));
				tb_er->Text = (gcnew String("Ça korablem :)))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))"));
				tb_wf->Text = (gcnew String("Ça korablem :)))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))"));
				continue;
			}
		}
	}
	
	private: System::Void btn_wf_Click(System::Object^ sender, System::EventArgs^ e) {
		
		if (wf) {
			btn_wf->Text = "Subscribe";
			tb_wf->Text = "You are not subscribed.";
			
			short int subscrp = 0b011; 
			mailiing_status = mailiing_status & subscrp;
			send(clientSocket, (char*)&mailiing_status, sizeof(mailiing_status), 0);
			wf = false;
		}
		else {
			btn_wf->Text = "Unsubscribe";
			tb_wf->Text = "";
			
			short int subscrp = 0b100;
			mailiing_status = subscrp | mailiing_status;
			send(clientSocket, (char*)&mailiing_status, sizeof(mailiing_status), 0);
			wf = true;
		}
	}
		   System::Void btn_sp_Click(System::Object^ sender, System::EventArgs^ e) {
			
			   if (sp) {
				   btn_sp->Text = "Subscribe";
				   tb_sp->Text = "You are not subscribed.";
				  
				   short int subscrp = 0b110;
				   mailiing_status = mailiing_status & subscrp;
				   send(clientSocket, (char*)&mailiing_status, sizeof(mailiing_status), 0);
				   sp = false;
			   }
			   else {
				   btn_sp->Text = "Unsubscribe";
				   tb_sp->Text = "";
				
				   short int subscrp = 0b001;
				   mailiing_status = mailiing_status | subscrp;
				   send(clientSocket, (char*)&mailiing_status, sizeof(mailiing_status), 0);
				   sp = true;
			   }
		   }
		   System::Void btn_er_Click(System::Object^ sender, System::EventArgs^ e) {
			
			   if (er) {
				   btn_er->Text = "Subscribe";
				   tb_er->Text = "You are not subscribed.";
				 
				   short int subscrp = 0b101;
				   mailiing_status = mailiing_status & subscrp;
				   send(clientSocket, (char*)&mailiing_status, sizeof(mailiing_status), 0);
				   er = false;
			   }
			   else {
				   btn_er->Text = "Unsubscribe";
				   tb_er->Text = "";
				  
				   short int subscrp = 0b010;
				   mailiing_status = mailiing_status | subscrp;
				   send(clientSocket, (char*)&mailiing_status, sizeof(mailiing_status), 0);
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
		
		~MyForm()
		{
			if (components)
			{
				delete components;
			}
		}


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
			this->label2 = (gcnew System::Windows::Forms::Label());
			this->label3 = (gcnew System::Windows::Forms::Label());
			this->label4 = (gcnew System::Windows::Forms::Label());
			this->tb_wf = (gcnew System::Windows::Forms::RichTextBox());
			this->tb_sp = (gcnew System::Windows::Forms::RichTextBox());
			this->tb_er = (gcnew System::Windows::Forms::RichTextBox());
			this->btn_wf = (gcnew System::Windows::Forms::Button());
			this->btn_sp = (gcnew System::Windows::Forms::Button());
			this->btn_er = (gcnew System::Windows::Forms::Button());
			this->SuspendLayout();
			// 
			// label2
			// 
			this->label2->AutoSize = true;
			this->label2->BackColor = System::Drawing::Color::SteelBlue;
			this->label2->Font = (gcnew System::Drawing::Font(L"Microsoft JhengHei UI", 13.8F, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(0)));
			this->label2->ForeColor = System::Drawing::Color::Black;
			this->label2->Location = System::Drawing::Point(29, 260);
			this->label2->Margin = System::Windows::Forms::Padding(4, 0, 4, 0);
			this->label2->Name = L"label2";
			this->label2->Size = System::Drawing::Size(261, 29);
			this->label2->TabIndex = 1;
			this->label2->Text = L"Weather forecast";
			// 
			// label3
			// 
			this->label3->AutoSize = true;
			this->label3->Font = (gcnew System::Drawing::Font(L"Microsoft JhengHei UI", 13.8F, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(0)));
			this->label3->Location = System::Drawing::Point(29, 34);
			this->label3->Margin = System::Windows::Forms::Padding(4, 0, 4, 0);
			this->label3->Name = L"label3";
			this->label3->Size = System::Drawing::Size(184, 29);
			this->label3->TabIndex = 2;
			this->label3->Text = L"Share price";
			//this->label3->Click += gcnew System::EventHandler(this, &MyForm::label3_Click);
			// 
			// label4
			// 
			this->label4->AutoSize = true;
			this->label4->Font = (gcnew System::Drawing::Font(L"Microsoft JhengHei UI", 13.8F, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(0)));
			this->label4->Location = System::Drawing::Point(29, 492);
			this->label4->Margin = System::Windows::Forms::Padding(4, 0, 4, 0);
			this->label4->Name = L"label4";
			this->label4->Size = System::Drawing::Size(264, 29);
			this->label4->TabIndex = 3;
			this->label4->Text = L"Exchange rates";
			// 
			// tb_wf
			// 
			this->tb_wf->Location = System::Drawing::Point(31, 293);
			this->tb_wf->Margin = System::Windows::Forms::Padding(4);
			this->tb_wf->Name = L"tb_wf";
			this->tb_wf->Size = System::Drawing::Size(363, 127);
			this->tb_wf->TabIndex = 4;
			this->tb_wf->Text = L"";
			// 
			// tb_sp
			// 
			this->tb_sp->Location = System::Drawing::Point(28, 67);
			this->tb_sp->Margin = System::Windows::Forms::Padding(4);
			this->tb_sp->Name = L"tb_sp";
			this->tb_sp->Size = System::Drawing::Size(364, 127);
			this->tb_sp->TabIndex = 5;
			this->tb_sp->Text = L"";
			// 
			// tb_er
			// 
			this->tb_er->Location = System::Drawing::Point(32, 525);
			this->tb_er->Margin = System::Windows::Forms::Padding(4);
			this->tb_er->Name = L"tb_er";
			this->tb_er->Size = System::Drawing::Size(363, 127);
			this->tb_er->TabIndex = 6;
			this->tb_er->Text = L"";
			// 
			// btn_wf
			// 
			this->btn_wf->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 12, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(204)));
			this->btn_wf->Location = System::Drawing::Point(443, 325);
			this->btn_wf->Margin = System::Windows::Forms::Padding(4);
			this->btn_wf->Name = L"btn_wf";
			this->btn_wf->Size = System::Drawing::Size(228, 58);
			this->btn_wf->TabIndex = 7;
			this->btn_wf->Text = L"Subscribe";
			this->btn_wf->UseVisualStyleBackColor = true;
			this->btn_wf->Click += gcnew System::EventHandler(this, &MyForm::btn_wf_Click);
			// 
			// btn_sp
			// 
			this->btn_sp->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 12, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(204)));
			this->btn_sp->Location = System::Drawing::Point(441, 102);
			this->btn_sp->Margin = System::Windows::Forms::Padding(4);
			this->btn_sp->Name = L"btn_sp";
			this->btn_sp->Size = System::Drawing::Size(228, 58);
			this->btn_sp->TabIndex = 8;
			this->btn_sp->Text = L"Subscribe";
			this->btn_sp->UseVisualStyleBackColor = true;
			this->btn_sp->Click += gcnew System::EventHandler(this, &MyForm::btn_sp_Click);
			// 
			// btn_er
			// 
			this->btn_er->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 12, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(204)));
			this->btn_er->Location = System::Drawing::Point(444, 558);
			this->btn_er->Margin = System::Windows::Forms::Padding(4);
			this->btn_er->Name = L"btn_er";
			this->btn_er->Size = System::Drawing::Size(228, 58);
			this->btn_er->TabIndex = 9;
			this->btn_er->Text = L"Subscribe";
			this->btn_er->UseVisualStyleBackColor = true;
			this->btn_er->Click += gcnew System::EventHandler(this, &MyForm::btn_er_Click);
			// 
			// MyForm
			// 
			this->AutoScaleDimensions = System::Drawing::SizeF(8, 16);
			this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
			this->BackColor = System::Drawing::Color::SteelBlue;
			this->ClientSize = System::Drawing::Size(710, 711);
			this->Controls->Add(this->btn_er);
			this->Controls->Add(this->btn_sp);
			this->Controls->Add(this->btn_wf);
			this->Controls->Add(this->tb_er);
			this->Controls->Add(this->tb_sp);
			this->Controls->Add(this->tb_wf);
			this->Controls->Add(this->label4);
			this->Controls->Add(this->label3);
			this->Controls->Add(this->label2);
			this->Icon = (cli::safe_cast<System::Drawing::Icon^>(resources->GetObject(L"$this.Icon")));
			this->Margin = System::Windows::Forms::Padding(4);
			this->Name = L"MyForm";
			this->Text = L"Mailing";
			this->FormClosing += gcnew System::Windows::Forms::FormClosingEventHandler(this, &MyForm::MyForm_FormClosing);
			this->ResumeLayout(false);
			this->PerformLayout();

		}
#pragma endregion

};
}
