#pragma once
#include <iostream>
#include "SocketManager.h"

namespace ChatClient
{
	using namespace System;
	using namespace System::ComponentModel;
	using namespace System::Collections;
	using namespace System::Windows::Forms;
	using namespace System::Data;
	using namespace System::Drawing;

	/// <summary>
	/// Summary for ConnectionWindow
	/// </summary>
	public ref class MainWindow : public System::Windows::Forms::Form
	{
		TextBox ^messageTextBox;
		DataGridView ^messageLog;
	public:
		MainWindow(void) { InitializeComponent(); }

		static MainWindow^ instance;

		static MainWindow^ getInstance() { if (!instance) instance = gcnew MainWindow(); return instance; }

		void MainWindow_Closing(Object^ obj, FormClosedEventArgs^ e) { Application::Exit(); }

		void MessageTextBox_KeyEvent(Object^ obj, KeyEventArgs^ e);

		delegate void updateLog(String ^login, String ^message);
		static updateLog ^logDelegate;

		void updateLogMethod(String ^login, String ^message);

	protected:
		/// <summary>
		/// Clean up any resources being used.
		/// </summary>
		~MainWindow() { if (components) delete components; }

	private:
		/// <summary>
		/// Required designer variable.
		/// </summary>
		System::ComponentModel::Container ^components;
		/// <summary>
		/// Required method for Designer support - do not modify
		/// the contents of this method with the code editor.
		/// </summary>
		void InitializeComponent(void);
	};
}
