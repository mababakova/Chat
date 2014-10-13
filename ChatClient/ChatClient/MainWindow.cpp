#include "MainWindow.h"
#include "SocketManager.h"
#include <msclr/marshal_cppstd.h>
#include <boost\bind.hpp>
#include <iostream>

using namespace ChatClient;

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
}