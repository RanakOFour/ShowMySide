#include "MainWindow.h"
#include "Timer.h"
#include "ClientSocket.h"
#include "ServerSocket.h"
#include <string>
#include <iostream>
#include <FL/Fl_Button.H>
#include <FL/Fl_Window.H>
#include <FL/Fl_Menu_Bar.H>
#include <FL/Fl_Input.H>
#include <FL/Fl_Output.H>

MainWindow::MainWindow(int _w, int _h, std::string _name) 
	: Fl_Window(200, 200, _w, _h, _name.c_str()), 
	Timer(1.0), 
	m_ipAddr(nullptr),
	m_isHost(false), 
	m_state(0),
	m_Client(nullptr),
	m_Server(nullptr)
{
	Fl_Button* startClient = new Fl_Button(((640 * 3) / 4) - 50, 160, 100, 100, "Join");
	Fl_Button* startServer = new Fl_Button((640 / 4) - 50, 160, 100, 100, "Host");
	startClient->callback(onClient, (void*)this);
	startServer->callback(onServer, (void*)this);
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
			printf("Client\n");
			break;
		}
	}

	Fl::repeat_timeout(m_duration, tick, _userData);
}

void MainWindow::onConnect(Fl_Widget* _widget, void* _userData)
{
	MainWindow* mw = (MainWindow*)_userData;
	const char* ip = mw->m_ipAddr->value();
	std::cout << "IP: " << ip << std::endl;
	
	if (mw->m_Client->Connect(ip))
	{

	}
}

void MainWindow::onServer(Fl_Widget* _widget, void* _userData)
{
	MainWindow* mw = (MainWindow*)_userData;
	mw->m_isHost = true;
	mw->m_Server = new ServerSocket(8080);
	mw->m_state = 1;

	std::string ipAddressString = mw->m_Server->m_ipAddress;
	std::cout << "Top IP: " << ipAddressString << std::endl;

	//Create client for host
	mw->m_Client = new ClientSocket();
	mw->m_Client->Connect(ipAddressString);

	//Remove client and server selection buttons
	mw->delete_child(1);
	mw->delete_child(0);
	mw->redraw();

	Fl_Output* ipAddress = new Fl_Output(100, 0, 300, 25, "Local IP: ");
	ipAddress->box(FL_NO_BOX);
	mw->add(ipAddress);
	ipAddress->insert(ipAddressString.c_str(), sizeof(ipAddressString.c_str()));
}

void MainWindow::onClient(Fl_Widget* _widget, void* _userData)
{
	MainWindow* mw = (MainWindow*)_userData;
	mw->m_Client = new ClientSocket();
	mw->m_state = 2;

	//Remove client and server selection buttons
	mw->delete_child(1);
	mw->delete_child(0);
	mw->redraw();

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