#include "MainWindow.h"
#include "SocketManager.h"
#include <msclr/marshal_cppstd.h>
#include <boost\bind.hpp>
#include <iostream>

using namespace ChatClient;

void onRead(std::string &login, std::string &message)
{
	MainWindow::getInstance()->BeginInvoke(MainWindow::logDelegate, gcnew String(login.c_str()), gcnew String(message.c_str()));
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

	System::Drawing::Icon ^icon = gcnew System::Drawing::Icon("icon.ico");
	Icon = icon;

	messageTextBox = gcnew TextBox();
	messageTextBox->Top = 400;
	messageTextBox->Height = 100;
	messageTextBox->Width = 500;
	messageTextBox->Multiline = true;
	messageTextBox->KeyUp += gcnew KeyEventHandler(this, &MainWindow::MessageTextBox_KeyEvent);
	this->Controls->Add(messageTextBox);

	messageLog = gcnew DataGridView();
	messageLog->RowHeadersVisible = false;
	messageLog->ColumnHeadersVisible = false;
	messageLog->Width = 500;
	messageLog->Height = 395;
	messageLog->BackgroundColor = Color::White;
	messageLog->BorderStyle = BorderStyle::None;
	messageLog->CellBorderStyle = DataGridViewCellBorderStyle::None;
	DataGridViewTextBoxColumn^ column1 = gcnew DataGridViewTextBoxColumn();
	column1->Width = 50;
	column1->DefaultCellStyle->ForeColor = Color::Blue;
	column1->DefaultCellStyle->SelectionBackColor = messageLog->DefaultCellStyle->BackColor;
	column1->DefaultCellStyle->SelectionForeColor = Color::Blue;
	column1->DefaultCellStyle->WrapMode = DataGridViewTriState::True;
	column1->DefaultCellStyle->Alignment = DataGridViewContentAlignment::TopLeft;
	column1->ReadOnly = true;
	messageLog->Columns->Add(column1);
	DataGridViewTextBoxColumn^ column2 = gcnew DataGridViewTextBoxColumn();
	column2->Width = 380;
	column2->DefaultCellStyle->SelectionBackColor = messageLog->DefaultCellStyle->BackColor;
	column2->DefaultCellStyle->SelectionForeColor = messageLog->DefaultCellStyle->ForeColor;
	column2->DefaultCellStyle->WrapMode = DataGridViewTriState::True;
	column2->DefaultCellStyle->Alignment = DataGridViewContentAlignment::TopLeft;
	column2->ReadOnly = true;
	messageLog->Columns->Add(column2);
	messageLog->AutoSizeRowsMode = DataGridViewAutoSizeRowsMode::AllCells;
	this->Controls->Add(messageLog);

	logDelegate = gcnew updateLog(this, &MainWindow::updateLogMethod);

	SocketManager::getInstance()->initializeReadCallback(&onRead);
}

void MainWindow::updateLogMethod(String ^login, String ^message)
{
	DataGridViewRow ^row = gcnew DataGridViewRow();
	row->CreateCells(messageLog);
	row->Cells[0]->Value = login;
	row->Cells[1]->Value = message;
	messageLog->Rows->Add(row);
	messageLog->Rows[0]->Selected = false;
	messageLog->FirstDisplayedCell = messageLog->Rows[messageLog->Rows->Count - 1]->Cells[0];
	messageLog->Invalidate();
}

void MainWindow::MessageTextBox_KeyEvent(Object^ obj, KeyEventArgs^ e)
{
	if (e->KeyCode == Keys::Enter)
	{
		std::string message = msclr::interop::marshal_as<std::string>(messageTextBox->Text);
		// Remove '\n' in message
		message.pop_back();
		SocketManager::getInstance()->write(message);
		messageTextBox->Clear();
	}
}