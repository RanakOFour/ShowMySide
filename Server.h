#include "Timer.h"
#include "Pugixml/pugixml.hpp"
#include <string>
#include <thread>

#pragma once
class ServerSocket;
class ServerRecords;

class Server
{
private:
	ServerSocket* m_socket;
	ServerRecords* m_records;

	/**
		Using a thread improves performance compared to having Server inherit from Timer and use Fl timeout events
	*/
	std::thread m_networkingThread;
	bool m_serverClosed;
public:
	Server(int _port, double _tickTime);
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

