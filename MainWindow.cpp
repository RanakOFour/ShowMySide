#include "MainWindow.h"
#include "Timer.h"
#include "Host.h"
#include "Client.h"
#include "ImagePool.h"
#include "MenuWrapper.h"
#include <string>
#include <iostream>
#include <thread>
#include <FL/Fl_Group.H>
#include <FL/Fl_Multiline_Input.H>
#include <FL/Fl_Menu_Bar.H>
#include <FL/Fl_Input.H>
#include <FL/Fl_Output.H>
#include "FL/Fl_Text_Display.H"
#include <FL/Fl_Flex.H>
#include <FL/Fl_Button.H>
#include "FL/Fl_Box.H"


MainWindow::MainWindow(int _w, int _h, std::string _name)
	: Fl_Double_Window(200, 200, _w, _h, "Show My Side"),
	m_Client(nullptr),
	m_Host(nullptr),
	m_menuWrapper(nullptr),
	m_splashImage(nullptr),
	m_ipInput(nullptr),
	m_ipAddressBox(nullptr),
	m_lobbyEventLog(nullptr)
{
	end();

	//Menubar with a box in a group so the menubar resizes correctly
	m_menuWrapper = new MenuWrapper(this);
	add(m_menuWrapper);

	m_splashImage = new Fl_Box(100, 100, _w - 200, _h - 200, "");
	m_splashImage->image(ImagePool::GetImage(ImageType::SPLASH).get());
	add(m_splashImage);
	m_splashImage->show();

	m_ipInput = new Fl_Input(100, 100, 120, 25, "Enter IP here: ");
	add(m_ipInput);

	//Forces the textbox to be shown on the screen
	m_ipInput->value(" ");
	m_ipInput->value("");
	m_ipInput->box(FL_ENGRAVED_BOX);
	m_ipInput->redraw_label();
	m_ipInput->hide();

	//Show connect button to confirm connection
	m_connectBtn = new Fl_Button(270, 260, 100, 50, "Connect");
	m_connectBtn->callback(OnJoinServer, (void*)this);
	add(m_connectBtn);
	m_connectBtn->hide();

	//Set minimum size to 640, 360
	size_range(_w, _h);


	resizable(m_splashImage);
	icon(ImagePool::GetImage(ImageType::ICON).get());
}

MainWindow::~MainWindow()
{
}

bool MainWindow::AttemptConnection(std::string& _ipAddress)
{
	return m_Client->Connect(_ipAddress);
}

void MainWindow::ChangeLayout(LayoutType _newState)
{

	switch (_newState)
	{
	case LayoutType::JOIN_GAME:
		m_ipInput->show();
		m_connectBtn->show();
		break;


	//Overlay for ingame and host is the same
	case LayoutType::HOST:
		//Display server IP address for other clients to connect
		m_ipAddressBox = new Fl_Output(58, 30, 95, 25, "Local IP:");
		m_ipAddressBox->value(m_Host->GetIPAddress().c_str());
		m_ipAddressBox->box(FL_NO_BOX);

		add(m_ipAddressBox);

		m_ipAddressBox->redraw_label();

	case LayoutType::IN_GAME:
		m_ipInput->hide();
		m_connectBtn->hide();

		m_lobbyEventLog = new Fl_Text_Display(5, 60, w() - 10, h() - 65);
		add(m_lobbyEventLog);

		m_Client->SetLogDisplay(m_lobbyEventLog);
	break;

	case LayoutType::ABOUT:

		break;

	case LayoutType::HELP:

		break;
	}

	redraw();
}

void MainWindow::OnClientStart(Fl_Widget* _widget, void* _userData)
{

	MainWindow* mw = (MainWindow*)_userData;
	//Start client
	mw->m_Client = new Client();


	mw->ChangeLayout(LayoutType::JOIN_GAME);
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

	mw->ChangeLayout(LayoutType::HOST);
}

void MainWindow::OnJoinServer(Fl_Widget* _widget, void* _userData)
{

	MainWindow* mw = (MainWindow*)_userData;

	std::string ip = mw->m_ipInput->value();

	printf("IP: %s\n", ip.c_str());

	if (mw->AttemptConnection(ip))
	{
		mw->ChangeLayout(LayoutType::IN_GAME);
	}
}


void MainWindow::OnExit(Fl_Widget* _widget, void* _userData)
{
	MainWindow* mw = (MainWindow*)_userData;
	mw->hide();
}

void MainWindow::ShowAbout(Fl_Widget* _widget, void* _userData)
{
	MainWindow* mw = (MainWindow*)_userData;
	mw->ChangeLayout(LayoutType::ABOUT);
}

void MainWindow::ShowHelp(Fl_Widget* _widget, void* _userData)
{
	MainWindow* mw = (MainWindow*)_userData;
	mw->ChangeLayout(LayoutType::HELP);
}

