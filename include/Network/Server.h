#include "Timer.h"
#include "Network/ServerSocket.h"
#include "Network/ServerRecords.h"
#include "pugixml.hpp"

#include <string>
#include <thread>
#include <memory>

#pragma once

class Server
{
private:
	ServerSocket m_socket;
	ServerRecords m_records;

	/**
		Using a thread improves performance compared to having Server inherit from Timer and use Fl timeout events.
		This also helps keep server seperate from as much of the client's performance as possible
	*/
	std::thread m_networkingThread;
	bool m_serverClosed;
public:
	Server(int _port);
	~Server();

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

