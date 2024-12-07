#include "Timer.h"
#include "Pugixml/pugixml.hpp"
#include <string>
#include <thread>

#pragma once
class ServerSocket;
class HeadlessLobby;

class Host
{
private:
	ServerSocket* m_server;
	HeadlessLobby* m_lobby;

	/**
		Using a thread improves performance compared to having Host inherit from Timer and use Fl timeout events
	*/
	std::thread* m_networkingThread;
	bool m_closeServer;
public:
	Host(int _port, double _tickTime);
	~Host();

	/**
		Handles all the server messages and runs inside of m_networkingThread
	*/
	void MonitorNetwork();
	std::string GetIPAddress();
};

