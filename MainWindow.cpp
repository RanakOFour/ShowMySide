#include "MainWindow.h"
#include "Timer.h"
#include "Host.h"
#include "Client.h"
#include <string>
#include <iostream>
#include <thread>
#include <FL/Fl_Group.H>
#include <FL/Fl_Multiline_Input.H>
#include <FL/Fl_Menu_Bar.H>
#include <FL/Fl_Input.H>
#include <FL/Fl_Output.H>
#include <FL/Fl_Flex.H>
#include <FL/Fl_Button.H>

/*
	Weird singleton - ish member so that I can access it within fltk callbacks
	while also encapsulating the layouts into their own structs passed into said callbacks
*/
static MainWindow* m_Main;

MainWindow::MainWindow(int _w, int _h, std::string _name)
	: Fl_Double_Window(200, 200, _w, _h, _name.c_str()),
	m_Client(nullptr),
	m_Host(nullptr)
{
	m_Main = this;
	ChangeLayout(LayoutType::SPLASH_SCREEN);
}

MainWindow::~MainWindow()
{
}

bool MainWindow::AttemptConnection(const char* _ipAddress)
{
	if (m_Client->Connect(_ipAddress))
	{
		return true;
	}

	return false;
}

void MainWindow::Send(const std::string _msg)
{
	m_Client->Send(_msg);
}

void MainWindow::ChangeLayout(LayoutType _newState)
{
	
	//Leaves the menubar intact
	while (children() > 1)
	{
		delete_child(1);
	}


	//Switch is secretly goto, so braces are needed to for the compiler to like declaractions inside
	switch (_newState)
	{
		// Start menu
	case SPLASH_SCREEN:
	{
		Fl_Menu_Bar* menu = new Fl_Menu_Bar(0, 0, w(), 30, "");
		menu->add("&Start/&Join Server", NULL, OnClientStart, NULL);
		menu->add("&Start/&Create Server", NULL, OnServerStart, NULL, FL_MENU_DIVIDER);
		menu->add("&Start/&Exit", NULL, OnExit, this);

		Fl_Flex* menuFlex = new Fl_Flex(menu->w(), menu->h(), 0);
		menuFlex->add(menu);
		menuFlex->fixed(menu, 0);
		add_resizable(*menuFlex);
		add(menuFlex);
	}
	break;

	// Client pre-connection state
	case JOIN_GAME:
	{
		Fl_Input* ipAddressInput = new Fl_Input(100, 100, 120, 25, "Enter IP here: ");
		add(ipAddressInput);

		//Forces the textbox to be shown on the screen
		ipAddressInput->value(" ");
		ipAddressInput->value("");
		ipAddressInput->box(FL_ENGRAVED_BOX);
		ipAddressInput->redraw_label();

		//Show connect button to confirm connection
		Fl_Button* connectBtn = new Fl_Button(270, 260, 100, 50, "Connect");
		connectBtn->callback(OnJoinServer, (void*)ipAddressInput);
		add(connectBtn);

		connectBtn->redraw();
	}
	break;


	//Overlay for ingame and host is the same
	case HOST:
	{
		//Display server IP address for other clients to connect
		Fl_Output* ipAddress = new Fl_Output(58, 30, 95, 25, "Local IP:");
		ipAddress->value(m_Host->GetIP().c_str());
		ipAddress->box(FL_NO_BOX);

		add(ipAddress);

		ipAddress->redraw_label();
	}
	case IN_GAME:
	{
		Fl_Output* outputBox = new Fl_Output(0, 60, w(), 200);
		Fl_Multiline_Input* messageInput = new Fl_Multiline_Input(220, 260, 200, 50, "Enter Text");
		Fl_Button* sendMessageBtn = new Fl_Button(270, 320, 100, 50, "Send");

		add(outputBox);
		add(messageInput);
		add(sendMessageBtn);
		sendMessageBtn->callback(OnSendData, (void*)messageInput);
	}
	break;
	}

	redraw();
}

void MainWindow::OnClientStart(Fl_Widget* _widget, void* _userData)
{
	//Start client
	m_Main->m_Client = new Client();


	m_Main->ChangeLayout(JOIN_GAME);
}

void MainWindow::OnServerStart(Fl_Widget* _widget, void* _userData)
{
	//Start a new host and connect client to it
	
							 //port, timer duration
	m_Main->m_Host = new Host(8080, 0.25);

	m_Main->m_Client = new Client();
	m_Main->m_Client->Connect(m_Main->m_Host->GetIP().c_str());



	m_Main->ChangeLayout(HOST);
}

void MainWindow::OnJoinServer(Fl_Widget* _widget, void* _userData)
{
	//Search to connect to given ip address
	Fl_Input* ipAddressInput = (Fl_Input*)_userData;

	const char* ip = ipAddressInput->value();
	printf("IP: %s\n", ip);

	if (m_Main->AttemptConnection(ip))
	{
		m_Main->ChangeLayout(IN_GAME);
	}
}

void MainWindow::OnSendData(Fl_Widget* _widget, void* _userData)
{
	// Send data to server

	Fl_Multiline_Input* messageBox = (Fl_Multiline_Input*)_userData;
	const std::string textToSend = messageBox->value();
	printf("Attempting to send: %s\n", textToSend.c_str());
	m_Main->Send(textToSend);
}


void MainWindow::OnExit(Fl_Widget* _widget, void* _userData)
{
	MainWindow* mw = (MainWindow*)_userData;
	mw->hide();
}