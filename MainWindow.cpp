#include "MainWindow.h"
#include "Timer.h"
#include "ClientSocket.h"
#include "ServerSocket.h"
#include <string>
#include <FL/Fl_Button.H>
#include <FL/Fl_Window.H>
#include <FL/Fl_Flex.H>

MainWindow::MainWindow(int x, int y, int w, int h, std::string name, bool hasServer) : Fl_Window(x, y, w, h, name.c_str()), Timer(1.0)
{
	m_isServer = hasServer;

	if (m_isServer)
	{
		m_Server = new ServerSocket(8080);
	}
	else
	{
		m_Server = nullptr;
	}

	m_Client = new ClientSocket();

	Fl_Button* ConnectBtn = new Fl_Button(0, 0, 100, 100, "Connect");
	ConnectBtn->callback();
}

MainWindow::~MainWindow()
{
}

void MainWindow::on_tick(void* _userData)
{
	MainWindow* mw = (MainWindow*)_userData;
	if (m_Server != nullptr)
	{
		m_Server->on_tick();
	}
	printf("Tick!");
	Fl::repeat_timeout(m_duration, tick, _userData);
}