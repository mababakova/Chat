#include "ConnectionWindow.h"
#include "MainWindow.h"
#include "SocketManager.h"
#include <msclr/marshal_cppstd.h>
#include <boost\bind.hpp>
#include <iostream>
#include <boost/thread.hpp>

using namespace ChatClient;

void ConnectionWindow::InitializeComponent()
{
	this->SuspendLayout();
	this->AutoScaleDimensions = System::Drawing::SizeF(6, 13);
	this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
	this->ClientSize = System::Drawing::Size(300, 200);
	this->Name = L"Connection";
	this->Text = L"Connection";
	this->ResumeLayout(false);

	descriptionLabel = gcnew Label();
	descriptionLabel->Text = "Enter server IP address and port in format <IP>:<port>,\nlogin and password in fields\nand press the button";
	descriptionLabel->Location = Point(25, 30);
	descriptionLabel->AutoSize = true;
	Controls->Add(this->descriptionLabel);

	serverAddressTextBox = gcnew TextBox();
	serverAddressTextBox->Location = Point(25, 80);
	serverAddressTextBox->Text = "127.0.0.1:33360";
	Controls->Add(serverAddressTextBox);

	loginTextBox = gcnew TextBox();
	loginTextBox->Location = Point(25, 110);
	loginTextBox->Height = 20;
	loginTextBox->Width = 50;
	loginTextBox->Text = "test";
	Controls->Add(loginTextBox);

	pwdTextBox = gcnew TextBox();
	pwdTextBox->Location = Point(80, 110);
	pwdTextBox->Height = 20;
	pwdTextBox->Width = 50;
	pwdTextBox->Text = "test123";
	pwdTextBox->PasswordChar = '*';
	Controls->Add(pwdTextBox);

	connectButton = gcnew Button();
	connectButton->Text = "Connect";
	connectButton->Location = Point(25, 150);
	connectButton->Click += gcnew System::EventHandler(this, &ChatClient::ConnectionWindow::onConnectButtonClick);
	Controls->Add(connectButton);

	status = gcnew StatusStrip();
	Controls->Add(status);

	statusLabel = gcnew ToolStripStatusLabel();
	status->Items->Add(statusLabel);

	switchDelegate = gcnew switchWindow(this, &ConnectionWindow::switchWindowMethod);
}

void ConnectionWindow::switchWindowMethod()
{
	this->Visible = false;
	MainWindow^ chat = MainWindow::getInstance();
	chat->ShowDialog();	
}

void onConnection(bool result)
{
	std::string message;
	if (result)
		message = "Connection was established.";
	else
		message = "Connection was not established. Try again later.";
	ConnectionWindow::updateStatusStrip(message);
	ConnectionWindow::autorize();
}

void onLogin(bool result)
{
	std::string message;
	if (result)
		message = "Client was connected.";
	else
		message = "Client was not connected.";
	ConnectionWindow::updateStatusStrip(message);
	if (result)
	{
		ConnectionWindow::getInstance()->BeginInvoke(ConnectionWindow::switchDelegate);
	}
}

void ConnectionWindow::onConnectButtonClick(Object^ sender, System::EventArgs^ e)
{
	std::string address = msclr::interop::marshal_as<std::string>(serverAddressTextBox->Text);
	std::size_t pos = address.find(":");

	SocketManager::getInstance(address.substr(0, pos), address.substr(pos + 1), boost::bind(&onConnection, _1), boost::bind(&onLogin, _1));
}

void ConnectionWindow::updateStatusStrip(const std::string &message)
{
	statusLabel->Text = gcnew String(message.c_str());
	status->Invalidate();
}

void ConnectionWindow::autorize()
{
	std::string login = msclr::interop::marshal_as<std::string>(loginTextBox->Text);
	std::string pwd = msclr::interop::marshal_as<std::string>(pwdTextBox->Text);
	SocketManager::getInstance()->autorize(login, pwd);
}

[STAThreadAttribute]
int main(array<System::String ^> ^args)
{
	Application::EnableVisualStyles();
	Application::SetCompatibleTextRenderingDefault(false);

	Application::Run(ConnectionWindow::getInstance());
	return 0;
}