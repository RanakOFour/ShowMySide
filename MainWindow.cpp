#include "MainWindow.h"
#include "Timer.h"
#include "Host.h"
#include "Client.h"
#include "ImagePool.h"
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


MainWindow::MainWindow(int _w, int _h, std::string _name)
	: Fl_Double_Window(200, 200, _w, _h, "Show My Side"),
	m_Client(nullptr),
	m_Host(nullptr),
	m_ipInput(nullptr),
	m_lobbyOutputLog(nullptr)
{
	ChangeLayout(LayoutType::SPLASH_SCREEN);
	icon(ImagePool::GetImage(ImagePool::ImageType::ICON));
}

MainWindow::~MainWindow()
{
}

bool MainWindow::AttemptConnection(std::string& _ipAddress)
{
	if (m_Client->Connect(_ipAddress))
	{
		return true;
	}

	return false;
}

void MainWindow::Send(std::string& _msg)
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
		menu->add("&Start/&Join Server", NULL, OnClientStart, this);
		menu->add("&Start/&Create Server", NULL, OnServerStart, this, FL_MENU_DIVIDER);
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
		m_ipInput = new Fl_Input(100, 100, 120, 25, "Enter IP here: ");
		add(m_ipInput);

		//Forces the textbox to be shown on the screen
		m_ipInput->value(" ");
		m_ipInput->value("");
		m_ipInput->box(FL_ENGRAVED_BOX);
		m_ipInput->redraw_label();

		//Show connect button to confirm connection
		Fl_Button* connectBtn = new Fl_Button(270, 260, 100, 50, "Connect");
		connectBtn->callback(OnJoinServer, (void*)this);
		add(connectBtn);

		connectBtn->redraw();
	}
	break;


	//Overlay for ingame and host is the same
	case HOST:
	{
		//Display server IP address for other clients to connect
		Fl_Output* ipAddress = new Fl_Output(58, 30, 95, 25, "Local IP:");
		ipAddress->value(m_Host->GetIPAddress().c_str());
		ipAddress->box(FL_NO_BOX);

		add(ipAddress);

		ipAddress->redraw_label();
	}

	case IN_GAME:
	{
		m_lobbyOutputLog = new Fl_Output(5, 60, w() - 10, h() - 65);
		add(m_lobbyOutputLog);

		m_Client->SetOutputLog(m_lobbyOutputLog);
	}
	break;
	}

	redraw();
}

void MainWindow::OnClientStart(Fl_Widget* _widget, void* _userData)
{

	MainWindow* mw = (MainWindow*)_userData;
	//Start client
	mw->m_Client = new Client();


	mw->ChangeLayout(JOIN_GAME);
}

void MainWindow::OnServerStart(Fl_Widget* _widget, void* _userData)
{

	MainWindow* mw = (MainWindow*)_userData;
	//Start a new host and connect client to it
	
							 //port, timer duration
	mw->m_Host = new Host(8080, 0.25);

	mw->m_Client = new Client();

	std::string ip = mw->m_Host->GetIPAddress();
	mw->m_Client->Connect(ip);


	mw->ChangeLayout(HOST);
}

void MainWindow::OnJoinServer(Fl_Widget* _widget, void* _userData)
{

	MainWindow* mw = (MainWindow*)_userData;

	std::string ip = mw->m_ipInput->value();

	printf("IP: %s\n", ip.c_str());

	if (mw->AttemptConnection(ip))
	{
		mw->ChangeLayout(IN_GAME);
	}
}


void MainWindow::OnExit(Fl_Widget* _widget, void* _userData)
{
	MainWindow* mw = (MainWindow*)_userData;
	mw->hide();
}