#include "Timer.h"
#include "Pugixml/pugixml.hpp"
#include <string>
#include <thread>

#pragma once
class ServerSocket;
class Server;

class Host
{
private:
	ServerSocket* m_socket;
	Server* m_server;

	/**
		Using a thread improves performance compared to having Host inherit from Timer and use Fl timeout events
	*/
	std::thread m_networkingThread;
	bool m_serverClosed;
public:
	Host(int _port, double _tickTime);
	~Host();

	/**
		Handles all the server messages and runs inside of m_networkingThread
	*/
	void MonitorNetwork();

	/**
		Stops server thread and tells clients to disconnect from the server, then shuts down the server
	*/
	bool CloseServer();
	std::string GetIPAddress();
};

