#include "MainWindow.h"
#include "Timer.h"
#include "ClientSocket.h"
#include "ServerSocket.h"
#include <string>
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
}

MainWindow::~MainWindow()
{
}

void MainWindow::on_tick(void* _userData)
{
	MainWindow* mw = (MainWindow*)_userData;
	//mw->hide();
	printf("Tick!\n");
}

/*
This will be useful

std::vector<std::shared_ptr<ClientSocket> > clients;
while(true)
{
 std::shared_ptr<ClientSocket> client = server.accept();
 if(client)
 {
 printf("Client Connected!\n");
 clients.push_back(client);
 }
 // TODO: Handle all clients
 printf("Tick...\n");
 Sleep(1000);
}

// Handle all clients
for(size_t ci = 0; ci < clients.size(); ++ci)
{
 std::string message;
 while(clients.at(ci)->receive(message))
 {
 printf("Message recived: %s\n", message.c_str());
 }
 if(clients.at(ci)->closed())
 {
 printf("Client Disconnected\n");
 clients.erase(clients.begin() + ci);
 --ci;
 }
}
*/