#include "MainWindow.h"
#include "Timer.h"
#include "ClientSocket.h"
#include "ServerSocket.h"
#include <string>
#include <iostream>
#include <FL/Fl_Flex.H>
#include <FL/Fl_Button.H>
#include <FL/Fl_Window.H>
#include <FL/Fl_Menu_Bar.H>
#include <FL/Fl_Input.H>
#include <FL/Fl_Output.H>

MainWindow::MainWindow(int _w, int _h, std::string _name) 
	: Fl_Window(200, 200, _w, _h, _name.c_str()), 
	Timer(0.5), 
	m_ipAddr(nullptr),
	m_state(0),
	m_Client(nullptr),
	m_Server(nullptr)
{
	changeInterface(m_state);
	Fl_Menu_Bar* menu = new Fl_Menu_Bar(0, 0, _w, 30, "");
	menu->add("&Start/&Join Server", NULL, onClient, this);
	menu->add("&Start/&Create Server", NULL, onServer, this, FL_MENU_DIVIDER);
	menu->add("&Start/&Exit", NULL, onExit, this);
}

MainWindow::~MainWindow()
{
}

void MainWindow::changeInterface(int _state)
{
	int currentChildren = children();

	// Removes all children except the menu bar
	for (int i = 0; i < currentChildren - 1; i++)
	{
		delete_child(1);
	}
	redraw();

	//Switch is secretly goto, so braces are needed to for the compiler to like declaractions inside
	switch (_state)
	{
		// Main menu
	case 0:
	{
		Fl_Menu_Bar* menu = new Fl_Menu_Bar(0, 0, w(), 30, "");
		menu->add("&Start/&Join Server", NULL, onClient, this);
		menu->add("&Start/&Create Server", NULL, onServer, this, FL_MENU_DIVIDER);
		menu->add("&Start/&Exit", NULL, onExit, this);

		Fl_Flex* menuFlex = new Fl_Flex(menu->w(), menu->h(), 0);
		menuFlex->add(menu);
		menuFlex->fixed(menu, 0);
		add_resizable(*menuFlex);
	}
		break;

		// Client pre-connection state
	case 1:
	{
		std::string ipAddressString = m_Server->m_ipAddress;

		//Display server IP address for other clients to connect
		Fl_Output* ipAddress = new Fl_Output(60, 30, 95, 25, "Local IP:");
		ipAddress->box(FL_NO_BOX);
		add(ipAddress);

		ipAddress->insert(ipAddressString.c_str(), ipAddressString.length());
		ipAddress->redraw_label();
	}
		break;

		// Host state
	case 2:
	{
		m_ipAddr = new Fl_Input(100, 100, 120, 25, "Enter IP here: ");
		add(m_ipAddr);

		//Forces the textbox to be shown on the screen
		m_ipAddr->redraw_label();
		m_ipAddr->value(" ");
		m_ipAddr->value("");

		//Show connect button to confirm connection
		Fl_Button* connectBtn = new Fl_Button(270, 260, 100, 50, "Connect");
		connectBtn->callback(onConnect, (void*)this);
		add(connectBtn);
		connectBtn->redraw();
	}
		break;
	}
}

void MainWindow::on_tick(void* _userData)
{
	if (m_state != 0)
	{
		MainWindow* mw = (MainWindow*)_userData;

		switch (m_state)
		{
		case 1:
			m_Server->on_tick();
		case 2:
			printf("Client tick\n");
			break;
		}
	}

	Fl::repeat_timeout(m_duration, tick, _userData);
}

void MainWindow::onConnect(Fl_Widget* _widget, void* _userData)
{
	MainWindow* mw = (MainWindow*)_userData;
	const char* ip = mw->m_ipAddr->value();
	printf("IP: %s\n", ip);
	
	if (mw->m_Client->Connect(ip))
	{

	}
}

void MainWindow::onServer(Fl_Widget* _widget, void* _userData)
{
	MainWindow* mw = (MainWindow*)_userData;

	if (mw->m_Server != nullptr)
	{
		delete mw->m_Server;
	}

	if (mw->m_Client != nullptr)
	{
		mw->m_Client->m_closed = true;
		mw->m_Client->Send("CLOSED");

		delete mw->m_Client;
	}

	mw->m_Server = new ServerSocket(8080);

	mw->m_Client = new ClientSocket();
	mw->m_Client->Connect(mw->m_Server->m_ipAddress);

	mw->m_state = 1;

	mw->changeInterface(mw->m_state);
}

void MainWindow::onClient(Fl_Widget* _widget, void* _userData)
{
	MainWindow* mw = (MainWindow*)_userData;

	if (mw->m_Server != nullptr)
	{
		delete mw->m_Server;
	}

	if (mw->m_Client != nullptr)
	{
		mw->m_Client->m_closed = true;
		mw->m_Client->Send("CLOSED");

		delete mw->m_Client;
	}

	mw->m_Client = new ClientSocket();
	mw->m_state = 2;


	mw->changeInterface(mw->m_state);
}

void MainWindow::onExit(Fl_Widget* _widget, void* _userData)
{
	MainWindow* self = (MainWindow*)_userData;
	self->hide();
}