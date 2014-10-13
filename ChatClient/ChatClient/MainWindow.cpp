#include "MainWindow.h"
#include "SocketManager.h"
#include <msclr/marshal_cppstd.h>
#include <boost\bind.hpp>
#include <iostream>

using namespace ChatClient;

void onRead(std::string &login, std::string &message)
{
	MessageBox::Show(gcnew String(login.c_str()), gcnew String(message.c_str()));
}

void MainWindow::InitializeComponent()
{
	this->SuspendLayout();
	this->AutoScaleDimensions = System::Drawing::SizeF(6, 13);
	this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
	this->ClientSize = System::Drawing::Size(500, 500);
	this->Name = L"Chat";
	this->Text = L"Chat";
	this->ResumeLayout(false);
	FormClosedEventHandler^ handler = gcnew FormClosedEventHandler(this, &MainWindow::MainWindow_Closing);
	this->FormClosed += handler;

	messageTextBox = gcnew TextBox();
	messageTextBox->Top = 400;
	messageTextBox->Height = 100;
	messageTextBox->Width = 500;
	messageTextBox->Multiline = true;
	messageTextBox->KeyPress += gcnew KeyPressEventHandler(this, &MainWindow::MessageTextBox_KeyPress);
	this->Controls->Add(messageTextBox);
}

void MainWindow::MessageTextBox_KeyPress(Object^ obj, KeyPressEventArgs^ e)
{
	if (e->KeyChar == 13)
	{
		std::string message = msclr::interop::marshal_as<std::string>(messageTextBox->Text);
		if (!messageSent)
		{
			SocketManager::getInstance()->write(message, &onRead);
			messageSent = true;
		}
		else
			SocketManager::getInstance()->write(message);
		messageTextBox->Clear();
	}
}