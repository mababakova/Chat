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
	public ref class ConnectionWindow : public System::Windows::Forms::Form
	{
		Label ^descriptionLabel;
		TextBox ^serverAddressTextBox;
		static TextBox ^loginTextBox, ^pwdTextBox;
		Button ^connectButton;
		static ToolStripStatusLabel^ statusLabel;
		static StatusStrip ^status;
	public:
		ConnectionWindow(void) { InitializeComponent(); }

		static void updateStatusStrip(const std::string &message);

		static void autorize();

	protected:
		/// <summary>
		/// Clean up any resources being used.
		/// </summary>
		~ConnectionWindow() { if (components) delete components; }

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

		void onConnectButtonClick(Object^ sender, System::EventArgs^ e);
	};
}
