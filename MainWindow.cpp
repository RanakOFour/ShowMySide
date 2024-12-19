#include "MainWindow.h"
#include "Timer.h"
#include "Server.h"
#include "Client.h"
#include "ImagePool.h"
#include "MenuWrapper.h"
#include "FileTextDisplay.h"

#include "FL/Fl_Box.H"
#include <FL/Fl_Input.H>
#include <FL/Fl_Output.H>
#include <FL/Fl_Button.H>
#include "FL/Fl_Text_Display.H"

#include <string>
#include <iostream>
#include <thread>
#include <memory>
#include <fstream>


MainWindow::MainWindow(int _w, int _h, std::string _name)
	: Timer((double)(1.0f / 30.0f)),
	Fl_Double_Window(200, 200, _w, _h, "Show My Side"),
	m_Client(nullptr),
	m_Server(nullptr),
	m_aboutText("./text/about.txt", _w, _h),
	m_menuWrapper(this),
	m_splashImage(100, 100, _w - 200, _h - 200, ""),
	m_ipInput(100, 100, 120, 25, "Enter IP here: "),
	m_ipAddressBox(58, 30, 95, 25, "Local IP:"),
	m_connectBtn(270, 260, 100, 50, "Connect"),
	m_lobbyEventLogDisplay(5, 60, w() - 10, h() - 65)
{
	end();

	//Set minimum size to 640, 360
	size_range(_w, _h);

	//Menubar with a box in a group so the menubar resizes correctly
	add(m_menuWrapper);

	// Splash image shown on startup
	m_splashImage.image(ImagePool::GetImage(ImageType::SPLASH).get());
	add(m_splashImage);
	m_splashImage.show();

	resizable(m_splashImage);
	icon(ImagePool::GetImage(ImageType::ICON).get());


	// Create the rest of the widgets and hide them
	add(m_aboutText);
	m_aboutText.hide();

	add(m_ipInput);

	//Forces the textbox to be shown on the screen when first shown
	m_ipInput.value(" ");
	m_ipInput.value("");
	m_ipInput.box(FL_ENGRAVED_BOX);
	m_ipInput.redraw_label();
	m_ipInput.hide();

	//Show connect button to confirm connection
	m_connectBtn.callback(OnJoinServer, this);
	add(m_connectBtn);
	m_connectBtn.hide();

	add(m_lobbyEventLogDisplay);
	m_lobbyEventLogDisplay.hide();

	m_ipAddressBox.box(FL_NO_BOX);
	add(m_ipAddressBox);
	m_ipAddressBox.hide();

	callback(OnExit, this);
}

MainWindow::~MainWindow()
{
	m_aboutText.buffer(NULL);
}

void MainWindow::OnTick()
{
	if (m_Client && m_Client->Closed())
	{
		if (m_Server)
		{
			m_Server->CloseServer();
			m_Server = nullptr;
		}

		m_Client = nullptr;

		ChangeLayout(LayoutType::SPLASH_SCREEN);
	}
}

void MainWindow::ChangeLayout(LayoutType _newState)
{
	//Skips hiding the menubar
	for (int i = 1; i < children(); i++)
	{
		child(i)->hide();
	}

	switch (_newState)
	{
	case LayoutType::SPLASH_SCREEN:
		m_splashImage.show();
		break;

	case LayoutType::JOIN_GAME:
		m_ipInput.show();
		m_connectBtn.show();
		break;

	//Overlay for ingame and Server is the same
	case LayoutType::SERVER:
		//Display server IP address for other clients to connect
		m_ipAddressBox.value(m_Server->GetIPAddress().c_str());
		m_ipAddressBox.show();
		m_ipAddressBox.redraw_label();

	case LayoutType::IN_GAME:
		m_lobbyEventLogDisplay.show();
	break;

	case LayoutType::ABOUT:
		m_aboutText.show();
		break;
	}

	redraw();
}

void MainWindow::OnClientStart(Fl_Widget* _widget, void* _userData)
{
	MainWindow* mw = (MainWindow*)_userData;

	//Start client
	mw->m_Client = std::make_unique<Client>();
	mw->ChangeLayout(LayoutType::JOIN_GAME);
}

void MainWindow::OnServerStart(Fl_Widget* _widget, void* _userData)
{
	MainWindow* mw = (MainWindow*)_userData;
	//Start a new Server and connect client to it
	
							 //port, timer duration
	mw->m_Server = std::make_unique<Server>(8080);

	mw->m_Client = std::make_unique<Client>();

	std::string ip = mw->m_Server->GetIPAddress();
	mw->m_Client->Connect(ip);
	mw->m_Client->SetLogDisplay(&(mw->m_lobbyEventLogDisplay));

	mw->ChangeLayout(LayoutType::SERVER);
}

void MainWindow::OnJoinServer(Fl_Widget* _widget, void* _userData)
{
	MainWindow* mw = (MainWindow*)_userData;

	std::string ip = mw->m_ipInput.value();

	printf("IP: %s\n", ip.c_str());

	if (mw->m_Client->Connect(ip))
	{
		mw->m_Client->SetLogDisplay(&(mw->m_lobbyEventLogDisplay));
		mw->ChangeLayout(LayoutType::IN_GAME);
	}
}


void MainWindow::OnExit(Fl_Widget* _widget, void* _userData)
{
	MainWindow* mw = (MainWindow*)_userData;
	mw->hide();

	if (mw->m_Client)
	{
		mw->m_Client.reset();
	}

	if (mw->m_Server)
	{
		mw->m_Server.reset();
	}
}

void MainWindow::ShowAbout(Fl_Widget* _widget, void* _userData)
{
	MainWindow* mw = (MainWindow*)_userData;
	mw->ChangeLayout(LayoutType::ABOUT);
}

