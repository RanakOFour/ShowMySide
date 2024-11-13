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
	Fl_Menu_Bar* menu = new Fl_Menu_Bar(0, 0, _w, 30, "");
	menu->add("&Start/&Join Server", NULL, onClient, this);
	menu->add("&Start/&Create Server", NULL, onServer, this, FL_MENU_DIVIDER);
	menu->add("&Start/&Exit", NULL, onExit, this);

	Fl_Flex* menuFlex = new Fl_Flex(menu->w(), menu->h(), 0);
	menuFlex->add(menu);
	menuFlex->fixed(menu, 0);
	add_resizable(*menuFlex);
}

MainWindow::~MainWindow()
{
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
	mw->m_Server = new ServerSocket(8080);
	mw->m_state = 1;

	std::string ipAddressString = mw->m_Server->m_ipAddress;

	//Create client for host
	mw->m_Client = new ClientSocket();
	mw->m_Client->Connect(ipAddressString);

	//Display server IP address for other clients to connect
	Fl_Output* ipAddress = new Fl_Output(60, 30, 95, 25, "Local IP:");
	ipAddress->box(FL_NO_BOX);
	mw->add(ipAddress);

	ipAddress->insert(ipAddressString.c_str(), ipAddressString.length());
	ipAddress->redraw_label();

}

void MainWindow::onClient(Fl_Widget* _widget, void* _userData)
{
	MainWindow* mw = (MainWindow*)_userData;
	mw->m_Client = new ClientSocket();
	mw->m_state = 2;

	//Create ip address input box
	mw->m_ipAddr = new Fl_Input(100, 100, 120, 25, "Enter IP here: ");
	mw->add(mw->m_ipAddr);

	//Forces the textbox to be shown on the screen
	mw->m_ipAddr->redraw_label();
	mw->m_ipAddr->value(" ");
	mw->m_ipAddr->value("");

	Fl_Button* connectBtn = new Fl_Button(270, 260, 100, 50, "Connect");
	connectBtn->callback(onConnect, (void*)mw);
	mw->add(connectBtn);
}

void MainWindow::onExit(Fl_Widget* _widget, void* _userData)
{
	MainWindow* self = (MainWindow*)_userData;
	self->hide();
}